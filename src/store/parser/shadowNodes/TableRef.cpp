/*============================================================================*/
/* TableRef.hpp - a reference to a named table                                */
/*============================================================================*/

#include "BASE.h"
#include "STORE.h"

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_TABLEREF_DEFN = "/__root__/definitions/object/tableRef"_Stz;
static Stz* C_TABLEREF_NAME = "tableRef"_Stz;
static Stz* C_CATALOG_PROP  = "catalogName"_Stz;
static Stz* C_SCHEMA_PROP   = "schemaName"_Stz;
static Stz* C_TABLE_PROP    = "tableName"_Stz;


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Helper function to create tableRef nodes.                                  */
/*----------------------------------------------------------------------------*/
QueryNode* TableRef::pCreate(Text* pCatalogName, Text* pSchemaName, Text* pTableName)
{
  _static_

  ASSERT(pTableName);
  ASSERT(pTableName->nLength() > 0);
  ASSERT(!pSchemaName || pSchemaName->nLength() > 0);
  ASSERT(!pCatalogName || pCatalogName->nLength() > 0);

  Value xCatalogValue(pCatalogName);
  Value xSchemaValue(pSchemaName);
  Value xTableValue(pTableName);

  // Create the tableRef object
  QueryNode* pTableRef = new QueryNode(C_TABLEREF_NAME, const_cast<Stz*>(C_TABLEREF_DEFN));
  QueryNode* pCatalog  = new QueryNode(C_CATALOG_PROP,  const_cast<Stz*>(C_STRING_OBJ_DEFN), xCatalogValue);
  QueryNode* pSchema   = new QueryNode(C_SCHEMA_PROP,   const_cast<Stz*>(C_STRING_OBJ_DEFN), xSchemaValue);
  QueryNode* pTable    = new QueryNode(C_TABLE_PROP,    const_cast<Stz*>(C_STRING_OBJ_DEFN), xTableValue);

  pTableRef->vAppendProperty(pCatalog);
  pTableRef->vAppendProperty(pSchema);
  pTableRef->vAppendProperty(pTable);

  return pTableRef;
}

/*----------------------------------------------------------------------------*/
/* Appends a description of the table reference into a buffer                 */
/*----------------------------------------------------------------------------*/
void TableRef::vDescribe(PlanNode* pNode, s2K& buf)
{
  _static_

  ASSERT(bValidate(pNode));

  QueryNode* pTableRef = static_cast<QueryNode*>(pNode);

  QueryNode* pCatalogNode = static_cast<QueryNode*>(GraphNav::pFindProperty(C_CATALOG_PROP, pTableRef));
  QueryNode* pSchemaNode  = static_cast<QueryNode*>(GraphNav::pFindProperty(C_SCHEMA_PROP, pTableRef));
  QueryNode* pTableNode   = static_cast<QueryNode*>(GraphNav::pFindProperty(C_TABLE_PROP, pTableRef));

  ASSERT(pCatalogNode);
  ASSERT(pSchemaNode);
  ASSERT(pTableNode);

  Stz* szCatalogName = pCatalogNode->getStringValue();
  Stz* szSchemaName  = pSchemaNode->getStringValue();
  Stz* szTableName   = pTableNode->getStringValue();

  if (szCatalogName) {
    buf.vConcat(szCatalogName);
    buf.vAppend('.');
    ASSERT(szSchemaName);
  }

  if (szSchemaName) {
    buf.vConcat(szSchemaName);
    buf.vAppend('.');
    ASSERT(szTableName);
  }

  if (szTableName) {
    buf.vConcat(szTableName);
  }
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the tableRef definition     */
/*----------------------------------------------------------------------------*/
b1 TableRef::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_TABLEREF_DEFN);
}

/*----------------------------------------------------------------------------*/
/* A tableRef is resolved iff it has a catalog, schema and tablename          */
/*----------------------------------------------------------------------------*/
b1 TableRef::bIsResolved(QueryNode* pTableRef)
{
  ASSERT(bValidate(pTableRef));

  QueryNode* pCatalogNode = static_cast<QueryNode*>(GraphNav::pFindProperty(C_CATALOG_PROP, pTableRef));
  QueryNode* pSchemaNode  = static_cast<QueryNode*>(GraphNav::pFindProperty(C_SCHEMA_PROP, pTableRef));
  QueryNode* pTableNode   = static_cast<QueryNode*>(GraphNav::pFindProperty(C_TABLE_PROP, pTableRef));

  ASSERT(pCatalogNode);
  ASSERT(pSchemaNode);
  ASSERT(pTableNode);

  Stz* szCatalogName = pCatalogNode->getStringValue();
  Stz* szSchemaName  = pSchemaNode->getStringValue();
  Stz* szTableName   = pTableNode->getStringValue();

  return (szCatalogName && szSchemaName && szTableName);
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
