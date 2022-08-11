/*============================================================================*/
/* StepAccum.hpp -- Step Accumulator                                          */
/*============================================================================*/
#pragma once

namespace STORE
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
    i8   m_iHeapsAlloc[12]; // total allocs done during per heap type this step
    i8   m_iHeapsFree[12];  // total frees done during per heap type this step

    // must be last
    Footer m_xFooter;
    
  public:
    StepAccum(void);
    ~StepAccum(void) noexcept(false);
  
    void vEnter(i8& nEnterCycles, i8& nEnterMisses);
    void vExit(i8 nEnterCycles, i8 nEnterMisses);
    void vHeapAllocCount(i8 iWhichHeap);
    void vHeapFreeCount(i8 iWhichHeap);
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
