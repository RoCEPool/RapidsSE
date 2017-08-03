/*============================================================================*/
/* StepArray.hpp -- Step Accumulator Array                                    */
/*============================================================================*/
#pragma once

namespace Tasks
{
  static const i8 C_STEPS = 1024;
  
  class StepArray : public Buffer
  {
  public:
    Lock      m_zLock;
    i8        m_iNext;
    StepAccum m_xStepAccum[C_STEPS];
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    
    StepArray(void);
    ~StepArray(void);
    
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

    StepAccum* pAllocAccum(Stz* aFnName);
    void vList(File& xFile);
    void vReset(void);
  
    void vGrabWrite(void);
    void vDropWrite(void);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
