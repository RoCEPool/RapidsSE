/*============================================================================*/
/* StepTiming.hpp -- Step Timing                                              */
/*============================================================================*/
#pragma once

namespace Measurement
{
  static const i8 C_STEPS = 2048;
  
  class StepTiming : public Object
  {
  public:
    Lock      m_okLock;
    i8        m_iNext;
    StepAccum m_xStepAccum[C_STEPS];
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    
    StepTiming(void);
    ~StepTiming(void) noexcept(false);
    
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

    StepAccum* pAllocAccum(Stz* aFnName);
  
    void vDropWrite(void);
    void vGrabWrite(void);
    void vReset(void);
    void vWrite(void);

  
    Footer* aFooter(void);
  };
  
  StepTiming* _pStepTiming(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
