/*============================================================================*/
/* QueryNode.hpp - An mutable node that holds the plan of the user's query    */
/*============================================================================*/
#pragma once

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

    QueryNode(Stz* szName, Stz* szType, Value xValue,
                   PlanNodeList* pProperties, PlanNodeList* pChildren);
    virtual ~QueryNode(void);

    Footer* aFooter(void);

  };
  
  QueryNode* _pQueryNode(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
