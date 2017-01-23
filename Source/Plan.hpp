/*============================================================================*/
/* Plan.hpp - plan                                                            */
/*============================================================================*/

#pragma once

#include "Plans.h"

namespace Plans
{
  class Plan : public Object,
               public ListNode_t<Plan>
  {
    
  private:
    i8      m_iPlanNo;     // plan number
    s32     m_sName;       // plan name
    
    // always last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void* operator new(size_t, Plan* pTask);
    void  operator delete(void*);
    
    Plan(void);
    Plan(const char* aLambda);
    void vInit(void);
    ~Plan(void);
    
    static void vDump(eCN eClass, Object* pObject, const char* pTab);
    static i8   iNextPlanId(void);
    
    Footer* pFooter(void);
  };
  
  Plan* _pPlan(void* pAddr);
  Plan* _pPlan(i8 iAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
