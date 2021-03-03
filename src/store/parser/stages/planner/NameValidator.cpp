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
#include "MetadataNode.hpp"
#include "SqlException.hpp"

#include "SeApi.hpp"
#include "InfoSet.hpp"
#include "TableMetadata.hpp"

#include "TableRef.hpp"
#include "ColumnRef.hpp"
#include "TableDef.hpp"
#include "ColumnDefs.hpp"
#include "ColumnDef.hpp"
#include "IndexDefs.hpp"
#include "IndexDef.hpp"


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
  // First find the metadata node
  QueryNode* pMetadataNode = static_cast<QueryNode*>(GraphNav::pFind(const_cast<Stz*>(C_METADATA_PATH), pRootNode));
  ASSERT(pMetadataNode);

  // Find the /plan node
  QueryNode* pPlanNode = static_cast<QueryNode*>(GraphNav::pFind(const_cast<Stz*>(C_PLAN_PATH), pRootNode));
  ASSERT(pPlanNode);

  STORE::SeApi* pSeApi = new STORE::SeApi();

  try {
    // First resolve all of the tables in the plan. We will need them to
    // then resolve columns and indexes.
    vValidateTables(pRootNode, pPlanNode, pMetadataNode, pSeApi);

    // Now resolve the columns against the known tables in the plan.
    vValidateColumns(pRootNode, pPlanNode, pMetadataNode);

  } catch (SqlException& e) {
    delete pSeApi;
    throw;
  }

  delete pSeApi;
}

/*----------------------------------------------------------------------------*/
/* Validate and qualify all of the tableRefs in the graph                     */
/*----------------------------------------------------------------------------*/
void NameValidator::vValidateTables(PlanNode *pRootNode, QueryNode* pPlanNode, QueryNode* pMetadataNode, STORE::SeApi* pSeApi)
{
  ASSERT(pRootNode);
  ASSERT(pMetadataNode);
  ASSERT(pPlanNode);

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

    if (pTableInfoSet->nInfoRows() == 0) {
      // We couldn't resolve the table in the SE metadata
      delete pTableInfoSet;
      NameValidationException::vThrowUnknownTable(szCatalogName, szSchemaName, szTableName);
    }

    if (pTableInfoSet->nInfoRows() > 1) {
      // We've resolved more than one table in the metadata. The table name is ambiguous.
      NameValidationException::vThrowAmbiguousTable(szCatalogName, szSchemaName, szTableName, pTableInfoSet);
    }

    ASSERT(pTableInfoSet->nInfoRows() == 1);
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
      // TODO: Change this to look at the eCT enum
      QueryNode* pTypeNode = SqlType::pCreate(SqlType::eType(pColMeta->szColumnType()));

      // Make the column definition node and add it to the list of column definitions
      QueryNode* pColumnDef = ColumnDef::pCreate(pColMeta->szColumnName(),
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
  GraphNav::bWalk(pPlanNode, true, true, fVisitor, pRootNode);
}


/*----------------------------------------------------------------------------*/
/* Validate and qualify all of the columnRefs using the known tables          */
/*----------------------------------------------------------------------------*/
void NameValidator::vValidateColumns(PlanNode *pRootNode, QueryNode* pPlanNode, QueryNode* pMetadataNode)
{
  ASSERT(pRootNode);
  ASSERT(pMetadataNode);
  ASSERT(pPlanNode);

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
  GraphNav::bWalk(pPlanNode, true, true, fVisitor, pRootNode);
}

/*============================================================================*/

/*============================================================================*/



/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
