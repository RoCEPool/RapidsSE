/*============================================================================*/
/* StepAccum.hpp -- Step Accumulator                                          */
/*============================================================================*/

#pragma once

#include "Tasks.h"

namespace Tasks
{
  class StepAccum : public Object
  {
  public:
    const char* m_aFnName;  // name of function
    i8          m_nInvoked; // number of times invoked
    i8          m_nCycles;  // total cycles
    
    // must be last
    Footer m_xFooter;
    
  public:
    StepAccum(void);
    ~StepAccum(void);
    
    void vPrint(void);
    void vReset(void);
    
    Footer* pFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
