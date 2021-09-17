/*============================================================================*/
/* CteDefs.hpp - a list of CTE definitions                                    */
/*============================================================================*/

#include "BASE.h"
#include "CteDefs.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "PlanConstants.hpp"
#include "CteDef.hpp"

#include "GraphNav.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_CTEDEFS_DEFN = "/__root__/definitions/object/list/cteDefs"_Stz;
static Stz* C_CTEDEFS_NAME = "cteDefs"_Stz;



/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Helper function to create a list of cteDef nodes.                          */
/*----------------------------------------------------------------------------*/
QueryNode* CteDefs::pCreate()
{
  _static_

  // Create an empty CteDefs object
  QueryNode* pCteDefs = new QueryNode(C_CTEDEFS_NAME, const_cast<Stz*>(C_CTEDEFS_DEFN));

  return pCteDefs;
}

/*----------------------------------------------------------------------------*/
/* Add a cteDef to the list                                                   */
/*----------------------------------------------------------------------------*/
void CteDefs::vAddCteDef(QueryNode* pNode, QueryNode* pCteDef)
{
  _static_
  ASSERT(bIsOfType(pNode));
  ASSERT(CteDef::bIsOfType(pCteDef));

  pNode->vAppendChild(pCteDef);
}

/*----------------------------------------------------------------------------*/
/* Appends a description of the CTE definitions into a buffer                 */
/*----------------------------------------------------------------------------*/
void CteDefs::vDescribe(PlanNode* pNode, s2K& sBuf)
{
  _static_

  ASSERT(bIsOfType(pNode));

  QueryNode* pCteDefs = _pQueryNode(pNode);

  PlanNodeIter* pIter = pCteDefs->pNewChildrenIter();
  b1 bFirst = true;

  while (pIter->bHasNext()) {
    PlanNode* pChild = pIter->pNext();

    if (bFirst) {
      bFirst = false;
    } else {
      sBuf.vConcat(", "_Stz);
    }

    sBuf.vConcat(CteDef::szAliasName(_pQueryNode(pChild)));
  }

  delete pIter;
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the CteDefs definition      */
/*----------------------------------------------------------------------------*/
b1 CteDefs::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_CTEDEFS_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 CteDefs::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_CTEDEFS_DEFN);
}

/*----------------------------------------------------------------------------*/
/* get the number of CteDefs in this list                                     */
/*----------------------------------------------------------------------------*/
i8 CteDefs::nItems(QueryNode* pCteDefs)
{
  ASSERT(bIsOfType(pCteDefs));

  return pCteDefs->nChildren();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
