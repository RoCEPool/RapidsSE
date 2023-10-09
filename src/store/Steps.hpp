/*============================================================================*/
/* Steps.hpp -- Step Timing                                              */
/*============================================================================*/
#pragma once

extern pthread_key_t giTLS_ThreadKey;
extern pthread_key_t giTLS_TaskKey;
extern pthread_key_t giTLS_HandlerKey;
extern pthread_key_t giTLS_StepKey;
extern pthread_key_t gbTLS_FaultInjection;
extern pthread_key_t giTLS_FaultPoint;
extern pthread_key_t giTLS_CurrentPoint;

namespace STORE
{
  static const i8 C_STEPS = 2048;
  
  class Steps : public Object
  {
  public:
    Lock      m_okLock;
    i8        m_iNext;
    StepAccum m_xStepAccum[C_STEPS];
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Steps(void);
    ~Steps(void) noexcept(false);
    
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

    StepAccum* pAllocAccum(Stz* aFnName);
  
    void vDropWrite(void);
    void vGrabWrite(void);
    void vReset(void);
    void vShow(Stz* szStepNm);
    void vShowAll(void);
    void vWrite(Stz* szPath);
    void vWriteAll(void);
    void vWriteSingleStep(Stz* szPath, Stz* szStepNm);

  
    Footer* aFooter(void);
  };
  
  Steps* _pSteps(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
