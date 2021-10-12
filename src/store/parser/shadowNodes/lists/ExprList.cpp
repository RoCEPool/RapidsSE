/*============================================================================*/
/* ExprList.hpp - a list of expression nodes                                  */
/*============================================================================*/

#include "BASE.h"
#include "ExprList.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "Expr.hpp"

#include "GraphNav.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_DEFN = "/__root__/definitions/object/list/exprList"_Stz;



/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Helper function to create a list of stringobject nodes.                    */
/*----------------------------------------------------------------------------*/
QueryNode* ExprList::pCreate(Stz* szName)
{
  _static_

  // Create an empty exprList object
  QueryNode* pExprList = new QueryNode(szName, const_cast<Stz*>(C_DEFN));

  return pExprList;
}

/*----------------------------------------------------------------------------*/
/* Appends a description of the string list into a buffer                     */
/*----------------------------------------------------------------------------*/
void ExprList::vDescribe(PlanNode* pNode, s2K& sBuf)
{
  _static_

  ASSERT(bIsOfType(pNode));

  QueryNode* pExprList = _pQueryNode(pNode);

  PlanNodeIter* pIter = pExprList->pNewChildrenIter();
  b1 bFirst = true;

  while (pIter->bHasNext()) {
    PlanNode* pChild = pIter->pNext();

    if (bFirst) {
      bFirst = false;
    } else {
      sBuf.vConcat(", "_Stz);
    }

    s2K sExprBuf;
    Expr::vDescribe(pChild, sExprBuf);

    sBuf.vConcat(sExprBuf.szStz());
  }

  delete pIter;
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the exprList definition   */
/*----------------------------------------------------------------------------*/
b1 ExprList::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 ExprList::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_DEFN);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
