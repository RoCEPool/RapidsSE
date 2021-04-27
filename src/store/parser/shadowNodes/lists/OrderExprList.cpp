/*============================================================================*/
/* OrderExprList.hpp - a list of OrderExpr objects                            */
/*============================================================================*/

#include "BASE.h"
#include "OrderExprList.hpp"

#include "QueryNode.hpp"
#include "OrderExpr.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_ORDEREXPRLIST_DEFN = "/__root__/definitions/object/list/orderExprList"_Stz;
static Stz* C_ORDEREXPRLIST_NAME = "OrderExprList"_Stz;



/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Helper function to create a list of catalogDef nodes.                       */
/*----------------------------------------------------------------------------*/
QueryNode* OrderExprList::pCreate()
{
  _static_

  // Create an empty OrderExprList object
  QueryNode* pOrderExprList = new QueryNode(C_ORDEREXPRLIST_NAME, const_cast<Stz*>(C_ORDEREXPRLIST_DEFN));

  return pOrderExprList;
}

/*----------------------------------------------------------------------------*/
/* Appends a description of the order expression list into a buffer           */
/*----------------------------------------------------------------------------*/
void OrderExprList::vDescribe(PlanNode* pNode, s2K& sBuf)
{
  _static_

  ASSERT(bIsOfType(pNode));

  QueryNode* pOrderExprList = _pQueryNode(pNode);

  PlanNodeIter* pIter = pOrderExprList->pNewChildrenIter();
  b1 bFirst = true;

  while (pIter->bHasNext()) {
    PlanNode* pChild = pIter->pNext();
    ASSERT(OrderExpr::bIsOfType(pChild));

    if (bFirst) {
      bFirst = false;
    } else {
      sBuf.vConcat(", "_Stz);
    }

    OrderExpr::vDescribe(pChild, sBuf);
  }

  delete pIter;
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the OrderExprList definition*/
/*----------------------------------------------------------------------------*/
b1 OrderExprList::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_ORDEREXPRLIST_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 OrderExprList::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_ORDEREXPRLIST_DEFN);
}

/*----------------------------------------------------------------------------*/
/* get the number of expressions in this list                                 */
/*----------------------------------------------------------------------------*/
i8 OrderExprList::nExprs(QueryNode* pOrderExprList)
{
  ASSERT(bIsOfType(pOrderExprList));

  return pOrderExprList->nChildren();
}

/*----------------------------------------------------------------------------*/
/* An iterator of OrderExpr objects                                           */
/*----------------------------------------------------------------------------*/
PlanNodeIter* OrderExprList::pOrderExprIter(QueryNode* pNode)
{
  ASSERT(pNode);
  ASSERT(bIsOfType(pNode));
  ASSERT(pNode->nChildren() > 0);

  return pNode->pNewChildrenIter();
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
