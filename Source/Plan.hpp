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
    u8     m_uPlanId;  // plan identifier       //# its an id because its unique
    s32    m_sName;    // plan name
    
    Dic    m_xDic;     // private dictionary of this plan
    s48K   m_sLambda;  // generated text of words
    
    RegFrame* m_pRegList; // set of register frames for use by operators
    
    // always last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void* operator new(size_t, Plan* pTask);
    void  operator delete(void*);
    
    Plan(void);
    void vInit(void);                     //# I like the position of this -- ray
    ~Plan(void);
    
    static void vDump(eCN eClass, Object* pObject, const char* pTab);
    
    u8   uPlanId(void);
    s32& sName(void);

    Dic* pDic(void);

    s48K& sLambda(void);
    void vLambda(s48K& sLambda);

    Footer* pFooter(void);
  };
  
  Plan* _pPlan(void* pAddr);
  Plan* _pPlan(i8 iAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
