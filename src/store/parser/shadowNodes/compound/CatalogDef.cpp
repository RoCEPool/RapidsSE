/*============================================================================*/
/* CatalogDef.hpp - a definition of a catalogs used in the plan               */
/*============================================================================*/

#include "BASE.h"
#include "CatalogDef.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "PlanConstants.hpp"
#include "SchemaDefs.hpp"
#include "SchemaDef.hpp"

#include "GraphNav.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_CATALOGDEF_DEFN   = "/__root__/definitions/object/compound/catalogDef"_Stz;
static Stz* C_CATALOGDEF_NAME   = "catalogDef"_Stz;

static Stz* C_CATALOGNAME_PROP  = "catalogName"_Stz;
static Stz* C_CATALOGNAME_DEFN  = "/__root__/definitions/object/valueObject/stringObject/nonNullableStringObject"_Stz;

static Stz* C_SCHEMAS_PROP  = "schemas"_Stz;
static Stz* C_SCHEMAS_DEFN  = "/__root__/definitions/object/list/schemaDefs"_Stz;

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Helper function to create catalogDef nodes.                                 */
/*----------------------------------------------------------------------------*/
QueryNode* CatalogDef::pCreate(Stz* szCatalogName, QueryNode* pSchemaDefs)
{
  _static_

  ASSERT(szCatalogName);
  ASSERT(szCatalogName->nLength() > 0);
  ASSERT(pSchemaDefs);

  // Create the catalogDef object
  QueryNode* pCatalogDef = new QueryNode(szCatalogName, C_CATALOGDEF_DEFN);

  // Create the catalogName property
  Value xCatalogNameVal(szCatalogName);
  QueryNode* pCatalogName = new QueryNode(C_CATALOGNAME_PROP, C_CATALOGNAME_DEFN, static_cast<Value&&>(xCatalogNameVal));
  pCatalogDef->vAppendProperty(pCatalogName);

  // Create the schemas property
  pSchemaDefs->vSetName(C_SCHEMAS_PROP);
  pCatalogDef->vAppendProperty(pSchemaDefs);

  return pCatalogDef;
}

/*----------------------------------------------------------------------------*/
/* Appends a description of the catalog reference into a buffer               */
/*----------------------------------------------------------------------------*/
void CatalogDef::vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode)
{
  _static_

  ASSERT(bIsOfType(pNode));

  QueryNode* pCatalogDef = _pQueryNode(pNode);

  QueryNode* pCatalogName = _pQueryNode(GraphNav::pFindProperty(C_CATALOGNAME_PROP, pCatalogDef));

  ASSERT(pCatalogName);
  ASSERT(pCatalogName->szValue());

  sBuf.vCatSprint("CATALOG %s"_Stz, pCatalogName->szValue());
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the catalogDef definition    */
/*----------------------------------------------------------------------------*/
b1 CatalogDef::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_CATALOGDEF_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 CatalogDef::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_CATALOGDEF_DEFN);
}

/*----------------------------------------------------------------------------*/
/* get the catalog name from catalogDef                                       */
/*----------------------------------------------------------------------------*/
Stz* CatalogDef::szCatalogName(QueryNode* pNode)
{
  ASSERT(bIsOfType(pNode));

  QueryNode* pCatalogNameNode = _pQueryNode(GraphNav::pFindProperty(C_CATALOGNAME_PROP, pNode));
  ASSERT(pCatalogNameNode);

  Stz* szCatalogName = pCatalogNameNode->szValue();

  return szCatalogName;
}

/*----------------------------------------------------------------------------*/
/* Returns a QueryNode that represents a given SchemaDef in this catalog      */
/*----------------------------------------------------------------------------*/
QueryNode* CatalogDef::pSchemaDef(QueryNode* pNode, Stz* szSchemaName)
{
  ASSERT(bIsOfType(pNode));
  ASSERT(szSchemaName);

  QueryNode* pSchemaDef = nullptr;

  // Get the schemas property
  QueryNode* pSchemasNode = _pQueryNode(GraphNav::pFindProperty(C_SCHEMAS_PROP, pNode));

  if (!pSchemasNode) {
    return pSchemaDef;
  }

  PlanNodeIter* pIter = pSchemasNode->pNewChildrenIter();

  while (pIter->bHasNext()) {
    QueryNode* pChild = _pQueryNode(pIter->pNext());
    ASSERT(SchemaDef::bIsOfType(pChild));

    if (SchemaDef::szSchemaName(pChild)->bSame(szSchemaName)) {
      pSchemaDef = pChild;
      break;
    }
  }

  delete pIter;
  return pSchemaDef;
}


/*----------------------------------------------------------------------------*/
/* get an iterator of PlanNodes that represent SchemaDefs                     */
/*----------------------------------------------------------------------------*/
PlanNodeIter* CatalogDef::pSchemaDefIter(QueryNode *pNode)
{
  ASSERT(bIsOfType(pNode));

  QueryNode* pSchemasNode = _pQueryNode(GraphNav::pFindProperty(C_SCHEMAS_PROP, pNode));
  ASSERT(pSchemasNode);

  return pSchemasNode->pNewChildrenIter();
}

/*----------------------------------------------------------------------------*/
/* Add a schemaDef to this catalogDef                                         */
/*----------------------------------------------------------------------------*/
void CatalogDef::vAddSchema(QueryNode *pNode, QueryNode *pSchemaDefNode)
{
  ASSERT(bIsOfType(pNode));

  QueryNode* pSchemasNode = _pQueryNode(GraphNav::pFindProperty(C_SCHEMAS_PROP, pNode));
  ASSERT(pSchemasNode);

  pSchemasNode->vAppendChild(pSchemaDefNode);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
