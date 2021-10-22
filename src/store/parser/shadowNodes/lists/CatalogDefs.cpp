/*============================================================================*/
/* CatalogDefs.hpp - a list of catalogs definitions                           */
/*============================================================================*/

#include "BASE.h"
#include "CatalogDefs.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "PlanConstants.hpp"
#include "CatalogDef.hpp"

#include "GraphNav.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_CATALOGDEFS_DEFN = "/__root__/definitions/object/list/catalogDefs"_Stz;
static Stz* C_CATALOGDEFS_NAME = "catalogDefs"_Stz;



/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Helper function to create a list of catalogDef nodes.                       */
/*----------------------------------------------------------------------------*/
QueryNode* CatalogDefs::pCreate()
{
  _static_

  // Create an empty catalogDefs object
  QueryNode* pCatalogDefs = new QueryNode(C_CATALOGDEFS_NAME, const_cast<Stz*>(C_CATALOGDEFS_DEFN));

  return pCatalogDefs;
}

/*----------------------------------------------------------------------------*/
/* Add a catalogDef to the list                                                */
/*----------------------------------------------------------------------------*/
void CatalogDefs::vAddCatalogDef(QueryNode* pNode, QueryNode* pCatalogDef)
{
  _static_
  ASSERT(bIsOfType(pNode));
  ASSERT(CatalogDef::bIsOfType(pCatalogDef));

  pNode->vAppendChild(pCatalogDef);
}

/*----------------------------------------------------------------------------*/
/* Appends a description of the catalog definitions into a buffer              */
/*----------------------------------------------------------------------------*/
void CatalogDefs::vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode)
{
  _static_

  ASSERT(bIsOfType(pNode));
  UNUSED(pRootNode);

  QueryNode* pCatalogDefs = _pQueryNode(pNode);

  PlanNodeIter* pIter = pCatalogDefs->pNewChildrenIter();
  b1 bFirst = true;

  while (pIter->bHasNext()) {
    PlanNode* pChild = pIter->pNext();

    if (bFirst) {
      bFirst = false;
    } else {
      sBuf.vConcat(", "_Stz);
    }

    sBuf.vConcat(CatalogDef::szCatalogName(_pQueryNode(pChild)));
  }

  delete pIter;
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the catalogDefs definition   */
/*----------------------------------------------------------------------------*/
b1 CatalogDefs::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_CATALOGDEFS_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 CatalogDefs::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_CATALOGDEFS_DEFN);
}

/*----------------------------------------------------------------------------*/
/* get the number of catalogs in this list                                     */
/*----------------------------------------------------------------------------*/
i8 CatalogDefs::nCatalogs(QueryNode* pCatalogDefs)
{
  ASSERT(bIsOfType(pCatalogDefs));

  return pCatalogDefs->nChildren();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
