/*============================================================================*/
/* NameValidator.hpp - Validates the names of objects used in the plan.       */
/*                     e.g. tables, columns, indexes, etc                     */
/*============================================================================*/

#include "BASE.h"
#include "NameValidator.hpp"
#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "PlanConstants.hpp"
#include "GraphNav.hpp"
#include "GraphEditor.hpp"
#include "MetadataNode.hpp"
#include "ShadowRoot.hpp"
#include "SqlException.hpp"

#include "SeApi.hpp"
#include "InfoSet.hpp"
#include "TableMetadata.hpp"

#include "TableRef.hpp"
#include "ColumnRef.hpp"
#include "ColumnWildcard.hpp"
#include "TableDef.hpp"
#include "ColumnDefs.hpp"
#include "ColumnDef.hpp"
#include "IndexDefs.hpp"
#include "IndexDef.hpp"
#include "ProjectOp.hpp"
#include "ProjectedColumn.hpp"
#include "DataSource.hpp"
#include "StatementMarker.hpp"


using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/



/*----------------------------------------------------------------------------*/
/* Walk the graph and look up all names encountered in the system metadata.   */
/* This function will fill in any unresolved name parts as well as populate   */
/* the plan with metadata.                                                    */
/* A SqlException will be thrown if a name cannot be resolved.                */
/*----------------------------------------------------------------------------*/
void NameValidator::vValidateNames(PlanNode *pRootNode)
{
  STORE::SeApi* pSeApi = new STORE::SeApi();

  // First find the metadata node
  QueryNode* pMetadataNode = ShadowRoot::pMetadataNode(pRootNode);
  ASSERT(pMetadataNode);

  // Find the /plan node
  QueryNode* pPlanNode = ShadowRoot::pPlanNode(pRootNode);
  ASSERT(pPlanNode);

  // The plan node should have one child which is a StatementMarker node
  ASSERT(pPlanNode->nChildren() == 1);
  ASSERT(StatementMarker::bIsOfType(pPlanNode->pFirstChild()));
  QueryNode* pStatementMarker = static_cast<QueryNode*>(pPlanNode->pFirstChild());

  // We want to iterate over each logical subgraph, bounded by StatementMarkers,
  // and run the validation checks on each of them.

  auto fVisitor = [pRootNode, pMetadataNode, pSeApi]
      (PlanNode *pNode, s256 &sPath, i8 iDepth, void *pvArg) -> GraphNav::eWalkCtrl {

    if (!StatementMarker::bIsOfType(pNode)) {
      return GraphNav::WALK_CONTINUE;
    }

    QueryNode* pStatementMarker = static_cast<QueryNode*>(pNode);

    // Run the validation logic on the logical subgraph of each StatementMarker encountered.
    try {
      // First resolve all of the tables in the plan. We will need them to
      // then resolve columns and indexes.
      vValidateTables(pRootNode, pStatementMarker, pMetadataNode, pSeApi);

      // Convert any column wildcards to column references
      vResolveColumnWildcards(pRootNode, pStatementMarker, pMetadataNode);

      // Now resolve the columns against the known tables in the plan.
      vValidateColumns(pRootNode, pStatementMarker, pMetadataNode);

    } catch (SqlException& e) {
      delete pSeApi;
      throw;
    }

    return GraphNav::WALK_CONTINUE;
  };

  // Walk the entire plan graph to look for StatementMarkers
  GraphNav::bGlobalWalk(pStatementMarker, false, true, fVisitor, pRootNode);

  delete pSeApi;

#ifdef WITH_ASSERT_MACRO
  vAssertNamesResolved(pRootNode, pPlanNode);
#endif
}

/*----------------------------------------------------------------------------*/
/* Validate and qualify all of the tableRefs in the graph                     */
/*----------------------------------------------------------------------------*/
void NameValidator::vValidateTables(PlanNode *pRootNode, QueryNode* pStartNode, QueryNode* pMetadataNode, STORE::SeApi* pSeApi)
{
  ASSERT(pRootNode);
  ASSERT(pMetadataNode);
  ASSERT(pStartNode);

  // Walk the graph looking for TableRefs
  auto fVisitor = [&pMetadataNode, &pSeApi](PlanNode *pNode, s256 &sPath, i8 iDepth, void *pvArg) -> GraphNav::eWalkCtrl {

    PlanNode* pRootNode = static_cast<PlanNode*>(pvArg);

    if (!TableRef::bIsOfType(pNode)) {
      return GraphNav::WALK_CONTINUE;   // Keep walking
    }

    QueryNode* pTableRef = static_cast<QueryNode*>(pNode);

    Stz* szCatalogName = TableRef::szCatalog(static_cast<QueryNode*>(pNode));
    Stz* szSchemaName  = TableRef::szSchema(static_cast<QueryNode*>(pNode));
    Stz* szTableName   = TableRef::szTable(static_cast<QueryNode*>(pNode));

    // Is this table already in the metadata? It must have a fully qualified name if it does.
    if (TableRef::bIsFullyQualified(pTableRef)) {
      QueryNode* pTableDef = MetadataNode::pTableDef(pMetadataNode, szCatalogName, szSchemaName, szTableName);
      if (pTableDef) {
        // Table is in the metadata. Nothing to do here.
        return GraphNav::WALK_CONTINUE;   // Keep walking
      }
    }

    // Look up table in SE API
    STORE::InfoSet* pTableInfoSet = pSeApi->pSearchTables(szCatalogName, szSchemaName, szTableName);
    ASSERT(pTableInfoSet);

    if (pTableInfoSet->nRows() == 0) {
      // We couldn't resolve the table in the SE metadata
      delete pTableInfoSet;
      NameValidationException::vThrowUnknownTable(szCatalogName, szSchemaName, szTableName);
    }

    if (pTableInfoSet->nRows() > 1) {
      // We've resolved more than one table in the metadata. The table name is ambiguous.
      NameValidationException::vThrowAmbiguousTable(szCatalogName, szSchemaName, szTableName, pTableInfoSet);
    }

    ASSERT(pTableInfoSet->nRows() == 1);
    Row* pTblRow = pTableInfoSet->pFirstRow();
    TableMetadata* pTableMeta = static_cast<TableMetadata*>(pTblRow);

    // Get the full name from the metadata as we may not have had all of the details before
    szCatalogName = pTableMeta->szCatalogName();
    szSchemaName = pTableMeta->szSchemaName();
    szTableName = pTableMeta->szTableName();

    // Retrieve the column definitions and make a node our of these
    QueryNode* pColumnDefs = ColumnDefs::pCreate();
    InfoSet* pColumnInfoSet = pSeApi->pColumnDefs(szCatalogName, szSchemaName, szTableName, nullptr);

    for (Row* pColRow = pColumnInfoSet->pFirstRow();
         pColRow;
         pColRow = pColumnInfoSet->pNextRow(pColRow)) {

      ColumnMetadata* pColMeta = static_cast<ColumnMetadata*>(pColRow);

      // Convert the metadata type string to an enum and use this to create
      // a QueryNode of the correct type.
      QueryNode* pTypeNode = SqlType::pCreate(static_cast<eCT>(pColMeta->iTypeEnum()));

      // Make the column definition node and add it to the list of column definitions
      QueryNode* pColumnDef = ColumnDef::pCreate(pColMeta->szCatalogName(),
                                                 pColMeta->szSchemaName(),
                                                 pColMeta->szTableName(),
                                                 pColMeta->szColumnName(),
                                                 pTypeNode,
                                                 !pColMeta->bIsNullable(),
                                                 pColMeta->iColumnNumber(),
                                                 pColMeta->iColumnLength(),
                                                 pColMeta->iColumnScale());
      ColumnDefs::vAddColumnDef(pColumnDefs, pColumnDef);
    }

    // TODO: Get the index definitions and make a node out of it.
    // TODO: For now this is empty
    QueryNode* pIndexDefs = IndexDefs::pCreate();

    // Now form a TableDef object out of the table metadata, columnDefs and indexDefs
    QueryNode* pTableDef;
    pTableDef = TableDef::pCreate(szCatalogName, szSchemaName, szTableName, pColumnDefs, pIndexDefs);

    // Add to the plan metadata
    MetadataNode::vAddTable(pMetadataNode, pTableDef);

    // Qualify the TableRef
    TableRef::vQualify(static_cast<QueryNode*>(pNode), szCatalogName, szSchemaName, szTableName);

    delete pTableInfoSet;
    delete pColumnInfoSet;

    return GraphNav::WALK_CONTINUE;   // Keep walking
  };

  // Walk the /plan node, it's properties and its children
  GraphNav::bBoundedWalk(pStartNode, true, true, fVisitor, pRootNode);
}

/*----------------------------------------------------------------------------*/
/* Convert any column wildcards into column references                        */
/*----------------------------------------------------------------------------*/
void NameValidator::vResolveColumnWildcards(PlanNode *pRootNode, QueryNode* pStartNode, QueryNode* pMetadataNode)
{
  ASSERT(pRootNode);
  ASSERT(pMetadataNode);
  ASSERT(pStartNode);

  // Walk the graph looking for ColumnWildcards
  auto fVisitor = [&pMetadataNode](PlanNode *pNode, s256 &sPath, i8 iDepth, void *pvArg) -> GraphNav::eWalkCtrl {

    PlanNode* pRootNode = static_cast<PlanNode*>(pvArg);

    if (!ProjectOp::bIsOfType(pNode)) {
      return GraphNav::WALK_CONTINUE;   // Keep walking
    }

    QueryNode* pProjectOp = static_cast<QueryNode*>(pNode);

    // Get the list of projected expressions and iterate these,
    // looking for columnWildcards.
    QueryNode* pProjectedColumns = ProjectOp::pProjectedColumns(pProjectOp);

    if (pProjectedColumns->nChildren() == 0) {
      return GraphNav::WALK_CONTINUE;   // Keep walking
    }

    b1 bRetry = true;

    while (bRetry) {
      bRetry = false;
      PlanNodeIter *pIter = pProjectedColumns->pNewChildrenIter();

      while (pIter->bHasNext()) {
        QueryNode *pProjectedColumn = static_cast<QueryNode *>(pIter->pNext());

        // Get the expr out of the ProjectedColumn
        QueryNode *pExpr = ProjectedColumn::pExprNode(pProjectedColumn);

        if (!ColumnWildcard::bIsOfType(pExpr)) {
          continue;
        }

        // We've got a columnWildcard. We need to remove this projectedColumn
        // from the projectedColumns list and replace it with new projectedColumn
        // instances that have columnRefs from the resolved tables.

        // First get any catalog/schema/table scopes that may limit the wildcard
        Stz* szCatalogName = ColumnWildcard::szCatalog(static_cast<QueryNode*>(pExpr));
        Stz* szSchemaName  = ColumnWildcard::szSchema(static_cast<QueryNode*>(pExpr));
        Stz* szTableName   = ColumnWildcard::szTable(static_cast<QueryNode*>(pExpr));

        // Now generate a list of ProjectedColumn instances, but importantly we can only
        // use tableRefs that we find underneath this part of the graph, as there can be
        // other tableRefs that are not visible to this part of the query.
        PlanNodeList* pResolvedWildcardList = pLookupWildcardColumns(static_cast<QueryNode*>(pNode),
                                                                     pRootNode,
                                                                     szCatalogName,
                                                                     szSchemaName,
                                                                     szTableName);

        // Ensure that the returned list is a list of ProjectedColumn instances
#ifdef WITH_ASSERT_MACRO
        PlanNodeIter* pResolvedIter = pResolvedWildcardList->pNewIter();
        while (pResolvedIter->bHasNext()) {
          PlanNode* pResolvedNode = pResolvedIter->pNext();
          ASSERT(ProjectedColumn::bIsOfType(pResolvedNode));
        }
        delete pResolvedIter;
#endif

        if (pResolvedWildcardList->nItems() == 0) {
          delete pResolvedWildcardList;
          NameValidationException::vThrowUnresolvableColumnWildcard(szCatalogName, szSchemaName, szTableName);
        }

        // Now replace the ProjectedColumn that contains the columnWildcard with
        // each of the resolved ProjectedColumn instances that contain columnRefs.
        GraphEditor::vReplaceChild(pProjectedColumns, pProjectedColumn, pResolvedWildcardList);

        ASSERT(pResolvedWildcardList->nItems() == 0);
        delete pResolvedWildcardList;

        // We can't modify the projectedColumns list and keep iterating it.
        // And there may be more columnWildcards. So we need to start again
        // and reiterate it from the beginning with a new iterator.
        bRetry = true;
        break;
      }

      delete pIter;
    } // while (bRetry)

    return GraphNav::WALK_CONTINUE;   // Keep walking, looking for more ProjectOps
  };

  // Walk the /plan node and its children only. We only want to stop at ProjectOps.
  // We do this so that we can modify the properties of the ProjectOp without it
  // affecting the walking of the graph. We do not want to modify the graph that
  // we are walking as this will lead to bad conclusions.
  GraphNav::bBoundedWalk(pStartNode, false, true, fVisitor, pRootNode);
}


/*----------------------------------------------------------------------------*/
/* Look up and return a list of columns that match a column wildcard          */
/* for a given location in the plan hierarchy.                                */
/*----------------------------------------------------------------------------*/
PlanNodeList* NameValidator::pLookupWildcardColumns(QueryNode* pStartNode, PlanNode* pRootNode,
                                                    Stz* szCatalog, Stz* szSchema, Stz* szTable)
{
  ASSERT(pStartNode);
  ASSERT(PlanObject::bIsOfType(pStartNode));

  PlanNodeList* pProjectedColumnList = new PlanNodeList();


  // Walk the graph from here on down, looking for all data sources.
  auto fVisitor =
      [&pProjectedColumnList, &szCatalog, &szSchema, &szTable](PlanNode *pNode, s256 &sPath, i8 iDepth, void *pvArg) -> GraphNav::eWalkCtrl {

    PlanNode* pRootNode = static_cast<PlanNode*>(pvArg);

    if (!DataSource::bIsOfType(pNode)) {
      return GraphNav::WALK_CONTINUE;   // Keep walking
    }

    QueryNode* pDataSourceOp = static_cast<QueryNode*>(pNode);
    Stz* szSrcCatalog = DataSource::szCatalogName(pDataSourceOp);
    Stz* szSrcSchema  = DataSource::szSchemaName(pDataSourceOp);
    Stz* szSrcTable   = DataSource::szTableName(pDataSourceOp);

    // Check if the wildcard constraints match the data source
    b1 bConstraintsMatch = 
        (!szCatalog || (szSrcCatalog && szCatalog->bSame(szSrcCatalog))) &&
        (!szSchema  || (szSrcSchema  && szSchema->bSame(szSrcSchema))  ) &&
        (!szTable   || (szSrcTable   && szTable->bSame(szSrcTable))    );

    if (!bConstraintsMatch) {
      // Don't include these columns
      return GraphNav::WALK_CONTINUE;   // Keep walking
    }

    // We do want to include these columns
    // Get the column definitions of the data source and convert them to columnRefs
    PlanNodeList* pColumnDefList = DataSource::pColumnDefs(pDataSourceOp, pRootNode);

    // Iterate through the columnDefs, convert them to columnRefs and add them to the list
    PlanNodeIter* pIter = pColumnDefList->pNewIter();
    while (pIter->bHasNext()) {
      QueryNode* pColumnDef = static_cast<QueryNode*>(pIter->pNext());

      QueryNode* pColumnRef = ColumnRef::pCreate( ColumnDef::szCatalogName(pColumnDef),
                                                  ColumnDef::szSchemaName(pColumnDef),
                                                  ColumnDef::szTableName(pColumnDef),
                                                  ColumnDef::szColumnName(pColumnDef) );

      // Make a projectedColumn from the columnRef.
      // We're not going to give the column an alias.
      QueryNode* pProjectedColumn = ProjectedColumn::pCreate(pColumnRef, nullptr);

      // Add it to the list
      pProjectedColumnList->vAppend(pProjectedColumn);
    }

    // Clean up
    delete pIter;
    pColumnDefList->vRemoveAll();
    delete pColumnDefList;

    return GraphNav::WALK_CONTINUE;   // Keep walking
  };


  GraphNav::bBoundedWalk(pStartNode, false, true, fVisitor, pRootNode);

  return pProjectedColumnList;
}

/*----------------------------------------------------------------------------*/
/* Validate and qualify all of the columnRefs using the known tables          */
/*----------------------------------------------------------------------------*/
void NameValidator::vValidateColumns(PlanNode *pRootNode, QueryNode* pStartNode, QueryNode* pMetadataNode)
{
  ASSERT(pRootNode);
  ASSERT(pMetadataNode);
  ASSERT(pStartNode);

  // Walk the graph looking for ColumnRefs
  auto fVisitor = [&pMetadataNode](PlanNode *pNode, s256 &sPath, i8 iDepth, void *pvArg) -> GraphNav::eWalkCtrl {

    PlanNode* pRootNode = static_cast<PlanNode*>(pvArg);

    if (!ColumnRef::bIsOfType(pNode)) {
      return GraphNav::WALK_CONTINUE;   // Keep walking
    }

    QueryNode* pColumnRef = static_cast<QueryNode*>(pNode);

    Stz* szCatalogName = ColumnRef::szCatalog(static_cast<QueryNode*>(pNode));
    Stz* szSchemaName  = ColumnRef::szSchema(static_cast<QueryNode*>(pNode));
    Stz* szTableName   = ColumnRef::szTable(static_cast<QueryNode*>(pNode));
    Stz* szColumnName  = ColumnRef::szColumn(static_cast<QueryNode *>(pNode));

    // Is this column already in the metadata? It must have a fully qualified name if it does.
    if (ColumnRef::bIsFullyQualified(pColumnRef)) {
      QueryNode* pColumnDef = MetadataNode::pColumnDef(pMetadataNode, szCatalogName, szSchemaName, szTableName, szColumnName);
      if (pColumnDef) {
        // Column is in the metadata. Nothing to do here.
        return GraphNav::WALK_CONTINUE;   // Keep walking
      }
    }

    // We need to find this partially-qualified column in the plan graph metadata.
    // Be careful, it might match against multiple tables.
    PlanNodeList* pTableDefList = MetadataNode::pMatchColumnRef(pMetadataNode, pColumnRef);

    if (pTableDefList->nItems() == 0) {
      delete pTableDefList;
      NameValidationException::vThrowUnknownColumn(szCatalogName, szSchemaName, szTableName, szColumnName);

    } else if (pTableDefList->nItems() > 1) {
      delete pTableDefList;
      NameValidationException::vThrowAmbiguousColumn(szCatalogName, szSchemaName, szTableName, szColumnName, pTableDefList);
    }

    ASSERT(pTableDefList->nItems() == 1);

    // Qualify the ColumnRef
    QueryNode* pTableDef = static_cast<QueryNode*>(pTableDefList->pPop());
    szCatalogName = TableDef::szCatalogName(pTableDef);
    szSchemaName  = TableDef::szSchemaName(pTableDef);
    szTableName   = TableDef::szTableName(pTableDef);

    ColumnRef::vQualify(static_cast<QueryNode*>(pNode), szCatalogName, szSchemaName,
                        szTableName, szColumnName);

    delete pTableDefList;
    return GraphNav::WALK_CONTINUE;   // Keep walking, looking for more ColumnRefs
  };

  // Walk the /plan node, it's properties and its children
  GraphNav::bBoundedWalk(pStartNode, true, true, fVisitor, pRootNode);
}

/*----------------------------------------------------------------------------*/
/* Assert that all tables and columns are fully resolved and that there are   */
/* no columnWildcards left in the plan                                        */
/*----------------------------------------------------------------------------*/
void NameValidator::vAssertNamesResolved(PlanNode *pRootNode, QueryNode* pPlanNode)
{
  // Walk every node (property and child). If we encounter a:
  //  - ColumnWildcard,
  //  - ColumnRef that is not fully resolved, or
  //  - TableRef that is not fully resolved
  // then assert an error.

  auto fVisitor = [](PlanNode *pNode, s256 &sPath, i8 iDepth, void *pvArg) -> GraphNav::eWalkCtrl {

    ASSERT(!ColumnWildcard::bIsOfType(pNode));

    if (ColumnRef::bIsOfType(pNode)) {
      ASSERT(ColumnRef::bIsFullyQualified(static_cast<QueryNode*>(pNode)));
      return GraphNav::WALK_CONTINUE;
    }

    if (TableRef::bIsOfType(pNode)) {
      ASSERT(TableRef::bIsFullyQualified(static_cast<QueryNode*>(pNode)));
      return GraphNav::WALK_CONTINUE;
    }

    return GraphNav::WALK_CONTINUE;
  };

  GraphNav::bGlobalWalk(pPlanNode, true, true, fVisitor, pRootNode);
}

/*============================================================================*/

/*============================================================================*/



/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
