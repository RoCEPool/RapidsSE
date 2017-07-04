/*============================================================================*/
/* StepAccum.hpp -- Step Accumulator                                          */
/*============================================================================*/
#pragma once

namespace Tasks
{
  class StepAccum : public Object
  {
  public:
    Str* m_aStepNm;  // name of function
    i8   m_nInvoked; // number of times invoked
    i8   m_nCycles;  // total cycles
    
    // must be last
    Footer m_xFooter;
    
  public:
    StepAccum(void);
    ~StepAccum(void);
    
    void vPrint(File& xFile);
    void vReset(void);
    
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
