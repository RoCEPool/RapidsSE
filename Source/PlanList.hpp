/*============================================================================*/
/* PlanList.hpp - list of plans                                               */
/*============================================================================*/

#pragma once

#include "Plans.h"

namespace Plans
{
  class PlanList : public Object,
                   public List_t<Plan>
  {
  private:
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void* operator new(size_t, PlanList* pPlanList);
    void  operator delete(void*);
    
    PlanList(void);
    ~PlanList(void);
    
    static void vDump(eCN eClass, Object* pObject, const char* pTab);
    
    void vContentDump(void);
    
    Plan* pFind(u8 uPlanId);
    
    Footer* pFooter(void);
  };
  
  PlanList* _pPlanList(void* pAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
