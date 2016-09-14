/*============================================================================*/
/* StepArray.hpp -- Step Accumulator Array                                    */
/*============================================================================*/

#pragma once

#include "Tasks.h"

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
    
    static void vDump(eCN eClass, Object* pObject, const char* pTab);

    StepAccum* pAllocAccum(const char* pFnName);
    void vList(void);
    void vReset(void);
    
    Footer* pFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/