/*============================================================================*/
/* TableRef.hpp - a reference to a named table                                */
/*============================================================================*/

#include "BASE.h"
#include "TableRef.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "PlanConstants.hpp"

#include "GraphNav.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_TABLEREF_DEFN = "/__root__/definitions/object/compound/tableRef"_Stz;
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
  QueryNode* pCatalog  = new QueryNode(C_CATALOG_PROP,  const_cast<Stz*>(C_STRING_OBJ_DEFN), static_cast<Value&&>(xCatalogValue));
  QueryNode* pSchema   = new QueryNode(C_SCHEMA_PROP,   const_cast<Stz*>(C_STRING_OBJ_DEFN), static_cast<Value&&>(xSchemaValue));
  QueryNode* pTable    = new QueryNode(C_TABLE_PROP,    const_cast<Stz*>(C_STRING_OBJ_DEFN), static_cast<Value&&>(xTableValue));

  pTableRef->vAppendProperty(pCatalog);
  pTableRef->vAppendProperty(pSchema);
  pTableRef->vAppendProperty(pTable);

  return pTableRef;
}

/*----------------------------------------------------------------------------*/
/* Helper function to create tableRef nodes for subclasses.                   */
/*----------------------------------------------------------------------------*/
QueryNode* TableRef::pCreate(Stz* szName, Stz* szType, Text* pCatalogName, Text* pSchemaName, Text* pTableName)
{
  _static_

  ASSERT(szName);
  ASSERT(szType);
  ASSERT(!pTableName || pTableName->nLength() > 0);
  ASSERT(!pSchemaName || pSchemaName->nLength() > 0);
  ASSERT(!pCatalogName || pCatalogName->nLength() > 0);

  Value xCatalogValue(pCatalogName);
  Value xSchemaValue(pSchemaName);
  Value xTableValue(pTableName);

  // Create the tableRef object
  QueryNode* pTableRef = new QueryNode(szName, szType);
  QueryNode* pCatalog  = new QueryNode(C_CATALOG_PROP,  const_cast<Stz*>(C_STRING_OBJ_DEFN), static_cast<Value&&>(xCatalogValue));
  QueryNode* pSchema   = new QueryNode(C_SCHEMA_PROP,   const_cast<Stz*>(C_STRING_OBJ_DEFN), static_cast<Value&&>(xSchemaValue));
  QueryNode* pTable    = new QueryNode(C_TABLE_PROP,    const_cast<Stz*>(C_STRING_OBJ_DEFN), static_cast<Value&&>(xTableValue));

  pTableRef->vAppendProperty(pCatalog);
  pTableRef->vAppendProperty(pSchema);
  pTableRef->vAppendProperty(pTable);

  return pTableRef;
}

/*----------------------------------------------------------------------------*/
/* Appends a description of the table reference into a buffer                 */
/*----------------------------------------------------------------------------*/
void TableRef::vDescribe(PlanNode* pNode, s2K& buf, PlanNode* pRootNode)
{
  _static_

  ASSERT(bIsOfType(pNode));
  UNUSED(pRootNode);

  QueryNode* pTableRef = _pQueryNode(pNode);

  QueryNode* pCatalogNode = _pQueryNode(GraphNav::pFindProperty(C_CATALOG_PROP, pTableRef));
  QueryNode* pSchemaNode  = _pQueryNode(GraphNav::pFindProperty(C_SCHEMA_PROP, pTableRef));
  QueryNode* pTableNode   = _pQueryNode(GraphNav::pFindProperty(C_TABLE_PROP, pTableRef));

  ASSERT(pCatalogNode);
  ASSERT(pSchemaNode);
  ASSERT(pTableNode);

  Stz* szCatalogName = pCatalogNode->szValue();
  Stz* szSchemaName  = pSchemaNode->szValue();
  Stz* szTableName   = pTableNode->szValue();

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
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 TableRef::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_TABLEREF_DEFN);
}

/*----------------------------------------------------------------------------*/
/* get catalog name from tableRef                                             */
/*----------------------------------------------------------------------------*/
Stz* TableRef::szCatalog(QueryNode* pTableRef)
{
  ASSERT(bIsOfType(pTableRef));

  QueryNode* pCatalogNode = _pQueryNode(GraphNav::pFindProperty(C_CATALOG_PROP, pTableRef));
  ASSERT(pCatalogNode);

  Stz* szCatalogName = pCatalogNode->szValue();

  return szCatalogName;
}

/*----------------------------------------------------------------------------*/
/* get schema name from tableRef                                             */
/*----------------------------------------------------------------------------*/
Stz* TableRef::szSchema(QueryNode* pTableRef)
{
  ASSERT(bIsOfType(pTableRef));

  QueryNode* pSchemaNode = _pQueryNode(GraphNav::pFindProperty(C_SCHEMA_PROP, pTableRef));
  ASSERT(pSchemaNode);

  Stz* szSchemaName = pSchemaNode->szValue();

  return szSchemaName;
}

/*----------------------------------------------------------------------------*/
/* get table name from tableRef                                               */
/*----------------------------------------------------------------------------*/
Stz* TableRef::szTable(QueryNode* pTableRef)
{
  ASSERT(bIsOfType(pTableRef));

  QueryNode* pTableNode = _pQueryNode(GraphNav::pFindProperty(C_TABLE_PROP, pTableRef));
  ASSERT(pTableNode);

  Stz* szTableName = pTableNode->szValue();

  return szTableName;
}

/*----------------------------------------------------------------------------*/
/* A tableRef is fully qualified iff it has a catalog, schema and tablename   */
/*----------------------------------------------------------------------------*/
b1 TableRef::bIsFullyQualified(QueryNode* pTableRef)
{
  ASSERT(bIsOfType(pTableRef));

  QueryNode* pCatalogNode = _pQueryNode(GraphNav::pFindProperty(C_CATALOG_PROP, pTableRef));
  QueryNode* pSchemaNode  = _pQueryNode(GraphNav::pFindProperty(C_SCHEMA_PROP, pTableRef));
  QueryNode* pTableNode   = _pQueryNode(GraphNav::pFindProperty(C_TABLE_PROP, pTableRef));

  ASSERT(pCatalogNode);
  ASSERT(pSchemaNode);
  ASSERT(pTableNode);

  Stz* szCatalogName = pCatalogNode->szValue();
  Stz* szSchemaName  = pSchemaNode->szValue();
  Stz* szTableName   = pTableNode->szValue();

  return (szCatalogName && szSchemaName && szTableName);
}

/*----------------------------------------------------------------------------*/
/* Update the TableRef so that it is fully qualified                          */
/*----------------------------------------------------------------------------*/
void TableRef::vQualify(QueryNode* pTableRef, Stz* szCatalogName, Stz* szSchemaName, Stz* szTableName)
{
  ASSERT(bIsOfType(pTableRef));
  ASSERT(szCatalogName);
  ASSERT(szSchemaName);
  ASSERT(szTableName);

  QueryNode* pCatalogNode = _pQueryNode(GraphNav::pFindProperty(C_CATALOG_PROP, pTableRef));
  QueryNode* pSchemaNode  = _pQueryNode(GraphNav::pFindProperty(C_SCHEMA_PROP, pTableRef));
  QueryNode* pTableNode   = _pQueryNode(GraphNav::pFindProperty(C_TABLE_PROP, pTableRef));

  ASSERT(pCatalogNode);
  ASSERT(pSchemaNode);
  ASSERT(pTableNode);

  Value xCatValue;
  xCatValue.vSet(szCatalogName);
  pCatalogNode->vSetValue(static_cast<Value&&>(xCatValue));

  Value xSchValue;
  xSchValue.vSet(szSchemaName);
  pSchemaNode->vSetValue(static_cast<Value&&>(xSchValue));

  Value xTblValue;
  xTblValue.vSet(szTableName);
  pTableNode->vSetValue(static_cast<Value&&>(xTblValue));

  ASSERT(bIsFullyQualified(pTableRef));
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
