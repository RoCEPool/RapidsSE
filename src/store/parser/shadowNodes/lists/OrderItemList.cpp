/*============================================================================*/
/* OrderItemList.hpp - a list of OrderItem objects                            */
/*============================================================================*/

#include "BASE.h"
#include "OrderItemList.hpp"

#include "QueryNode.hpp"
#include "OrderItem.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_ORDERITEMLIST_DEFN = "/__root__/definitions/object/list/orderItemList"_Stz;
static Stz* C_ORDERITEMLIST_NAME = "OrderItemList"_Stz;



/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Helper function to create a list of catalogDef nodes.                       */
/*----------------------------------------------------------------------------*/
QueryNode* OrderItemList::pCreate()
{
  _static_

  // Create an empty OrderItemList object
  QueryNode* pOrderItemList = new QueryNode(C_ORDERITEMLIST_NAME, const_cast<Stz*>(C_ORDERITEMLIST_DEFN));

  return pOrderItemList;
}

/*----------------------------------------------------------------------------*/
/* Appends a description of the order expression list into a buffer           */
/*----------------------------------------------------------------------------*/
void OrderItemList::vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode)
{
  _static_

  ASSERT(bIsOfType(pNode));

  QueryNode* pOrderItemList = _pQueryNode(pNode);

  PlanNodeIter* pIter = pOrderItemList->pNewChildrenIter();
  b1 bFirst = true;

  while (pIter->bHasNext()) {
    PlanNode* pChild = pIter->pNext();
    ASSERT(OrderItem::bIsOfType(pChild));

    if (bFirst) {
      bFirst = false;
    } else {
      sBuf.vConcat(", "_Stz);
    }

    OrderItem::vDescribe(pChild, sBuf, pRootNode);
  }

  delete pIter;
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the OrderItemList definition*/
/*----------------------------------------------------------------------------*/
b1 OrderItemList::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_ORDERITEMLIST_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 OrderItemList::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_ORDERITEMLIST_DEFN);
}

/*----------------------------------------------------------------------------*/
/* get the number of expressions in this list                                 */
/*----------------------------------------------------------------------------*/
i8 OrderItemList::nExprs(QueryNode* pOrderItemList)
{
  ASSERT(bIsOfType(pOrderItemList));

  return pOrderItemList->nChildren();
}

/*----------------------------------------------------------------------------*/
/* An iterator of OrderItem objects                                           */
/*----------------------------------------------------------------------------*/
PlanNodeIter* OrderItemList::pOrderItemIter(QueryNode* pNode)
{
  ASSERT(pNode);
  ASSERT(bIsOfType(pNode));
  ASSERT(pNode->nChildren() > 0);

  return pNode->pNewChildrenIter();
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
