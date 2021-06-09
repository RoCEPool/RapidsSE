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
#include "AliasRef.hpp"
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
#include "SqlOperator.hpp"
#include "SetOp.hpp"


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
  QueryNode* pStatementMarker = _pQueryNode(pPlanNode->pFirstChild());

  // We want to iterate over each logical subgraph, bounded by StatementMarkers,
  // and run the validation checks on each of them.
  //
  // First we will validate all of the tables. Then we can validate all of the columns.
  // This is because validating columns will do a global walk downwards (because operators
  // like UNIONs hide tableRefs below them in a bounded walk). So we need all tables
  // to be qualified first.

  auto fTableVisitor = [pRootNode, pMetadataNode, pSeApi]
      (PlanNode *pNode, s256 &sPath, i8 iDepth, void *pvArg) -> GraphNav::eWalkCtrl {

    UNUSED(sPath);
    UNUSED(iDepth);
    UNUSED(pvArg);

    if (!StatementMarker::bIsOfType(pNode)) {
      return GraphNav::WALK_CONTINUE;
    }

    QueryNode* pStatementMarker = _pQueryNode(pNode);

    // Run the validation logic on the logical subgraph of each StatementMarker encountered.
    try {
      // First resolve all of the tables in the plan. We will need them to
      // then resolve columns and indexes.
      vValidateTables(pRootNode, pStatementMarker, pMetadataNode, pSeApi);

    } catch (SqlException& e) {
      delete pSeApi;
      throw;
    }

    return GraphNav::WALK_CONTINUE;
  };


  auto fColumnVisitor = [pRootNode, pMetadataNode, pSeApi]
      (PlanNode *pNode, s256 &sPath, i8 iDepth, void *pvArg) -> GraphNav::eWalkCtrl {

    UNUSED(sPath);
    UNUSED(iDepth);
    UNUSED(pvArg);

    if (!StatementMarker::bIsOfType(pNode)) {
      return GraphNav::WALK_CONTINUE;
    }

    QueryNode* pStatementMarker = _pQueryNode(pNode);

    // Run the validation logic on the logical subgraph of each StatementMarker encountered.
    try {
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

  // Walk the entire plan graph to look for StatementMarkers.
  // Validate the tables first, then the columns separately.
  // We do a bottom up walk for the column validation so that we can qualify
  // any columnRefs lower in the plan before we hit any aliases higher up that
  // then copy those expressions.
  GraphNav::bGlobalWalk(pStatementMarker, false, true, fTableVisitor, pRootNode);
  GraphNav::bGlobalWalkBottomUp(pStatementMarker, false, true, fColumnVisitor, pRootNode);

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

    UNUSED(sPath);
    UNUSED(iDepth);

    PlanNode* pRootNode = static_cast<PlanNode*>(pvArg);
    UNUSED(pRootNode);

    if (!TableRef::bIsOfType(pNode)) {
      return GraphNav::WALK_CONTINUE;   // Keep walking
    }

    QueryNode* pTableRef = _pQueryNode(pNode);

    Stz* szCatalogName = TableRef::szCatalog(_pQueryNode(pNode));
    Stz* szSchemaName  = TableRef::szSchema(_pQueryNode(pNode));
    Stz* szTableName   = TableRef::szTable(_pQueryNode(pNode));

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

    // Now that the table is fully qualified, check again if this is already in the metadata
    QueryNode* pTableDef = MetadataNode::pTableDef(pMetadataNode, szCatalogName, szSchemaName, szTableName);
    if (pTableDef) {
      TableRef::vQualify(_pQueryNode(pNode), szCatalogName, szSchemaName, szTableName);
      delete pTableInfoSet;
      return GraphNav::WALK_CONTINUE;
    }

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
    pTableDef = TableDef::pCreate(szCatalogName, szSchemaName, szTableName, pColumnDefs, pIndexDefs);

    // Add to the plan metadata
    MetadataNode::vAddTable(pMetadataNode, pTableDef);

    // Qualify the TableRef
    TableRef::vQualify(_pQueryNode(pNode), szCatalogName, szSchemaName, szTableName);

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
  UNUSED(pMetadataNode);

  ASSERT(pRootNode);
  ASSERT(pMetadataNode);
  ASSERT(pStartNode);

  // Walk the graph looking for ColumnWildcards
  auto fVisitor = [](PlanNode *pNode, s256 &sPath, i8 iDepth, void *pvArg) -> GraphNav::eWalkCtrl {

    UNUSED(sPath);
    UNUSED(iDepth);

    PlanNode* pRootNode = static_cast<PlanNode*>(pvArg);

    if (!ProjectOp::bIsOfType(pNode)) {
      return GraphNav::WALK_CONTINUE;   // Keep walking
    }

    QueryNode* pProjectOp = _pQueryNode(pNode);

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
        Stz* szCatalogName = ColumnWildcard::szCatalog(_pQueryNode(pExpr));
        Stz* szSchemaName  = ColumnWildcard::szSchema(_pQueryNode(pExpr));
        Stz* szTableName   = ColumnWildcard::szTable(_pQueryNode(pExpr));

        // Now generate a list of ProjectedColumn instances, but importantly we can only
        // use tableRefs that we find underneath this part of the graph, as there can be
        // other tableRefs that are not visible to this part of the query.
        QueryNodeList* pResolvedWildcardList = pLookupWildcardColumns(_pQueryNode(pNode),
                                                                     pRootNode,
                                                                     szCatalogName,
                                                                     szSchemaName,
                                                                     szTableName);

        // Ensure that the returned list is a list of ProjectedColumn instances
#ifdef WITH_ASSERT_MACRO
        QueryNodeIter* pResolvedIter = pResolvedWildcardList->pNewIter();
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
QueryNodeList* NameValidator::pLookupWildcardColumns(QueryNode* pStartNode, PlanNode* pRootNode,
                                                    Stz* szCatalog, Stz* szSchema, Stz* szTable)
{
  ASSERT(pStartNode);
  ASSERT(PlanObject::bIsOfType(pStartNode));

  QueryNodeList* pProjectedColumnList = new QueryNodeList();


  // Walk the graph from here on down, looking for all data sources.
  auto fVisitor =
      [&pProjectedColumnList, &szCatalog, &szSchema, &szTable](PlanNode *pNode, s256 &sPath, i8 iDepth, void *pvArg) -> GraphNav::eWalkCtrl {

    UNUSED(sPath);
    UNUSED(iDepth);

    PlanNode* pRootNode = static_cast<PlanNode*>(pvArg);

    if (!DataSource::bIsOfType(pNode)) {
      return GraphNav::WALK_CONTINUE;   // Keep walking
    }

    QueryNode* pDataSourceOp = _pQueryNode(pNode);
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
    QueryNodeList* pColumnDefList = DataSource::pColumnDefs(pDataSourceOp, pRootNode);

    // Iterate through the columnDefs, convert them to columnRefs and add them to the list
    QueryNodeIter* pIter = pColumnDefList->pNewIter();
    while (pIter->bHasNext()) {
      QueryNode* pColumnDef = pIter->pNext();

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
  auto fVisitor = [&pMetadataNode, pStartNode](PlanNode *pNode, s256 &sPath, i8 iDepth, void *pvArg) -> GraphNav::eWalkCtrl {

    UNUSED(iDepth);

    PlanNode* pRootNode = static_cast<PlanNode*>(pvArg);

    if (!ColumnRef::bIsOfType(pNode)) {
      return GraphNav::WALK_CONTINUE;   // Keep walking
    }

    QueryNode* pColumnRef = _pQueryNode(pNode);

    Stz* szCatalogName = ColumnRef::szCatalog(_pQueryNode(pNode));
    Stz* szSchemaName  = ColumnRef::szSchema(_pQueryNode(pNode));
    Stz* szTableName   = ColumnRef::szTable(_pQueryNode(pNode));
    Stz* szColumnName  = ColumnRef::szColumn(static_cast<QueryNode *>(pNode));

    // Is this column already in the metadata? It must have a fully qualified name if it does.
    if (ColumnRef::bIsFullyQualified(pColumnRef)) {
      QueryNode* pColumnDef = MetadataNode::pColumnDef(pMetadataNode, szCatalogName, szSchemaName, szTableName, szColumnName);
      if (pColumnDef) {
        // Column is in the metadata. Nothing to do here.
        return GraphNav::WALK_CONTINUE;   // Keep walking
      }
    }

    // We either need to match this partially-qualified column against the plan
    // graph metadata or against an alias for a projected expression.
    //
    // Let's first consider matching it against a column from a table.
    // First we need to work out which table it might be from by looking for TableRefs
    // underneath the sql operator that contains this columnRef.

    // Get the Sql operator by using the current path and the starting node
    s256 sSqlOpPath;
    sSqlOpPath.vCopyTill(sPath, ':');

    QueryNode* pSqlOp = _pQueryNode(GraphNav::pFind(sSqlOpPath.szStz(), pStartNode));
    ASSERT(pSqlOp);
    ASSERT(SqlOperator::bIsOfType(pSqlOp));

    QueryNodeList* pTableDefList = pMatchingTableDefsFor(pColumnRef, pSqlOp, pRootNode, pMetadataNode);

    // We can check for an ambiguous column name
    if (pTableDefList->nItems() > 1) {
      // pTableDefList will be deleted in vThrowAmbiguousColumn() below
      NameValidationException::vThrowAmbiguousColumn(szCatalogName, szSchemaName, szTableName, szColumnName, pTableDefList);
    }

    if (szCatalogName || szSchemaName || szTableName) {
      // It must be a columnRef. Can't be an aliasRef

      if (pTableDefList->nItems() == 0) {
        delete pTableDefList;
        NameValidationException::vThrowUnknownColumn(szCatalogName, szSchemaName, szTableName, szColumnName);
      }

      ASSERT(pTableDefList->nItems() == 1);

      // Qualify the ColumnRef
      QueryNode* pTableDef = _pQueryNode(pTableDefList->pPop());
      szCatalogName = TableDef::szCatalogName(pTableDef);
      szSchemaName  = TableDef::szSchemaName(pTableDef);
      szTableName   = TableDef::szTableName(pTableDef);

      ColumnRef::vQualify(_pQueryNode(pNode), szCatalogName, szSchemaName,
                          szTableName, szColumnName);

      delete pTableDefList;
      return GraphNav::WALK_CONTINUE;   // Keep walking, looking for more ColumnRefs
    }

    // We've potentially got a list of matching table defs above, but we still need to check
    // for aliased expressions because there might be an ambiguity in alias/column names.
    QueryNodeList* pProjectedColList = pMatchingExprAliasesFor(szColumnName, pSqlOp, pRootNode);

    // A TableRef produces a projected column for each table with an alias set
    // to that table name. So we can end up with both a TableDef and an projected
    // column alias that both refer to the same table.
    if (pProjectedColList->nItems() >0 && pTableDefList->nItems() > 0) {

      QueryNodeIter* pTableDefIter = pTableDefList->pNewIter();
      while (pTableDefIter->bHasNext()) {

        QueryNode* pTableDef = pTableDefIter->pNext();
        ASSERT(TableDef::bIsOfType(pTableDef));

        QueryNodeIter* pProjectedColIter = pProjectedColList->pNewIter();
        while (pProjectedColIter->bHasNext()) {
          QueryNode* pProjectedCol = pProjectedColIter->pNext();
          ASSERT(ProjectedColumn::bIsOfType(pProjectedCol));

          QueryNode* pExpr = ProjectedColumn::pExprNode(pProjectedCol);
          ASSERT(!ColumnRef::bIsOfType(pExpr) || ColumnRef::bIsFullyQualified(pExpr));

          if (ColumnRef::bIsOfType(pExpr) &&
              ColumnRef::szCatalog(pExpr)->bSame(TableDef::szCatalogName(pTableDef)) &&
              ColumnRef::szSchema(pExpr)->bSame(TableDef::szSchemaName(pTableDef)) &&
              ColumnRef::szTable(pExpr)->bSame(TableDef::szTableName(pTableDef)) ) {

            // Remove the projected column from the list.
            pProjectedColIter->vRemove();
            delete pProjectedCol;
          }
        }
        delete pProjectedColIter;
      }

      delete pTableDefIter;
    }

    if (pProjectedColList->nItems() == 0) {
      // No aliased expressions. Could we match the columnRef against the table metadata?
      if (pTableDefList->nItems() == 0) {
        delete pTableDefList;
        delete pProjectedColList;
        NameValidationException::vThrowUnknownColumn(szCatalogName, szSchemaName, szTableName, szColumnName);
      }

      ASSERT(pTableDefList->nItems() == 1);

      // Qualify the ColumnRef
      QueryNode* pTableDef = _pQueryNode(pTableDefList->pPop());
      szCatalogName = TableDef::szCatalogName(pTableDef);
      szSchemaName  = TableDef::szSchemaName(pTableDef);
      szTableName   = TableDef::szTableName(pTableDef);

      ColumnRef::vQualify(_pQueryNode(pNode), szCatalogName, szSchemaName,
                          szTableName, szColumnName);

    } else if (pProjectedColList->nItems() == 1) {
      // We matched against an aliased (projected) expression

      if (pTableDefList->nItems() == 1) {
        // pTableDefList will be deleted in vThrowAmbiguousColumn() below
        // pProjectedColList will be deleted in vThrowAmbiguousColumn() below
        NameValidationException::vThrowAmbiguousColumn(szColumnName, pTableDefList, pProjectedColList);
      }

      ASSERT(pTableDefList->nItems() == 0);

      // We've got a single unambiguous aliased expression.
      // We need to change the ColumnRef to an AliasRef in this spot in the graph.
      // But we are also currently walking the graph and removing the node
      // would mess things up. So we need to transmogrify the node
      // in place instead.
      //
      // Create the AliasRef that we wish to transmogrify it to
      QueryNode* pProjectedCol = pProjectedColList->pPop();
      Text* pAliasName = new Text(ProjectedColumn::szAliasName(pProjectedCol));
      QueryNode* pAliasExpr = ProjectedColumn::pExprNode(pProjectedCol)->pDeepCopy(true);

      QueryNode* pAliasRef = AliasRef::pCreate(pAliasName, pAliasExpr);
      pColumnRef->vTransmogrifyTo(pAliasRef, true);
      delete pAliasRef;
      delete pProjectedCol;

    } else {
      ASSERT(pProjectedColList->nItems() > 1);
      delete pTableDefList;
      // pProjectedColList will be deleted in vThrowAmbiguousColumn() below
      NameValidationException::vThrowAmbiguousAlias(szColumnName, pProjectedColList);
    }

    delete pTableDefList;
    delete pProjectedColList;
    return GraphNav::WALK_CONTINUE;   // Keep walking, looking for more ColumnRefs
  };

  // Walk the /plan node, it's properties and its children.
  // We do a bottom up walk because there may be aliases higher up the plan
  // that refer to exprs lower down and we want to qualify columnRefs in those
  // lower expressions before we hit the aliases higher up.
  GraphNav::bBoundedWalkBottomUp(pStartNode, true, true, fVisitor, pRootNode);
}


/*----------------------------------------------------------------------------*/
/* Return a list of TableDef nodes that are reachable from a given sql op     */
/* and that contain a column as referenced by pColumnRef.                     */
/*----------------------------------------------------------------------------*/
QueryNodeList* NameValidator::pMatchingTableDefsFor(QueryNode* pColumnRef,
                                                    QueryNode* pSqlOp,
                                                    PlanNode*  pRootNode,
                                                    QueryNode* pMetadataNode)
{
  // List of matching TableDefs to be returned
  QueryNodeList* pTableDefList = new QueryNodeList();

  // Lambda called for each node under SqlOp, to look for TableRefs.
  auto fVisitor = [pColumnRef, pRootNode, pMetadataNode, &pTableDefList]
      (PlanNode *pNode, s256 &sPath, i8 iDepth, void *pvArg) -> GraphNav::eWalkCtrl {

    UNUSED(sPath);
    UNUSED(iDepth);
    UNUSED(pvArg);

    if (SetOp::bIsOfType(pNode)) {
      // We only walk down through the first child of a SetOp to resolve tableDefs.
      // Normally this could lead to issues whereby an operator above the SetOp could
      // refer to a column or alias that is not being projected. But at this point we
      // are only interested in resolving names. Later on (in the project pushdown stage)
      // we will check that each SQL operator has visibility of the expressions it is
      // trying to manipulate.

      // Recursively call the same function but only for the first child
      // of the SetOp.
      QueryNodeList* pSubList = pMatchingTableDefsFor(pColumnRef,
                                                      _pQueryNode(pNode->pFirstChild()),
                                                      pRootNode,
                                                      pMetadataNode);

      // Copy the results into the parent list
      QueryNode* pItem;
      while ( (pItem = pSubList->pPop()) != nullptr) {
        pTableDefList->vAppend(pItem);
      }

      delete pSubList;

      // We don't wish to descend this sub-tree because we manually descended
      // the first child above.
      return GraphNav::WALK_SKIP_SUBTREE;
    }

    if (!TableRef::bIsOfType(pNode)) {
      return GraphNav::WALK_CONTINUE;
    }

    QueryNode* pTableRef = _pQueryNode(pNode);
    ASSERT(TableRef::bIsFullyQualified(pTableRef));

    Stz* szCatalogName = ColumnRef::szCatalog(pColumnRef);
    Stz* szSchemaName  = ColumnRef::szSchema(pColumnRef);
    Stz* szTableName   = ColumnRef::szTable(pColumnRef);

    if ( (szCatalogName == nullptr || szCatalogName->bSame(TableRef::szCatalog(pTableRef))) &&
         (szSchemaName  == nullptr ||  szSchemaName->bSame(TableRef::szSchema(pTableRef)))  &&
         (szTableName   == nullptr ||   szTableName->bSame(TableRef::szTable(pTableRef))) ) {

      // This column partially or fully matches the name of this table. Get the
      // table definition of this table from the metadata
      QueryNode* pTableDef = MetadataNode::pTableDef(pMetadataNode,
                                                     TableRef::szCatalog(pTableRef),
                                                     TableRef::szSchema(pTableRef),
                                                     TableRef::szTable(pTableRef));
      ASSERT(pTableDef);

      // Test if the column belongs to this table definition
      QueryNode* pColumnDef = TableDef::pColumnDef(pTableDef, ColumnRef::szColumn(pColumnRef));
      if (pColumnDef) {
        // Add the table definition to the list
        pTableDefList->vAppend(pTableDef);
      }
    }

    // Keep looking, even if we found a matching TableRef - it might be an
    // ambiguous column.
    return GraphNav::WALK_CONTINUE;
  };

    // Walk the graph from pSqlOp looking for TableRefs
  GraphNav::bGlobalWalk(pSqlOp, true, true, fVisitor, pRootNode);

  return pTableDefList;
}

/*----------------------------------------------------------------------------*/
/* Return a list of ProjectedColumn nodes that are reachable from a given sql */
/* operator and that contain a given alias name. Need to free the list and    */
/* nodes returned.                                                            */
/*----------------------------------------------------------------------------*/
QueryNodeList* NameValidator::pMatchingExprAliasesFor(Stz* szAlias, QueryNode* pSqlOp, PlanNode* pRootNode)
{
  ASSERT(szAlias);
  ASSERT(pSqlOp);
  ASSERT(SqlOperator::bIsOfType(pSqlOp));
  ASSERT(pRootNode);
  ASSERT(ShadowRoot::bIsOfType(pRootNode));

  // List of matching ProjectedColumns to be returned
  QueryNodeList* pProjectedColumnList = new QueryNodeList();

  // Get the list of columns that are projected from pSqlOp and below.
  QueryNodeList* pProjectedCols = PlanObject::pColumnExprs(pSqlOp, pRootNode);

  QueryNode* pProjectedCol;

  while ((pProjectedCol = pProjectedCols->pPop()) != nullptr) {
    Stz* szColAlias = ProjectedColumn::szAliasName(pProjectedCol);

    if (szColAlias && szAlias->bSame(szColAlias)) {
      pProjectedColumnList->vAppend(pProjectedCol);

    } else {
      delete pProjectedCol;
    }
  }

  delete pProjectedCols;

  return pProjectedColumnList;
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

    UNUSED(sPath);
    UNUSED(iDepth);
    UNUSED(pvArg);

    ASSERT(!ColumnWildcard::bIsOfType(pNode));

    if (ColumnRef::bIsOfType(pNode)) {
      ASSERT(ColumnRef::bIsFullyQualified(_pQueryNode(pNode)));
      return GraphNav::WALK_CONTINUE;
    }

    if (TableRef::bIsOfType(pNode)) {
      ASSERT(TableRef::bIsFullyQualified(_pQueryNode(pNode)));
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
