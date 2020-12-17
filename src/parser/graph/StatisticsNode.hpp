/*============================================================================*/
/* StatisticsNode.hpp - An mutable node that holds stats for the user's query */
/*============================================================================*/
#pragma once

namespace Parser
{
  class StatisticsNode : public MutablePlanNode
  {
  protected:

  private:
    Footer m_xFooter;
    
  public:
    // Overriding these because we inherited from ListItem_t and ListItem_t overrides
    // operator new with a fatal error that it cannot directly be newed.
    void* operator new(size_t);
    void  operator delete(void*);

    StatisticsNode(Stz* szName, Stz* szType, Value xValue,
                   PlanNodeList* pProperties, PlanNodeList* pChildren);
    virtual ~StatisticsNode(void);

    Footer* aFooter(void);

  };
  
  StatisticsNode* _pStatisticsNode(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
