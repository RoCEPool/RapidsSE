/*============================================================================*/
/* StepAccum.cpp -- Step Accumulator                                          */
/*============================================================================*/

#include "Steps.h"

#include "File.hpp" //?

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
#endif
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

//            0    1      2      3       4     5     6     7
//  vTrace("  %24s %12lld %12lld %8lldns %8lld %8lld %8lld %8lld\n"_Stz,
//          m_szStepNm,               // 0
//          m_nInvoked,              // 1
//
//          m_nSumCycles,            // 2
//          nCyclesPerInvoke,        // 3
//          nNanosecondsPerInvoke,   // 4
//          nMaxNanosecondsForInvoke,// 5
//
//          m_nSumMisses,            // 6
//          m_nMaxMisses             // 7
//          );

  //            0   1     2     3     4     5     6     7
  pFile->vPrint("%s, %lld, %lld, %lld, %lld, %lld, %lld, %lld\n"_Stz,
               m_szStepNm,               // 0
               m_nInvoked,              // 1
               
               m_nSumCycles,            // 2
               nCyclesPerInvoke,        // 3
               nNanosecondsPerInvoke,   // 4
               nMaxNanosecondsForInvoke,// 5
               
               m_nSumMisses,            // 6
               m_nMaxMisses             // 7
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
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(StepAccum)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
