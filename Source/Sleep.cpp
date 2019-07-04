/*============================================================================*/
/* Sleep.cpp - how long to sleep for ?                                        */
/*============================================================================*/

#include "System.h"

#include "Pool.h"

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Sleep::Sleep(void)
: Object(CN_SINCE)
{
  _precondition_(_MAGIC_,CN_SINCE,_FOOTER_);
  
  m_wSleptTsp.m_iI8 = 0;
  m_wDurationIvl.m_iI8 = 0;
  m_wWakeupIvl.m_iI8 = 0;
  m_wWakeupTsp.m_iI8 = 0;
  
  _checkInitialised_(CN_SINCE);
  
  _postcondition_(_MAGIC_,CN_SINCE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Sleep::~Sleep(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_SINCE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* clear the sleep                                                            */
/*----------------------------------------------------------------------------*/
void Sleep::vClear(void)
{
  // reset slept tsp
  m_wSleptTsp.m_iI8 = 0;
  
  // reset the sleep interval
  m_wDurationIvl.m_iI8 = 0;
  
  // reset the wakeup Ivl
  m_wWakeupIvl.m_iI8 = 0;
  
  // reset the wakeup time
  m_wWakeupTsp.m_iI8 = 0;
}

/*----------------------------------------------------------------------------*/
/* sleep for microseconds                                                     */
/*----------------------------------------------------------------------------*/
void Sleep::vSleepFor(i8 iMicroseconds)
{
  // get current tsp as an ivl
  m_wSleptTsp = TspLib::wGetCurrTsp_local();
  
  // set the sleep duration as Ivl
  m_wDurationIvl = IvlLib::wToIvl(iMicroseconds);
  
  // get current tsp as ivl
  Dtm wCurrentIvl = IvlLib::wToIvl(m_wSleptTsp);
  Dtm wTempTsp = TspLib::wToTsp_local(wCurrentIvl);
  
  // calculate the wakeup time in Ivl
  m_wWakeupIvl = IvlLib::wAdd(wCurrentIvl, m_wDurationIvl);
  
  // set the wakeup time
  m_wWakeupTsp = TspLib::wToTsp_local(m_wWakeupIvl);
}

/*----------------------------------------------------------------------------*/
/* Time to wakeup ?                                                           */
/*----------------------------------------------------------------------------*/
bool Sleep::bWakeupNow(void)
{
  // get current time as an Ivl
  Dtm xCurrentIvl = IvlLib::wGetCurrIvl();
  
  // compare wakeup ivl with current ivl
  if (m_wWakeupIvl.m_iI8 <= xCurrentIvl.m_iI8)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* how long before task can be woken up (in microseconds)                     */
/*----------------------------------------------------------------------------*/
u8 Sleep::uWakeupInUsec(void)
{
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get current time as an Ivl
  Dtm xCurrentIvl = IvlLib::wGetCurrIvl();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  u8 uMicro = IvlLib::uSubtract(xCurrentIvl, m_wWakeupIvl);
  
  return uMicro;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Sleep::aFooter(void)
{
  _precondition_(_MAGIC_,CN_SINCE);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
