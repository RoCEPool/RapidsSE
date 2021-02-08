/*============================================================================*/
/* CatalogDef.hpp - a definition of a catalogs used in the plan               */
/*============================================================================*/

#include "BASE.h"
#include "CatalogDef.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "PlanConstants.hpp"
#include "SchemaDefs.hpp"

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
  QueryNode* pCatalogDef = new QueryNode(C_CATALOGDEF_NAME, C_CATALOGDEF_DEFN);

  // Create the catalogName property
  Value xCatalogNameVal(szCatalogName);
  QueryNode* pCatalogName = new QueryNode(C_CATALOGNAME_PROP, C_CATALOGNAME_DEFN, static_cast<Value&&>(xCatalogNameVal));
  pCatalogDef->vAppendProperty(pCatalogName);

  // Create the schemas property
  pSchemaDefs->vRename(C_SCHEMAS_PROP);
  pCatalogDef->vAppendProperty(pSchemaDefs);

  return pCatalogDef;
}

/*----------------------------------------------------------------------------*/
/* Appends a description of the catalog reference into a buffer               */
/*----------------------------------------------------------------------------*/
void CatalogDef::vDescribe(PlanNode* pNode, s2K& sBuf)
{
  _static_

  ASSERT(bIsOfType(pNode));

  QueryNode* pCatalogDef = static_cast<QueryNode*>(pNode);

  QueryNode* pCatalogName = static_cast<QueryNode*>(GraphNav::pFindProperty(C_CATALOGNAME_PROP, pCatalogDef));
  QueryNode* pSchemas     = static_cast<QueryNode*>(GraphNav::pFindProperty(C_SCHEMAS_PROP, pCatalogDef));

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
/* get the catalog name from catalogDef                                        */
/*----------------------------------------------------------------------------*/
Stz* CatalogDef::szCatalogName(QueryNode* pNode)
{
  ASSERT(bIsOfType(pNode));

  QueryNode* pCatalogNameNode = static_cast<QueryNode*>(GraphNav::pFindProperty(C_CATALOGNAME_PROP, pNode));
  ASSERT(pCatalogNameNode);

  Stz* szCatalogName = pCatalogNameNode->szValue();

  return szCatalogName;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
