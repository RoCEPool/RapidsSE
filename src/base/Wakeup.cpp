/*============================================================================*/
/* Wakeup.cpp - when to be woken up ?                                         */
/*============================================================================*/

#include "BASE.h"
#include "eIV.h"

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Wakeup::Wakeup(void)
: Object(CN_WAKEUP),
  m_wFrom(IV_SECOND),
  m_wDuration(IV_SECOND),
  m_wWakeup(IV_SECOND)
{
  _precondition_(_MAGIC_,CN_WAKEUP,_FOOTER_);
  
  _checkInitialised_(CN_WAKEUP);
  
  _postcondition_(_MAGIC_,CN_WAKEUP,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Wakeup::~Wakeup(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_WAKEUP,_FOOTER_);
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
void Wakeup::vClear(void)
{
  _precondition_(_MAGIC_,CN_WAKEUP,_FOOTER_);
  
  m_wFrom     = 0;
  m_wDuration = 0;
  m_wWakeup   = 0;
  
}

/*----------------------------------------------------------------------------*/
/* sleep for microseconds                                                     */
/*----------------------------------------------------------------------------*/
void Wakeup::vSleepFor(i8 iMicroseconds)
{
  _precondition_(_MAGIC_,CN_WAKEUP,_FOOTER_);
  
  // get this point in ivl time
  m_wFrom = IvlLib::wGetCurrIvl();;
  
  // set the duration of the sleep as ivl time
  m_wDuration = IvlLib::wToIvl(iMicroseconds);
  
  // calculate the wakeup point
  m_wWakeup = IvlLib::wAdd(m_wFrom, m_wDuration);
}

/*----------------------------------------------------------------------------*/
/* how long before task can be woken up (in microseconds)                     */
/*----------------------------------------------------------------------------*/
u8 Wakeup::uWakeupInUsec(void)
{
  _precondition_(_MAGIC_,CN_WAKEUP,_FOOTER_);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get current time as an Ivl
  Ivl xCurrentIvl = IvlLib::wGetCurrIvl();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // subtract wkaeupIvl from CurrentIvl
  u8 uMicro = IvlLib::uSubtract(xCurrentIvl, m_wWakeup);
  
  return uMicro;
}

/*----------------------------------------------------------------------------*/
/* Time to wakeup ?                                                           */
/*----------------------------------------------------------------------------*/
bool Wakeup::bWakeupNow(void)
{
  _precondition_(_MAGIC_,CN_WAKEUP,_FOOTER_);
  
  // get current time as an Ivl
  Ivl xCurrentIvl = IvlLib::wGetCurrIvl();
  
  // compare wakeup ivl with current ivl
  if (m_wWakeup.T.m_iAsI8 <= xCurrentIvl.T.m_iAsI8)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(Wakeup);


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
