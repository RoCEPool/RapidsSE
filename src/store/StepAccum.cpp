/*============================================================================*/
/* StepAccum.cpp -- Step Accumulator                                          */
/*============================================================================*/

#include "BASE.h"

#include "StepAccum.hpp"

#include "SP_Timer.hpp"

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
StepAccum::StepAccum(void)
: Object(CN_STEPACCUM)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STEPACCUM,_FOOTER_);
  
  m_szStepNm  = nullptr;
  m_nInvoked = 0;
  
  m_nSumCycles = 0;
  m_nMaxCycles = 0;
  m_nSumMisses = 0;
  m_nMaxMisses = 0;

  m_nSumAllocs = 0;
  m_nMaxAllocs = 0;
  m_nSumFrees = 0;
  m_nMaxFrees = 0;

  for(i8 i = 0; i < 12; i++)
  {
    m_iHeapsAlloc[i] = 0;
    m_iHeapsFree[i] = 0;
  }

  _checkInitialised_(CN_STEPACCUM);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_STEPACCUM,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
StepAccum::~StepAccum(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STEPACCUM,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* enter a step                                                               */
/*----------------------------------------------------------------------------*/
void StepAccum::vEnter(i8& nEnterCycles, i8& nEnterMisses)
{
#ifdef WITH_APPLE
  nEnterCycles = TickingDevice::iGetTick();
  nEnterMisses = 0; //iPmc_GetLlcMisses();
#else
  nEnterCycles = 0;
  nEnterMisses = 0;
#endif
}

/*----------------------------------------------------------------------------*/
/* exit a step                                                                */
/*----------------------------------------------------------------------------*/
void StepAccum::vExit(i8 nEnterCycles, i8 nEnterMisses)
{
  #ifdef WITH_APPLE
  i8 nExitCycles = TickingDevice::iGetTick();
  i8 nDiffCycles = nExitCycles - nEnterCycles;

  i8 nExitMisses = 0; //iPmc_GetLlcMisses();
  i8 nDiffMisses = nExitMisses - nEnterMisses;

  m_nInvoked++;
  m_nSumCycles += nDiffCycles;
  m_nSumMisses += nDiffMisses;
  
  if (nDiffCycles > m_nMaxCycles)
    m_nMaxCycles = nDiffCycles;
  if (nDiffMisses > m_nMaxMisses)
    m_nMaxMisses = nDiffMisses;
#else
  // do nothing
  UNUSED(nEnterCycles);
  UNUSED(nEnterMisses);

#endif
  if(m_nSumAllocs > m_nMaxAllocs)
    m_nMaxAllocs = m_nSumAllocs;

  if(m_nSumFrees > m_nMaxFrees)
    m_nMaxFrees = m_nSumFrees;

  m_nSumAllocs = 0;
  m_nSumFrees = 0;
}

/*----------------------------------------------------------------------------*/
/* print to given file                                                        */
/*----------------------------------------------------------------------------*/
void StepAccum::vHeapAllocCount(i8 iWhichHeap)
{
  RASSERT(iWhichHeap > 0);
  m_iHeapsAlloc[iWhichHeap-1]++;
}

/*----------------------------------------------------------------------------*/
/* print to given file                                                        */
/*----------------------------------------------------------------------------*/
void StepAccum::vHeapFreeCount(i8 iWhichHeap)
{
  RASSERT(iWhichHeap > 0);
  m_iHeapsFree[iWhichHeap-1]++;
}

/*----------------------------------------------------------------------------*/
/* print to given file                                                        */
/*----------------------------------------------------------------------------*/
void StepAccum::vPrint(File* pFile)
{
  if (0 == m_nInvoked)
    return;
  
  i8 nCyclesPerGigaNanosecond = TickingDevice::iTicksPerSec();
  i8 nCyclesPerNanosecond = DIV(nCyclesPerGigaNanosecond,C_1G);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // 0) m_szStepNm
  
  // 1) m_nInvoked
  
  // 2) m_nSumCycles
  
  //    Cycles   Sum(Cycles)
  // 3) ------ = -----------
  //    Invoke   NoOfInvokes
  i8 nCyclesPerInvoke = DIV(m_nSumCycles,m_nInvoked);
  
  //               Cycles
  //               ------
  //      Ns       Invoke     Cycles     NS
  // 4) ------ = ---------- : ------ x ------
  //    Invoke     Cycles     Invoke   Cycles
  //               ------
  //                 Ns
  i8 nNanosecondsPerInvoke = DIV(nCyclesPerInvoke,nCyclesPerNanosecond);
  
  //       Max(Ns) for     Max(Cycles)    Max(Cycles)
  //        an Invoke  = ------------- : ----------- x Ns
  // 5)                     Cycles          Cycles
  //                        ------
  //                          Ns
  i8 nMaxNanosecondsForInvoke = DIV(m_nMaxCycles,nCyclesPerNanosecond);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  //             0   1     2     3     4     5
  pFile->vPrint("%s, %lld, %lld, %lld, %lld, %lld\n"_Stz,
               m_szStepNm,               // 0
               m_nInvoked,              // 1
               
               m_nSumCycles,            // 2
               nCyclesPerInvoke,        // 3
               nNanosecondsPerInvoke,   // 4
               nMaxNanosecondsForInvoke,// 5
               
              C_VARGS_END);
}

/*----------------------------------------------------------------------------*/
/* print to given file                                                        */
/*----------------------------------------------------------------------------*/
void StepAccum::vPrintAll(File* pFile)
{
  if (0 == m_nInvoked)
    return;
  
  i8 nCyclesPerGigaNanosecond = TickingDevice::iTicksPerSec();
  i8 nCyclesPerNanosecond = DIV(nCyclesPerGigaNanosecond,C_1G);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // 0) m_szStepNm
  
  // 1) m_nInvoked
  
  // 2) m_nSumCycles
  
  //    Cycles   Sum(Cycles)
  // 3) ------ = -----------
  //    Invoke   NoOfInvokes
  i8 nCyclesPerInvoke = DIV(m_nSumCycles,m_nInvoked);
  
  //               Cycles
  //               ------
  //      Ns       Invoke     Cycles     NS
  // 4) ------ = ---------- : ------ x ------
  //    Invoke     Cycles     Invoke   Cycles
  //               ------
  //                 Ns
  i8 nNanosecondsPerInvoke = DIV(nCyclesPerInvoke,nCyclesPerNanosecond);
  
  //       Max(Ns) for     Max(Cycles)    Max(Cycles)
  //        an Invoke  = ------------- : ----------- x Ns
  // 5)                     Cycles          Cycles
  //                        ------
  //                          Ns
  i8 nMaxNanosecondsForInvoke = DIV(m_nMaxCycles,nCyclesPerNanosecond);

  // 6) m_nSumMisses
  
  // 7) m_nMaxMisses

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  pFile->vPrint("%s, %lld, "
                "%lld, %lld, %lld, %lld, "
                "%lld, %lld, "
                "%lld, %lld, "
                "%lld, %lld, %lld, %lld, "
                "%lld, %lld, %lld, %lld, "
                "%lld, %lld, %lld, %lld, "
                "%lld, %lld, %lld, %lld, "
                "%lld, %lld, %lld, %lld, "
                "%lld, %lld, %lld, %lld\n"_Stz,
               m_szStepNm,              // 0
               m_nInvoked,              // 1
               
               m_nSumCycles,            // 2
               nCyclesPerInvoke,        // 3
               nNanosecondsPerInvoke,   // 4
               nMaxNanosecondsForInvoke,// 5
               
               m_nSumMisses,            // 6
               m_nMaxMisses,            // 7

               m_nMaxAllocs,            // 8
               m_nMaxFrees,             // 9

               m_iHeapsAlloc[0],        // 10 p_tiny
               m_iHeapsAlloc[1],        // 11 p_small
               m_iHeapsAlloc[2],        // 12 p_medium
               m_iHeapsAlloc[3],        // 13 p_large

               m_iHeapsAlloc[4],        // 14 t_tiny
               m_iHeapsAlloc[5],        // 15 t_small
               m_iHeapsAlloc[6],        // 16 t_medium
               m_iHeapsAlloc[7],        // 17 t_large

               m_iHeapsAlloc[8],        // 18 v_tiny
               m_iHeapsAlloc[9],        // 19 v_small
               m_iHeapsAlloc[10],       // 20 v_medium
               m_iHeapsAlloc[11],       // 21 v_large

               m_iHeapsFree[0],         // 22 p_tiny
               m_iHeapsFree[1],         // 23 p_small
               m_iHeapsFree[2],         // 24 p_medium
               m_iHeapsFree[3],         // 25 p_large

               m_iHeapsFree[4],         // 26 t_tiny
               m_iHeapsFree[5],         // 27 t_small
               m_iHeapsFree[6],         // 28 t_medium
               m_iHeapsFree[7],         // 29 t_large

               m_iHeapsFree[8],         // 30 v_tiny
               m_iHeapsFree[9],         // 31 v_small
               m_iHeapsFree[10],        // 32 v_medium
               m_iHeapsFree[11]         // 33 v_large
               );
}

/*----------------------------------------------------------------------------*/
/* print                                                                      */
/*----------------------------------------------------------------------------*/
void StepAccum::vReset(void)
{
  m_nInvoked = 0;
  
  m_nSumCycles = 0;
  m_nMaxCycles = 0;
  m_nSumMisses = 0;
  m_nMaxMisses = 0;
}

/*----------------------------------------------------------------------------*/
/* print to given file                                                        */
/*----------------------------------------------------------------------------*/
void StepAccum::vShow(void)
{
  if (0 == m_nInvoked)
    return;
  
  i8 nCyclesPerGigaNanosecond = TickingDevice::iTicksPerSec();
  i8 nCyclesPerNanosecond = DIV(nCyclesPerGigaNanosecond,C_1G);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // 0) m_szStepNm
  
  // 1) m_nInvoked
  
  // 2) m_nSumCycles
  
  //    Cycles   Sum(Cycles)
  // 3) ------ = -----------
  //    Invoke   NoOfInvokes
  i8 nCyclesPerInvoke = DIV(m_nSumCycles,m_nInvoked);
  
  //               Cycles
  //               ------
  //      Ns       Invoke     Cycles     NS
  // 4) ------ = ---------- : ------ x ------
  //    Invoke     Cycles     Invoke   Cycles
  //               ------
  //                 Ns
  i8 nNanosecondsPerInvoke = DIV(nCyclesPerInvoke,nCyclesPerNanosecond);
  
  //       Max(Ns) for     Max(Cycles)    Max(Cycles)
  //        an Invoke  = ------------- : ----------- x Ns
  // 5)                     Cycles          Cycles
  //                        ------
  //                          Ns
  i8 nMaxNanosecondsForInvoke = DIV(m_nMaxCycles,nCyclesPerNanosecond);

  
  i8 nTotalTime = DIV(m_nSumCycles,nCyclesPerNanosecond);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  SP_Timer spTotal;
  SP_Timer spPerInvoke;
  SP_Timer spMaxInvoke;

  vTrace("%s: "
         "called %lld, total %s, "
         "avgPerCall %s, maxCall %s (%s)\n"_Stz,
         
          m_szStepNm,               // 0
         
          m_nInvoked,               // 1
          spTotal.sz(nTotalTime),   // 2
         
          spPerInvoke.sz(nNanosecondsPerInvoke),    // 3
          spMaxInvoke.sz(nMaxNanosecondsForInvoke), // 4
         
           AM::szBuild(),

          C_VARGS_END);

  vTrace("        %7s, %7s, %7s, %7s, %7s, %7s, %7s, %7s, %7s, %7s, %7s, %7s, \n"_Stz,
         "P_T", "P_S", "P_M", "P_L", "T_T", "T_S", "T_M", "T_L", "V_T", "V_S", "V_M", "V_L");

  vTrace("Allocs: %7lld, %7lld, %7lld, %7lld, %7lld, %7lld, %7lld, %7lld, %7lld, %7lld, %7lld, %7lld\n"_Stz,
         m_iHeapsAlloc[0], m_iHeapsAlloc[1], m_iHeapsAlloc[2], m_iHeapsAlloc[3],
         m_iHeapsAlloc[4], m_iHeapsAlloc[5], m_iHeapsAlloc[6], m_iHeapsAlloc[7],
         m_iHeapsAlloc[8], m_iHeapsAlloc[9], m_iHeapsAlloc[10], m_iHeapsAlloc[11]);

  vTrace("Frees:  %7lld, %7lld, %7lld, %7lld, %7lld, %7lld, %7lld, %7lld, %7lld, %7lld, %7lld, %7lld\n\n"_Stz,
         m_iHeapsFree[0], m_iHeapsFree[1], m_iHeapsFree[2], m_iHeapsFree[3],
         m_iHeapsFree[4], m_iHeapsFree[5], m_iHeapsFree[6], m_iHeapsFree[7],
         m_iHeapsFree[8], m_iHeapsFree[9], m_iHeapsFree[10], m_iHeapsFree[11]);

}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(StepAccum)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
