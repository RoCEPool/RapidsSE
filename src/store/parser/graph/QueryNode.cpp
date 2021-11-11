/*============================================================================*/
/* QueryNode.cpp - An mutable node that holds the plan of the user's query    */
/*============================================================================*/

#include "BASE.h"
#include "QueryNode.hpp"
#include "NodeNamer.hpp"

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
               new PlanNodeList, new PlanNodeList)
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
               new PlanNodeList, new PlanNodeList)
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
  m_xValue.vDealloc();
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
    m_pChildren = new PlanNodeList;
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
    m_pChildren = new PlanNodeList;
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
/* Insert a child after a given reference child                               */
/*----------------------------------------------------------------------------*/
void QueryNode::vInsertChildrenAfter(PlanNodeList* pChildren, QueryNode* pRefNode)
{
  ASSERT(m_pChildren);
  ASSERT(pRefNode);

  // We need to maintain the same order, so we need to remember the last
  // inserted item.
  QueryNode* pLastInsItem = pRefNode;

  PlanNodeIter* pIter = pChildren->pNewIter();

  while (pIter->bHasNext()) {
    QueryNode* pChild = _pQueryNode(pIter->pNext());
    m_pChildren->vInsertAfter(pChild, pLastInsItem);
    pLastInsItem = pChild;
  }
  delete pIter;
}

/*----------------------------------------------------------------------------*/
/* Insert a child before a given reference child                              */
/*----------------------------------------------------------------------------*/
void QueryNode::vInsertChildrenBefore(PlanNodeList* pChildren, QueryNode* pRefNode)
{
  ASSERT(m_pChildren);
  ASSERT(pRefNode);

  PlanNodeIter* pIter = pChildren->pNewIter();

  while (pIter->bHasNext()) {
    QueryNode* pChild = _pQueryNode(pIter->pNext());
    m_pChildren->vInsertAfter(pChild, pRefNode);
  }
  delete pIter;
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
  while( (pChild = _pQueryNode(m_pChildren->pPop())) != nullptr ) {
    pInsNode->vAppendChild(pChild);
  }

  ASSERT(m_pChildren->nItems() == 0);

  m_pChildren->vPush(pInsNode);
}

/*----------------------------------------------------------------------------*/
/* Inserts a child in the current node in place of the first child and        */
/* makes that first child a child of the node being inserted.                 */
/* e.g. if node A has children B and C and we are inserting node D then       */
/* node A will have children D and C, and node D will have a child node B.    */
/*----------------------------------------------------------------------------*/
void QueryNode::vInsertAsParentOfChild1(QueryNode* pInsNode)
{
  ASSERT(m_pChildren);
  ASSERT(m_pChildren->nItems() > 0);
  ASSERT(pInsNode);

  QueryNode* pChild1 = _pQueryNode(m_pChildren->pPop());
  pInsNode->vAppendChild(pChild1);
  m_pChildren->vPush(pInsNode);
}

/*----------------------------------------------------------------------------*/
/* Inserts a child in the current node in place of the second child and       */
/* makes that second child a child of the node being inserted.                */
/* e.g. if node A has children B and C and we are inserting node D then       */
/* node A will have children B and D, and node D will have a child node C.    */
/*----------------------------------------------------------------------------*/
void QueryNode::vInsertAsParentOfChild2(QueryNode* pInsNode)
{
  ASSERT(m_pChildren);
  ASSERT(m_pChildren->nItems() > 1);
  ASSERT(pInsNode);

  QueryNode* pChild1 = _pQueryNode(m_pChildren->pPop());
  QueryNode* pChild2 = _pQueryNode(m_pChildren->pPop());
  pInsNode->vAppendChild(pChild2);
  m_pChildren->vPush(pInsNode);
  m_pChildren->vPush(pChild1);
}

/*----------------------------------------------------------------------------*/
/* Inserts pInsNode as a child and makes pCurChild a child of pInsNode        */
/*----------------------------------------------------------------------------*/
void QueryNode::vInsertAsParentOfChild(QueryNode* pCurChild, QueryNode* pInsNode)
{
  ASSERT(m_pChildren);
  ASSERT(m_pChildren->nItems() >= 1);
  ASSERT(pCurChild);
  ASSERT(bHasChild(pCurChild));
  ASSERT(pInsNode);

  vInsertChildAfter(pInsNode, pCurChild);
  vRemoveChild(pCurChild);
  pInsNode->vAppendChild(pCurChild);
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
  QueryNode* pNode = _pQueryNode(m_pChildren->pPop());
  return pNode;
}

/*----------------------------------------------------------------------------*/
/* Removes and returns the last child. The removed node retains all of        */
/* its children.                                                              */
/*----------------------------------------------------------------------------*/
QueryNode* QueryNode::pPopLastChild()
{
  ASSERT(m_pChildren);
  QueryNode* pNode = _pQueryNode(m_pChildren->pPopTail());
  return pNode;
}

/*----------------------------------------------------------------------------*/
/* Appends a node as a property of this node                                  */
/*----------------------------------------------------------------------------*/
void QueryNode::vAppendProperty(QueryNode* pNode)
{
  if (!m_pProperties) {
    m_pProperties = new PlanNodeList;
  }

  m_pProperties->vAppend(pNode);
}

/*----------------------------------------------------------------------------*/
/* Removes and returns the property, otherwise returns nullptr                */
/*----------------------------------------------------------------------------*/
QueryNode* QueryNode::pRemoveProperty(Stz* szPropName)
{
  if (!m_pProperties) {
    return nullptr;
  }

  QueryNode* pProp = nullptr;

  PlanNodeIter* pPropIter = m_pProperties->pNewIter();
  while (pPropIter->bHasNext()) {
    PlanNode* pItem = pPropIter->pNext();

    if (pItem->szName()->bEqual(szPropName)) {
      pProp = _pQueryNode(pItem);
      break;
    }
  }
  delete pPropIter;

  m_pProperties->vRemove(pProp);
  return pProp;
}

/*----------------------------------------------------------------------------*/
/* Return a deep copy of the current node, it's properties and all children   */
/*----------------------------------------------------------------------------*/
QueryNode* QueryNode::pDeepCopy(b1 bNewName)
{
  // Deep copy the value
  Value xValue = this->m_xValue.xDup();

  // Create a new unique name
  Stz* szName = this->szName();
  s256 sNewName;

  if (bNewName) {
    NodeNamer::vGenerateName(this, sNewName);
    szName = sNewName.szStz();
  }

  // Create the new node. This will copy in the name and type and take ownership
  // of the value and any pointers it has.
  QueryNode* pNewNode = new QueryNode(szName,
                                      this->szType(),
                                      static_cast<Value&&>(xValue));

  // Deep copy the properties individually before adding them to the new node
  if (m_pProperties->nItems() > 0) {
    PlanNodeIter* pPropIter = m_pProperties->pNewIter();
    while (pPropIter->bHasNext()) {
      QueryNode* pProp = _pQueryNode(pPropIter->pNext());
      QueryNode* pDupProp = pProp->pDeepCopy(false);
      pNewNode->vAppendProperty(pDupProp);
    }
    delete pPropIter;
  }

  // Deep copy the children
  if (m_pChildren->nItems() > 0) {
    PlanNodeIter* pChildIter = m_pChildren->pNewIter();
    while (pChildIter->bHasNext()) {
      QueryNode* pChild = _pQueryNode(pChildIter->pNext());
      QueryNode* pDupChild = pChild->pDeepCopy(false);
      pNewNode->vAppendChild(pDupChild);
    }
    delete pChildIter;
  }

  return pNewNode;
}

/*----------------------------------------------------------------------------*/
/* Return a deep copy of the current node and it's properties but use the     */
/* supplied children. pNewChildren can be null to specify no children         */
/*----------------------------------------------------------------------------*/
QueryNode* QueryNode::pDeepCopy(b1 bNewName, PlanNodeList* pNewChildren)
{
  // Deep copy the value
  Value xValue = this->m_xValue.xDup();

  // Create a new unique name
  Stz* szName = this->szName();
  s256 sNewName;

  if (bNewName) {
    NodeNamer::vGenerateName(this, sNewName);
    szName = sNewName.szStz();
  }

  if (!pNewChildren) {
    pNewChildren = new PlanNodeList();
  }

  // Create the new node. This will copy in the name and type and take ownership
  // of the value and any pointers it has.
  QueryNode* pNewNode = new QueryNode(szName,
                                      this->szType(),
                                      static_cast<Value&&>(xValue),
                                      new PlanNodeList(),
                                      pNewChildren);

  // Deep copy the properties individually before adding them to the new node
  if (m_pProperties->nItems() > 0) {
    PlanNodeIter* pPropIter = m_pProperties->pNewIter();
    while (pPropIter->bHasNext()) {
      QueryNode* pProp = _pQueryNode(pPropIter->pNext());
      QueryNode* pDupProp = pProp->pDeepCopy(false);
      pNewNode->vAppendProperty(pDupProp);
    }
    delete pPropIter;
  }

  return pNewNode;
}

/*----------------------------------------------------------------------------*/
/* Change the current node in place to reflect pTemplate.                     */
/* This is an alternative to removing the current node and inserting a new    */
/* node. Changing it in place allows it to avoid affecting top-down walks of  */
/* the graph.                                                                 */
/*----------------------------------------------------------------------------*/
void QueryNode::vTransmogrifyTo(QueryNode* pTemplate)
{
  vTransmogrifyTo(pTemplate, false);
}

/*----------------------------------------------------------------------------*/
/* Change the current node in place to reflect pTemplate.                     */
/* This is an alternative to removing the current node and inserting a new    */
/* node. Changing it in place allows it to avoid affecting top-down walks of  */
/* the graph.                                                                 */
/* If bKeepName is true then the existing name of the current node is kept.   */
/*----------------------------------------------------------------------------*/
void QueryNode::vTransmogrifyTo(QueryNode* pTemplate, b1 bKeepName)
{
  ASSERT(pTemplate);

  if (!bKeepName) {
    this->vSetName(pTemplate->szName());
  }

  this->vSetType(pTemplate->szType());
  this->vSetValue(pTemplate->xValue().xDup());

  if (m_pProperties) {
    m_pProperties->vDeleteAll();
  }

  if (pTemplate->m_pProperties) {
    if (!m_pProperties) {
      m_pProperties = new PlanNodeList();
    }

    PlanNodeIter* pIter = pTemplate->pNewPropertyIter();
    while (pIter->bHasNext()) {
      // Deep copy the property so that we aren't modifying the template
      QueryNode* pProperty = _pQueryNode(pIter->pNext())->pDeepCopy(false);
      m_pProperties->vAppend(pProperty);
    }
    delete pIter;
  }

  if (m_pChildren) {
    m_pChildren->vDeleteAll();
  }

  if (pTemplate->m_pChildren) {
    if (!m_pChildren) {
      m_pChildren = new PlanNodeList();
    }

    PlanNodeIter* pIter = pTemplate->pNewChildrenIter();
    while (pIter->bHasNext()) {
      // Deep copy the child so that we aren't modifying the template
      QueryNode* pChild = _pQueryNode(pIter->pNext())->pDeepCopy(true);
      m_pChildren->vAppend(pChild);
    }
    delete pIter;
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

