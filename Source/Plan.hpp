/*============================================================================*/
/* Plan.hpp - plan                                                            */
/*============================================================================*/
#pragma once

namespace Plans
{
  class Plan : public Object,
               public ListItem_t<Plan>
  {
  private:
    i8    m_iPlanId;  // plan identifier       //# its an id because its unique
    s32   m_sName;    // plan name
    
    Dic   m_xDic;     // private dictionary of this plan       //# may not need
    s48K  m_sLambda;  // generated text of words
    
    LocalChain* m_pRegList; // set of register frames for use by operators
    Task* m_pExecutingTask; // task on the originating node executing the plan
    
    // always last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void* operator new(size_t, Plan* pTask);
    void  operator delete(void*);
    
    Plan(void);
    void vInit(void);                     //# I like the position of this -- ray
    ~Plan(void) noexcept(false);
    
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
    
    void vAppendToLambda(s48K& sText);
    
    i8   iPlanId(void);
    
    Task* pExecutingTask(void);
    void vExecutingTask(Task* pTask);
    
    s32& sName(void);
    void vName(s32& sName);

    Dic* pDic(void);
  
    void vFinished(void);

    s48K& sLambda(void);
    void vLambda(s48K& sLambda);
    
    void vNullifyPointers(void);
    
    void      vLocalSetPush(LocalChain* pLocalSet);
    LocalChain* pLocalSetTop(void);
    
    void vShow(void);
    
    Footer* aFooter(void);
  };
  
  Plan* _pPlan(void* aAddr);
  Plan* _pPlan(i8 iAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
