/*============================================================================*/
/* PlanNodeBuilderList.hpp - a list of PlanNodeBuilder objects                */
/*============================================================================*/
#pragma once

#include "PlanNodeBuilder.hpp"

namespace Parser
{  
  class PlanNodeBuilderList : public ListNL_t<PlanNodeBuilder>
  {
  private:
    // must be last
    Footer m_xFooter;
    
   public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    PlanNodeBuilderList();
    ~PlanNodeBuilderList(void);
    
    Footer* aFooter(void);
    
  };
  
  PlanNodeBuilderList* _pPlanNodeBuilderList(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
