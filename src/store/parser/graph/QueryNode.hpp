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

    // Insert a child in the current node and make it the parent of any existing children.
    // See implementation for more details.
    void vInsertAsParentOfChildren(QueryNode* pInsNode);

    // Removes a given child from the list. The removed node retains all of its children.
    void vRemoveChild(QueryNode* pNode);

    // Removes a given child from the list and merges the child's children into
    // the children list of this node.
    void vSnipChild(QueryNode* pNode);

    // Removes and returns the first child. The removed node retains all of its children.
    QueryNode* pPopChild();

    void vAppendProperty(QueryNode* pNode);
  };
  
  QueryNode* _pQueryNode(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
