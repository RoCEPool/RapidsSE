/*============================================================================*/
/* IndexRef.hpp - a reference to a named index                                */
/*============================================================================*/

#include "BASE.h"
#include "IndexRef.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "PlanConstants.hpp"

#include "GraphNav.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_DEFN = "/__root__/definitions/object/compound/indexRef"_Stz;
static Stz* C_NAME = "indexRef"_Stz;
static Stz* C_CATALOG_PROP  = "catalogName"_Stz;
static Stz* C_SCHEMA_PROP   = "schemaName"_Stz;
static Stz* C_TABLE_PROP    = "tableName"_Stz;
static Stz* C_INDEX_PROP    = "indexName"_Stz;


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Helper function to create IndexRef nodes.                                  */
/*----------------------------------------------------------------------------*/
QueryNode* IndexRef::pCreate(Text* pIndexName)
{
  _static_

  return pCreate(nullptr, nullptr, nullptr, pIndexName);
}

/*----------------------------------------------------------------------------*/
/* Helper function to create IndexRef nodes.                                  */
/*----------------------------------------------------------------------------*/
QueryNode* IndexRef::pCreate(Text* pCatalogName, Text* pSchemaName, Text* pTableName, Text* pIndexName)
{
  _static_

  ASSERT(pIndexName);
  ASSERT(pIndexName->nLength() > 0);
  ASSERT(!pTableName   || pTableName->nLength() > 0);
  ASSERT(!pSchemaName  || pSchemaName->nLength() > 0);
  ASSERT(!pCatalogName || pCatalogName->nLength() > 0);

  Value xCatalogValue(pCatalogName);
  Value xSchemaValue(pSchemaName);
  Value xTableValue(pTableName);
  Value xIndexValue(pIndexName);

  // Create the IndexRef object
  QueryNode* pIndexRef = new QueryNode(C_NAME, const_cast<Stz*>(C_DEFN));
  QueryNode* pCatalog  = new QueryNode(C_CATALOG_PROP,  const_cast<Stz*>(C_STRING_OBJ_DEFN), static_cast<Value&&>(xCatalogValue));
  QueryNode* pSchema   = new QueryNode(C_SCHEMA_PROP,   const_cast<Stz*>(C_STRING_OBJ_DEFN), static_cast<Value&&>(xSchemaValue));
  QueryNode* pTable    = new QueryNode(C_TABLE_PROP,    const_cast<Stz*>(C_STRING_OBJ_DEFN), static_cast<Value&&>(xTableValue));
  QueryNode* pIndex    = new QueryNode(C_INDEX_PROP,    const_cast<Stz*>(C_STRING_OBJ_DEFN), static_cast<Value&&>(xIndexValue));

  pIndexRef->vAppendProperty(pCatalog);
  pIndexRef->vAppendProperty(pSchema);
  pIndexRef->vAppendProperty(pTable);
  pIndexRef->vAppendProperty(pIndex);

  return pIndexRef;
}

/*----------------------------------------------------------------------------*/
/* Appends a description of the table reference into a buffer                 */
/*----------------------------------------------------------------------------*/
void IndexRef::vDescribe(PlanNode* pNode, s2K& buf, PlanNode* pRootNode)
{
  _static_

  ASSERT(bIsOfType(pNode));
  UNUSED(pRootNode);

  QueryNode* pIndexRef = _pQueryNode(pNode);

  QueryNode* pCatalogNode = _pQueryNode(GraphNav::pFindProperty(C_CATALOG_PROP, pIndexRef));
  QueryNode* pSchemaNode  = _pQueryNode(GraphNav::pFindProperty(C_SCHEMA_PROP,  pIndexRef));
  QueryNode* pTableNode   = _pQueryNode(GraphNav::pFindProperty(C_TABLE_PROP,   pIndexRef));
  QueryNode* pIndexNode   = _pQueryNode(GraphNav::pFindProperty(C_INDEX_PROP,   pIndexRef));

  ASSERT(pCatalogNode);
  ASSERT(pSchemaNode);
  ASSERT(pTableNode);
  ASSERT(pIndexNode);

  Stz* szCatalogName = pCatalogNode->szValue();
  Stz* szSchemaName  = pSchemaNode->szValue();
  Stz* szTableName   = pTableNode->szValue();
  Stz* szIndexName   = pIndexNode->szValue();

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
    buf.vAppend('.');
  }

  ASSERT(szIndexName);
  buf.vConcat(szIndexName);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the IndexRef definition     */
/*----------------------------------------------------------------------------*/
b1 IndexRef::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 IndexRef::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_DEFN);
}

/*----------------------------------------------------------------------------*/
/* get catalog name from IndexRef                                             */
/*----------------------------------------------------------------------------*/
Stz* IndexRef::szCatalog(QueryNode* pIndexRef)
{
  ASSERT(bIsOfType(pIndexRef));

  QueryNode* pCatalogNode = _pQueryNode(GraphNav::pFindProperty(C_CATALOG_PROP, pIndexRef));
  ASSERT(pCatalogNode);

  Stz* szCatalogName = pCatalogNode->szValue();

  return szCatalogName;
}

/*----------------------------------------------------------------------------*/
/* get schema name from IndexRef                                             */
/*----------------------------------------------------------------------------*/
Stz* IndexRef::szSchema(QueryNode* pIndexRef)
{
  ASSERT(bIsOfType(pIndexRef));

  QueryNode* pSchemaNode = _pQueryNode(GraphNav::pFindProperty(C_SCHEMA_PROP, pIndexRef));
  ASSERT(pSchemaNode);

  Stz* szSchemaName = pSchemaNode->szValue();

  return szSchemaName;
}

/*----------------------------------------------------------------------------*/
/* get table name from IndexRef                                               */
/*----------------------------------------------------------------------------*/
Stz* IndexRef::szTable(QueryNode* pIndexRef)
{
  ASSERT(bIsOfType(pIndexRef));

  QueryNode* pTableNode = _pQueryNode(GraphNav::pFindProperty(C_TABLE_PROP, pIndexRef));
  ASSERT(pTableNode);

  Stz* szTableName = pTableNode->szValue();

  return szTableName;
}

/*----------------------------------------------------------------------------*/
/* get index name from IndexRef                                               */
/*----------------------------------------------------------------------------*/
Stz* IndexRef::szIndex(QueryNode* pIndexRef)
{
  ASSERT(bIsOfType(pIndexRef));

  QueryNode* pIndexNode = _pQueryNode(GraphNav::pFindProperty(C_INDEX_PROP, pIndexRef));
  ASSERT(pIndexNode);

  Stz* szIndexName = pIndexNode->szValue();

  return szIndexName;
}

/*----------------------------------------------------------------------------*/
/* A IndexRef is fully qualified iff it has a catalog, schema and tablename   */
/*----------------------------------------------------------------------------*/
b1 IndexRef::bIsFullyQualified(QueryNode* pIndexRef)
{
  ASSERT(bIsOfType(pIndexRef));

  QueryNode* pCatalogNode = _pQueryNode(GraphNav::pFindProperty(C_CATALOG_PROP, pIndexRef));
  QueryNode* pSchemaNode  = _pQueryNode(GraphNav::pFindProperty(C_SCHEMA_PROP, pIndexRef));
  QueryNode* pTableNode   = _pQueryNode(GraphNav::pFindProperty(C_TABLE_PROP, pIndexRef));
  QueryNode* pIndexNode   = _pQueryNode(GraphNav::pFindProperty(C_INDEX_PROP, pIndexRef));

  ASSERT(pCatalogNode);
  ASSERT(pSchemaNode);
  ASSERT(pTableNode);
  ASSERT(pIndexNode);

  Stz* szCatalogName = pCatalogNode->szValue();
  Stz* szSchemaName  = pSchemaNode->szValue();
  Stz* szTableName   = pTableNode->szValue();
  Stz* szIndexName   = pIndexNode->szValue();

  return (szCatalogName && szSchemaName && szTableName && szIndexName);
}

/*----------------------------------------------------------------------------*/
/* Update the IndexRef so that it is fully qualified                          */
/*----------------------------------------------------------------------------*/
void IndexRef::vQualify(QueryNode* pIndexRef, Stz* szCatalogName, Stz* szSchemaName,
                        Stz* szTableName, Stz* szIndexName)
{
  ASSERT(bIsOfType(pIndexRef));
  ASSERT(szCatalogName);
  ASSERT(szSchemaName);
  ASSERT(szTableName);
  ASSERT(szIndexName);

  QueryNode* pCatalogNode = _pQueryNode(GraphNav::pFindProperty(C_CATALOG_PROP, pIndexRef));
  QueryNode* pSchemaNode  = _pQueryNode(GraphNav::pFindProperty(C_SCHEMA_PROP, pIndexRef));
  QueryNode* pTableNode   = _pQueryNode(GraphNav::pFindProperty(C_TABLE_PROP, pIndexRef));
  QueryNode* pIndexNode   = _pQueryNode(GraphNav::pFindProperty(C_INDEX_PROP, pIndexRef));

  ASSERT(pCatalogNode);
  ASSERT(pSchemaNode);
  ASSERT(pTableNode);
  ASSERT(pIndexNode);

  Value xCatValue;
  xCatValue.vSet(szCatalogName);
  pCatalogNode->vSetValue(static_cast<Value&&>(xCatValue));

  Value xSchValue;
  xSchValue.vSet(szSchemaName);
  pSchemaNode->vSetValue(static_cast<Value&&>(xSchValue));

  Value xTblValue;
  xTblValue.vSet(szTableName);
  pTableNode->vSetValue(static_cast<Value&&>(xTblValue));

  Value xIdxValue;
  xIdxValue.vSet(szIndexName);
  pIndexNode->vSetValue(static_cast<Value&&>(xIdxValue));

  ASSERT(bIsFullyQualified(pIndexRef));
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
