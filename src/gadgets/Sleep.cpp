/*============================================================================*/
/* Sleep.cpp - how long to sleep for ?                                        */
/*============================================================================*/

#include "Resources.h"

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Sleep::Sleep(void)
: Object(CN_SLEEP)
{
  _precondition_(_MAGIC_,CN_SLEEP,_FOOTER_);
  
  m_wSleptIvl    = 0;
  m_wWakeupIvl   = 0;
  m_wDurationIvl = 0;
  
  _checkInitialised_(CN_SLEEP);
  
  _postcondition_(_MAGIC_,CN_SLEEP,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Sleep::~Sleep(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_SLEEP,_FOOTER_);
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
  _precondition_(_MAGIC_,CN_SLEEP,_FOOTER_);
  
  // reset slept interval
  m_wSleptIvl = 0;
  
  // reset the wakeup interval
  m_wWakeupIvl = 0;
  
  // reset the sleep interval
  m_wDurationIvl = 0;
}

/*----------------------------------------------------------------------------*/
/* sleep for microseconds                                                     */
/*----------------------------------------------------------------------------*/
void Sleep::vSleepFor(i8 iMicroseconds)
{
  _precondition_(_MAGIC_,CN_SLEEP,_FOOTER_);
  
  // get current ivlwhen slept
  m_wSleptIvl = IvlLib::wGetCurrIvl();;
  
  // set the sleep duration as Ivl
  m_wDurationIvl = IvlLib::wToIvl(iMicroseconds);
  
  // calculate the wakeup time in Ivl
  m_wWakeupIvl = IvlLib::wAdd(m_wSleptIvl, m_wDurationIvl);
}

/*----------------------------------------------------------------------------*/
/* how long before task can be woken up (in microseconds)                     */
/*----------------------------------------------------------------------------*/
u8 Sleep::uWakeupInUsec(void)
{
  _precondition_(_MAGIC_,CN_SLEEP,_FOOTER_);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get current time as an Ivl
  Ivl xCurrentIvl = IvlLib::wGetCurrIvl();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // subtract wkaeupIvl from CurrentIvl
  u8 uMicro = IvlLib::uSubtract(xCurrentIvl, m_wWakeupIvl);
  
  return uMicro;
}

/*----------------------------------------------------------------------------*/
/* Time to wakeup ?                                                           */
/*----------------------------------------------------------------------------*/
bool Sleep::bWakeupNow(void)
{
  _precondition_(_MAGIC_,CN_SLEEP,_FOOTER_);
  
  // get current time as an Ivl
  Ivl xCurrentIvl = IvlLib::wGetCurrIvl();
  
  // compare wakeup ivl with current ivl
  if (m_wWakeupIvl.m_iAsI8 <= xCurrentIvl.m_iAsI8)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(Sleep);


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
