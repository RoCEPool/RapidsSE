/*============================================================================*/
/* PlanNodeList.hpp - a list of PlanNode objects                              */
/*============================================================================*/
#pragma once

namespace Parser
{  
  class PlanNodeList : public ListNL_t<PlanNode>
  {
  private:
    // must be last
    Footer m_xFooter;
    
   public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    PlanNodeList();
    ~PlanNodeList(void);
    
    Footer* aFooter(void);
    
  };
  
  PlanNodeList* _pPlanNodeList(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
