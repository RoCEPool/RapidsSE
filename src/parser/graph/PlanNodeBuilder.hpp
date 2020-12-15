/*============================================================================*/
/* PlanNodeBuilder.hpp - A mutable way to build PlanNodes                     */
/*============================================================================*/
#pragma once

namespace Parser
{
  class PlanNodeBuilder : public Object,
                          public ListItem_t<PlanNodeBuilder>
  {
  public:
    s256  m_sName;
    s256  m_sType;
    Value m_xValue;
    PlanNodeList* m_pProperties;
    PlanNodeList* m_pChildren;
    
  private:
    Footer m_xFooter;
    
   public:
    // Overriding these because we inherited from ListItem_t
    void* operator new(size_t);
    void  operator delete(void*);
    
    PlanNodeBuilder();
    ~PlanNodeBuilder(void);

    PlanNode* pBuild();
    
    Footer* aFooter(void);

  };
  
  PlanNodeBuilder* _pPlanNodeBuilder(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
