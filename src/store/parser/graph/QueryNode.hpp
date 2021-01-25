/*============================================================================*/
/* QueryNode.hpp - An mutable node that holds the plan of the user's query    */
/*============================================================================*/
#pragma once

#include "MutablePlanNode.hpp"

namespace Parser
{
  class QueryNode : public MutablePlanNode
  {
  protected:

  private:
    Footer m_xFooter;
    
  public:
    // Overriding these because we inherited from ListItem_t and ListItem_t overrides
    // operator new with a fatal error that it cannot directly be newed.
    void* operator new(size_t);
    void  operator delete(void*);

    QueryNode(Stz* szName, Stz* szType);
    QueryNode(Stz* szName, Stz* szType, Value&& pValue);
    QueryNode(Stz* szName, Stz* szType, Value&& pValue,
                   PlanNodeList* pProperties, PlanNodeList* pChildren);
    QueryNode(const QueryNode& oNode);
    virtual ~QueryNode(void);

    Footer* aFooter(void);

    void vRename(Stz* szName);

    // Returns the number of children of this node
    i8 iNumChildren(void);

    // Returns true if the current node has a specific child
    b1 bHasChild(QueryNode* pNode);

    // Adds a new child at the end of the list
    void vAppendChild(QueryNode* pNode);

    // Adds a child at the start of the list
    void vPushChild(QueryNode* pNode);

    // Insert a child after a given reference child
    void vInsertChildAfter(QueryNode* pInsNode, QueryNode* pRefNode);

    // Insert a child before a given reference child
    void vInsertChildBefore(QueryNode* pInsNode, QueryNode* pRefNode);

    // Removes a given child from the list. The removed node retains all of its children.
    void vRemoveChild(QueryNode* pNode);

    // Removes a given child from the list and merges the child's children into
    // the children list of this node.
    void vSnipChild(QueryNode* pNode);

    // Removes and returns the first child. The removed node retains all of its children.
    QueryNode* pPopChild();
  };
  
  QueryNode* _pQueryNode(void* aAddr);
  //typedef StackNL_t<CN_QUERYNODESTACK,QueryNode,1024> QueryNodeStack;
  //typedef ListExo_t<CN_QUERYNODELIST, QueryNode> QueryNodeList;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
