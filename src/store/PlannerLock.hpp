/*============================================================================*/
/* PlannerLock.hpp -- Planner Lock                                            */
/*============================================================================*/
#pragma once

#include "MasterLock.hpp"

namespace STORE
{
  class PlannerLock : public MasterLock
  {
  private:
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    PlannerLock(void);
   ~PlannerLock(void) noexcept(false);
    
  private:
    Footer* aFooter(void);
  };
  
  PlannerLock* _pPlannerLock(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
