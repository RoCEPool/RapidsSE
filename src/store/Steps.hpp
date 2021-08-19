/*============================================================================*/
/* Steps.hpp -- Step Timing                                              */
/*============================================================================*/
#pragma once

extern pthread_key_t giTLS_ThreadKey;
extern pthread_key_t giTLS_TaskKey;
extern pthread_key_t giTLS_HandlerKey;
extern pthread_key_t giTLS_StepKey;

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
    
    Steps(void);
    ~Steps(void) noexcept(false);
    
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

    StepAccum* pAllocAccum(Stz* aFnName);
  
    void vDropWrite(void);
    void vGrabWrite(void);
    void vReset(void);
    void vShow(Stz* szStepNm);
    void vWrite(Stz* szPath);
    void vWriteSingleStep(Stz* szPath, Stz* szStepNm);

  
    Footer* aFooter(void);
  };
  
  Steps* _pSteps(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
