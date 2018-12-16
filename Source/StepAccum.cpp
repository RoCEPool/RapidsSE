/*============================================================================*/
/* StepAccum.cpp -- Step Accumulator                                          */
/*============================================================================*/

#include "Tasks.h"

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
StepAccum::StepAccum(void)
: Object(CN_STEPACCUM)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STEPACCUM,_FOOTER_);
  
  m_aStepNm  = nullptr;
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
/* print                                                                      */
/*----------------------------------------------------------------------------*/
void StepAccum::vPrint(File& xFile)
{
  if (0 == m_nInvoked)
    return;
  
  i8 iTicksPerSecond = Synch::iTickFrequency();
  i8 iTicksPerNanos  = DIV(iTicksPerSecond,1000000000);
  
  i8 iStepNanos      = DIV(m_nSumCycles,iTicksPerNanos);
  i8 iTicksPerInvoke = DIV(m_nSumCycles,m_nInvoked);
  
  i8 iInvokeNanos    = DIV(iTicksPerInvoke,iTicksPerNanos);

  
//  vTrace("  %24s %12lld %12lld %8lldns\n"_Stz,
//         m_aStepNm,
//         m_nInvoked,
//         iStepNanos,
//         iInvokeNanos
//         );
  
  xFile.vPrint("%s, %lld, %lld, %lld\n"_Stz,
         m_aStepNm,
         m_nInvoked,
         iStepNanos,
         iInvokeNanos
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
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer*StepAccum::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STEPACCUM);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
