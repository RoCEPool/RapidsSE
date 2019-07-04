/*============================================================================*/
/* Birth.cpp - seconds&microseconds since Posix Epoch January 1, 1970         */
/*============================================================================*/

#include "System.h"

#include "Pool.h"

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Birth::Birth(void)
: Object(CN_BIRTH)
{
  _precondition_(_MAGIC_,CN_BIRTH,_FOOTER_);
  
  m_wTsp.m_iI8 = 0;
  m_wIvl.m_iI8 = 0;
  
  _checkInitialised_(CN_BIRTH);
  
  _postcondition_(_MAGIC_,CN_BIRTH,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Birth::~Birth(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_BIRTH,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get time of day (current realtime clock)                                   */
/*----------------------------------------------------------------------------*/
void Birth::vGetTimeOfDay(i8& iSeconds, i8& iMicroseconds, SysCall& oSysCall)
{
  struct timeval xTimeVal;
  
  oSysCall.vClear();
  
  // get seconds&microseconds sine Posix Epoch January 1, 1970
  i8 rCode = gettimeofday(&xTimeVal,NULL);
  
  if (rCode) {
    oSysCall.vError(errno);
    vWarn("gettimeofday() error; errno=%d"_Stz,errno);
    return;
  }
  
  oSysCall.vSuccess();
  
  iSeconds      = xTimeVal.tv_sec;
  iMicroseconds = xTimeVal.tv_usec;
  
  ASSERT(iMicroseconds < C_1M); // logic check
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* set this Birth as at now                                                   */
/*----------------------------------------------------------------------------*/
void Birth::vNow(void)
{
  _precondition_(_MAGIC_,CN_BIRTH,_FOOTER_);
  
  // get current tsp
  m_wTsp = TspLib::wGetCurrTsp_local();
  
  m_wIvl = IvlLib::wToIvl(m_wTsp);
}

/*----------------------------------------------------------------------------*/
/* get seconds elapsed since birth                                            */
/*----------------------------------------------------------------------------*/
u8 Birth::uSecondsElapsed(void)
{
  _precondition_(_MAGIC_,CN_BIRTH,_FOOTER_);
  
  // get current tsp as Ivl
  Dtm wCurrentIvl = IvlLib::wGetCurrIvl();
  
  u8 uMicros = IvlLib::uSubtract(wCurrentIvl, m_wIvl);
  
  return uMicros/C_1M;
  
}

///*----------------------------------------------------------------------------*/
///* get number of seconds                                                      */
///*----------------------------------------------------------------------------*/
//i8 Birth::iSeconds()
//{
//  _precondition_(_MAGIC_,CN_BIRTH,_FOOTER_);
//  
//  return (i8)m_wIvl.xIvl.m_uSecond;
//}
//
///*----------------------------------------------------------------------------*/
///* get number of microseconds                                                 */
///*----------------------------------------------------------------------------*/
//i8 Birth::iMicroseconds()
//{
//  _precondition_(_MAGIC_,CN_BIRTH,_FOOTER_);
//  
//  return (i8)m_wIvl.xIvl.m_uMicro;
//}

/*----------------------------------------------------------------------------*/
/* vToTime: get clock into given time                                         */
/*----------------------------------------------------------------------------*/
//void Birth::vToTime(Time* pTime)
//{
//  _precondition_(_MAGIC_,CN_BIRTH,_FOOTER_);
//  
//  time_t iSecondsBirthPosixEpoch = m_iSeconds;
//  
//  // convert secondsBirth into a tm structure for UTC/GMT time zone
//  struct tm* aCalendarDateTime_UTC = gmtime(&iSecondsBirthPosixEpoch);
//
//  if ( ! aCalendarDateTime_UTC )
//    vWarn("gmtime() error; errno=%d"_Stz,errno);
//  
//  pTime->m_iYearDelta = aCalendarDateTime_UTC->tm_year;
//  pTime->m_iMonth     = aCalendarDateTime_UTC->tm_mon + 1;
//  pTime->m_iDay       = aCalendarDateTime_UTC->tm_mday;
//  pTime->m_iHour      = aCalendarDateTime_UTC->tm_hour;
//  pTime->m_iMinute    = aCalendarDateTime_UTC->tm_min;
//  pTime->m_iSecond    = aCalendarDateTime_UTC->tm_sec;
//  pTime->m_iMicro     = (int)m_iMicroseconds;
//}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Birth::aFooter(void)
{
  _precondition_(_MAGIC_,CN_BIRTH);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
