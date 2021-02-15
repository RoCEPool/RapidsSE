/*============================================================================*/
/* QueryNode.cpp - An mutable node that holds the plan of the user's query    */
/*============================================================================*/

#include "BASE.h"
#include "QueryNode.hpp"

using namespace Parser;

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
    : PlanNode(CN_QUERYNODE, szName, szType, {},
               new PlanNodeList(LK_LIST), new PlanNodeList(LK_LIST))
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUERYNODE,_FOOTER_);

  _checkInitialised_(CN_QUERYNODE);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_QUERYNODE,_FOOTER_);

}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
QueryNode::QueryNode(Stz* szName, Stz* szType, Value&& xValue)
    : PlanNode(CN_QUERYNODE, szName, szType, static_cast<Value&&>(xValue),
               new PlanNodeList(LK_LIST), new PlanNodeList(LK_LIST))
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUERYNODE,_FOOTER_);

  _checkInitialised_(CN_QUERYNODE);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_QUERYNODE,_FOOTER_);

}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
QueryNode::QueryNode(Stz* szName, Stz* szType, Value&& xValue,
                               PlanNodeList* pProperties, PlanNodeList* pChildren)
    : PlanNode(CN_QUERYNODE, szName, szType, static_cast<Value&&>(xValue),
               pProperties, pChildren)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUERYNODE,_FOOTER_);

  _checkInitialised_(CN_QUERYNODE);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_QUERYNODE,_FOOTER_);

}

/*----------------------------------------------------------------------------*/
/* Copy constructor                                                           */
/*----------------------------------------------------------------------------*/
QueryNode::QueryNode(const QueryNode& oNode)
    : PlanNode(CN_QUERYNODE, oNode)
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
/* Give this node a new name                                                  */
/*----------------------------------------------------------------------------*/
void QueryNode::vSetName(Stz *szName)
{
  // This is useful when building compound objects and this object will be
  // a property of some parent. The name of this object is how the property
  // is referred to from the parent. So this is useful where we need the
  // properties to have specific names.

  ASSERT(szName);
  ASSERT(szName->nLength() > 0);

  m_sName.vCopy(szName);
}

/*----------------------------------------------------------------------------*/
/* Sets the type of this node                                                 */
/*----------------------------------------------------------------------------*/
void QueryNode::vSetType(Stz* szType)
{
  // TODO: Validate that it is a valid type
  m_sType.vCopy(szType);
}

/*----------------------------------------------------------------------------*/
/* Sets the value component of this node                                      */
/*----------------------------------------------------------------------------*/
void QueryNode::vSetValue(Value&& xValue)
{
  m_xValue = static_cast<Value&&>(xValue);
}

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

  if (!m_pChildren || m_pChildren->nItems() == 0) {
    return false;
  }

  PlanNodeIter* pChildIter = m_pChildren->pNewIter();
  while (pChildIter->bHasNext()) {
    PlanNode* pItem = pChildIter->pNext();

    if (pItem == pNode) {
      delete pChildIter;
      return true;
    }
  }
  delete pChildIter;
  return false;
}

/*----------------------------------------------------------------------------*/
/* Adds a new child at the end of the list                                    */
/*----------------------------------------------------------------------------*/
void QueryNode::vAppendChild(QueryNode* pNode)
{
  ASSERT(pNode);

  if (!m_pChildren) {
    m_pChildren = new PlanNodeList(LK_LIST);
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
    m_pChildren = new PlanNodeList(LK_LIST);
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
/* Insert a child in the current node and make it the parent of any existing  */
/* children. e.g. if node A has children B, C, and we are inserting node D    */
/* then after this operation node A will have one child, node D, and node D   */
/* will have children B and C.                                                */
/*----------------------------------------------------------------------------*/
void QueryNode::vInsertAsParentOfChildren(QueryNode* pInsNode)
{
  ASSERT(m_pChildren);
  ASSERT(pInsNode);

  // Move the children from the current node to the node being inserted
  QueryNode* pChild;
  while( (pChild = static_cast<QueryNode*>(m_pChildren->pPop())) != nullptr ) {
    pInsNode->vAppendChild(pChild);
  }

  ASSERT(m_pChildren->nItems() == 0);

  m_pChildren->vPush(pInsNode);
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

/*----------------------------------------------------------------------------*/
/* Appends a node as a property of this node                                  */
/*----------------------------------------------------------------------------*/
void QueryNode::vAppendProperty(QueryNode* pNode)
{
  if (!m_pProperties) {
    m_pProperties = new PlanNodeList(LK_LIST);
  }

  m_pProperties->vAppend(pNode);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

