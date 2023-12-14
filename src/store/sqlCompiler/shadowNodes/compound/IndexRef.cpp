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
static Stz* C_DEFN = "/__root__/definitions/object/compound/tableRef/indexRef"_Stz;
static Stz* C_NAME = "indexRef"_Stz;
static Stz* C_INDEX_PROP = "indexName"_Stz;


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
QueryNode* IndexRef::pCreate(Stz* szCatalogName, Stz* szSchemaName, Stz* szTableName, Stz* szIndexName)
{
  return IndexRef::pCreate( new Text(szCatalogName),
                            new Text(szSchemaName),
                            new Text(szTableName),
                            new Text(szIndexName) );
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

  // Create the IndexRef object, a subclass of TableRef
  QueryNode* pIndexRef = TableRef::pCreate(C_NAME, C_DEFN, pCatalogName, pSchemaName, pTableName);

  Value xIndexValue(pIndexName);
  QueryNode* pIndex = new QueryNode(C_INDEX_PROP,    const_cast<Stz*>(C_STRING_OBJ_DEFN), static_cast<Value&&>(xIndexValue));
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

  QueryNode* pIndexNode   = _pQueryNode(GraphNav::pFindProperty(C_INDEX_PROP,   pIndexRef));
  ASSERT(pIndexNode);

  Stz* szCatalogName = szCatalog(pIndexRef);
  Stz* szSchemaName  = szSchema(pIndexRef);
  Stz* szTableName   = szTable(pIndexRef);
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

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
