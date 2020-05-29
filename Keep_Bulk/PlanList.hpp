/*============================================================================*/
/* PlanList.hpp - list of plans                                               */
/*============================================================================*/
#pragma once

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
    ~PlanList(void) noexcept(false);
    
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
    
    void vContentDump(void);
    
    Plan* pFind(i8 iPlanId);
    void  vShow(void);
    
    Footer* aFooter(void);
  };
  
  PlanList* _pPlanList(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
