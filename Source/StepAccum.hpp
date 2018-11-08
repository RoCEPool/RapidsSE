/*============================================================================*/
/* StepAccum.hpp -- Step Accumulator                                          */
/*============================================================================*/
#pragma once

namespace Measurement
{
  class StepAccum : public Object
  {
  public:
    Stz* m_aStepNm;  // name of function
    i8   m_nInvoked; // number of times invoked
    i8   m_nCycles;  // total cycles
    
    // must be last
    Footer m_xFooter;
    
  public:
    StepAccum(void);
    ~StepAccum(void) noexcept(false);
    
    void vPrint(File& xFile);
    void vReset(void);
    
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
