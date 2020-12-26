/*============================================================================*/
/* QueryNode.cpp - An mutable node that holds the plan of the user's query    */
/*============================================================================*/

#include "StorageEngine.h"
#include "QueryNode.hpp"



/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__pCastFunction(Parser,QueryNode,CN_QUERYNODE)
__aNewFunction(QueryNode,CN_QUERYNODE)
__vDeleteFunction(QueryNode,CN_QUERYNODE)
__aFooterFunction(QueryNode)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
QueryNode::QueryNode(Stz* szName, Stz* szType)
    : MutablePlanNode(CN_QUERYNODE, szName, szType, {},
                      new PlanNodeList(), new PlanNodeList())
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUERYNODE,_FOOTER_);

  _checkInitialised_(CN_QUERYNODE);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_QUERYNODE,_FOOTER_);

}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
QueryNode::QueryNode(Stz* szName, Stz* szType, Value xValue)
    : MutablePlanNode(CN_QUERYNODE, szName, szType, xValue,
                      new PlanNodeList(), new PlanNodeList())
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUERYNODE,_FOOTER_);

  _checkInitialised_(CN_QUERYNODE);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_QUERYNODE,_FOOTER_);

}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
QueryNode::QueryNode(Stz* szName, Stz* szType, Value xValue,
                               PlanNodeList* pProperties, PlanNodeList* pChildren)
    : MutablePlanNode(CN_QUERYNODE, szName, szType, xValue, pProperties, pChildren)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUERYNODE,_FOOTER_);

  _checkInitialised_(CN_QUERYNODE);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_QUERYNODE,_FOOTER_);

}

/*----------------------------------------------------------------------------*/
/* Copy constructor                                                           */
/*----------------------------------------------------------------------------*/
QueryNode::QueryNode(const QueryNode& oNode)
    : MutablePlanNode(CN_QUERYNODE, oNode)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUERYNODE,_FOOTER_);

  _checkInitialised_(CN_QUERYNODE);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_QUERYNODE,_FOOTER_);

}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
QueryNode::~QueryNode(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUERYNODE,_FOOTER_);

}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/



/*----------------------------------------------------------------------------*/
/* Returns the number of children of this node                                */
/*----------------------------------------------------------------------------*/
i8 QueryNode::iNumChildren(void)
{
  if (!m_pChildren) {
    return 0;
  }

  return m_pChildren->nItems();
}

/*----------------------------------------------------------------------------*/
/* Returns true if the current node has a specific child                      */
/*----------------------------------------------------------------------------*/
b1 QueryNode::bHasChild(QueryNode* pNode)
{
  ASSERT(pNode);

  if (!m_pChildren) {
    return false;
  }

  PlanNode* pItem = m_pChildren->pHead();
  while (pItem) {
    if (pItem == pNode) {
      return true;
    }
    pItem = pItem->pNextItem();
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/* Returns the zero-based index of the child, or -1 if the child doesn't exist*/
/*----------------------------------------------------------------------------*/
i8 QueryNode::bChildIndex(QueryNode* pNode)
{
  ASSERT(pNode);

  i8 idx = -1;

  if (!m_pChildren) {
    return idx;
  }

  PlanNode* pItem = m_pChildren->pHead();
  while (pItem) {
    ++idx;
    if (pItem == pNode) {
      return idx;
    }
    pItem = pItem->pNextItem();
  }

  return idx;
}

/*----------------------------------------------------------------------------*/
/* Adds a new child at the end of the list                                    */
/*----------------------------------------------------------------------------*/
void QueryNode::vAppendChild(QueryNode* pNode)
{
  ASSERT(pNode);

  if (!m_pChildren) {
    m_pChildren = new PlanNodeList();
  }

  m_pChildren->vAppend(pNode);
}

/*----------------------------------------------------------------------------*/
/* Adds a child at the start of the list                                      */
/*----------------------------------------------------------------------------*/
void QueryNode::vPushChild(QueryNode* pNode)
{
  ASSERT(pNode);

  if (!m_pChildren) {
    m_pChildren = new PlanNodeList();
  }

  m_pChildren->vPush(pNode);
}

/*----------------------------------------------------------------------------*/
/* Insert a child after a given reference child                               */
/*----------------------------------------------------------------------------*/
void QueryNode::vInsertChildAfter(QueryNode* pInsNode, QueryNode* pRefNode)
{
  ASSERT(m_pChildren);
  ASSERT(pInsNode);
  ASSERT(pRefNode);

  m_pChildren->vInsertAfter(pInsNode, pRefNode);
}

/*----------------------------------------------------------------------------*/
/* Insert a child before a given reference child                              */
/*----------------------------------------------------------------------------*/
void QueryNode::vInsertChildBefore(QueryNode* pInsNode, QueryNode* pRefNode)
{
  ASSERT(m_pChildren);
  ASSERT(pInsNode);
  ASSERT(pRefNode);

  m_pChildren->vInsertBefore(pInsNode, pRefNode);
}

/*----------------------------------------------------------------------------*/
/* Removes a given child from the list. The removed node retains all of       */
/* its children.                                                              */
/*----------------------------------------------------------------------------*/
void QueryNode::vRemoveChild(QueryNode* pNode)
{
  ASSERT(m_pChildren);
  ASSERT(pNode);

  m_pChildren->vRemove(pNode);
}

/*----------------------------------------------------------------------------*/
/* Removes a given child from the list and merges the child's children into   */
/* the children list of this node.                                            */
/*----------------------------------------------------------------------------*/
void QueryNode::vSnipChild(QueryNode* pNode)
{
  ASSERT(m_pChildren);
  ASSERT(pNode);

  m_pChildren->vRemove(pNode);

  while (PlanNode* pChild = pNode->m_pChildren->pPop()) {
    m_pChildren->vAppend(pChild);
  }
}

/*----------------------------------------------------------------------------*/
/* Removes and returns the first child. The removed node retains all of       */
/* its children.                                                              */
/*----------------------------------------------------------------------------*/
QueryNode* QueryNode::pPopChild()
{
  ASSERT(m_pChildren);
  QueryNode* pNode = static_cast<QueryNode*>(m_pChildren->pPop());
  return pNode;
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

