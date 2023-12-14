/*============================================================================*/
/* QueryNode.hpp - An mutable node that holds the plan of the user's query    */
/*============================================================================*/
#pragma once

#include "PlanNode.hpp"

namespace Parser
{
  class QueryNode : public PlanNode
  {
  protected:

  private:
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    QueryNode(Stz* szName, Stz* szType);
    QueryNode(Stz* szName, Stz* szType, Value&& pValue);
    QueryNode(Stz* szName, Stz* szType, Value&& pValue,
                   PlanNodeList* pProperties, PlanNodeList* pChildren);
    QueryNode(const QueryNode& oNode);
    virtual ~QueryNode(void);

    Footer* aFooter(void);

    void vSetName(Stz* szName);
    void vSetType(Stz* szType);
    void vSetValue(Value&& xValue);  // move semantics

    // Returns the number of children of this node
    i8 iNumChildren(void);

    // Returns true if the current node has a specific child
    b1 bHasChild(QueryNode* pNode);

    // Adds a new child at the end of the list
    void vAppendChild(QueryNode* pNode);

    // Adds a child at the start of the list
    void vPushChild(QueryNode* pNode);

    // Insert a child in the current node after a given reference child
    void vInsertChildAfter(QueryNode* pInsNode, QueryNode* pRefNode);

    // Insert a child in the current node before a given reference child
    void vInsertChildBefore(QueryNode* pInsNode, QueryNode* pRefNode);

    // Inserts a list of children in the current node after a given reference child
    void vInsertChildrenAfter(PlanNodeList* pChildren, QueryNode* pRefNode);

    // Inserts a list of children in the current node before a given reference child
    void vInsertChildrenBefore(PlanNodeList* pChildren, QueryNode* pRefNode);

    // Insert a child in the current node and make it the parent of any existing children.
    // See implementation for more details.
    void vInsertAsParentOfChildren(QueryNode* pInsNode);

    // Inserts a child in the current node in place of the first child and
    // makes that first child a child of the node being inserted.
    void vInsertAsParentOfChild1(QueryNode* pInsNode);

    // Inserts a child in the current node in place of the second child and
    // makes that second child a child of the node being inserted.
    void vInsertAsParentOfChild2(QueryNode* pInsNode);

    // Inserts pInsNode as a child and makes pCurChild a child of pInsNode
    void vInsertAsParentOfChild(QueryNode* pCurChild, QueryNode* pInsNode);

    // Removes a given child from the list. The removed node retains all of its children.
    void vRemoveChild(QueryNode* pNode);

    // Removes a given child from the list and merges the child's children into
    // the children list of this node.
    void vSnipChild(QueryNode* pNode);

    // Removes and returns the first child. The removed node retains all of its children.
    QueryNode* pPopChild();

    // Removes and returns the last child. The removed node retains all of its children.
    QueryNode* pPopLastChild();

    void vAppendProperty(QueryNode* pNode);

    // Removes and returns the property, otherwise returns nullptr
    QueryNode* pRemoveProperty(Stz* szPropName);

    // Return a deep copy of the current node, it's properties and all children.
    // NB: May need to make the name unique if using this copy as a sibling of its original.
    QueryNode* pDeepCopy(b1 bNewName);

    // Return a deep copy of the current node and it's properties but use the supplied children
    // NB: May need to make the name unique if using this copy as a sibling of its original.
    QueryNode* pDeepCopy(b1 bNewName, PlanNodeList* pNewChildren);

    // Change the current node in place to reflect pTemplate.
    // This is an alternative to removing the current node and inserting a new node.
    // Changing it in place allows it to avoid affecting top-down walks of the graph.
    void vTransmogrifyTo(QueryNode* pTemplate);
    void vTransmogrifyTo(QueryNode* pTemplate, b1 bKeepName);
  };
  
  QueryNode* _pQueryNode(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
