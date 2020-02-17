/*============================================================================*/
/* StepAccum.hpp -- Step Accumulator                                          */
/*============================================================================*/
#pragma once

namespace Steps
{
  class StepAccum : public Object
  {
  public:
    Stz* m_szStepNm;    // name of function
    i8   m_nInvoked;    // number of times the function has been invoked
  
    i8   m_nSumCycles;  // total cycles
    i8   m_nMaxCycles;  // maximum cycles
    i8   m_nSumMisses;  // total llc misses
    i8   m_nMaxMisses;  // maximum llc misses

    // must be last
    Footer m_xFooter;
    
  public:
    StepAccum(void);
    ~StepAccum(void) noexcept(false);
  
    void vEnter(i8& nEnterCycles, i8& nEnterMisses);
    void vExit(i8 nEnterCycles, i8 nEnterMisses);
    void vPrint(File* pFile);
    void vPrintAll(File* pFile);
    void vReset(void);
    void vShow(void);

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
