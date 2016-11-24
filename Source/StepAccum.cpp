/*============================================================================*/
/* StepAccum.cpp -- Step Accumulator                                          */
/*============================================================================*/

#include "Tasks.h"

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
StepAccum::StepAccum(void)
: Header(CN_STEPACCUM)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STEPACCUM,_FOOTER_);
  
  m_aFnName  = nullptr;
  m_nInvoked = 0;
  m_nCycles  = 0;
  
  _checkInitialised_(CN_STEPACCUM);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_STEPACCUM,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
StepAccum::~StepAccum(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STEPACCUM,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* print                                                                      */
/*----------------------------------------------------------------------------*/
void StepAccum::vPrint(void)
{
  if (0 == m_nInvoked)
    return;
  
  i8 iTicksPerSecond = Synch::iTickFrequency();
  i8 iTicksPerNanos = DIV(iTicksPerSecond,1000000000);
  
  i8 iTicksPerInvoke = DIV(m_nCycles,m_nInvoked);
  
  i8 iInvokeMircos = DIV(iTicksPerInvoke,iTicksPerNanos);

  
  vTrace("  %20s %12lld %12lld %8lldns\n",
         m_aFnName,
         m_nInvoked,
         m_nCycles,
         iInvokeMircos
         );
}

/*----------------------------------------------------------------------------*/
/* print                                                                      */
/*----------------------------------------------------------------------------*/
void StepAccum::vReset(void)
{
  m_nInvoked = 0;
  m_nCycles  = 0;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer*StepAccum::pFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STEPACCUM);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
