/*============================================================================*/
/* Since.cpp - seconds&microseconds since Posix Epoch January 1, 1970         */
/*============================================================================*/

#include "System.h"

#include "Pool.h"

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Since::Since(void)
: Object(CN_SINCE)
{
  _precondition_(_MAGIC_,CN_SINCE,_FOOTER_);
  
  m_iSeconds      = 0;
  m_iMicroseconds = 0;
  
  _checkInitialised_(CN_SINCE);
  
  _postcondition_(_MAGIC_,CN_SINCE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Since::~Since(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_SINCE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get time of day (current realtime clock)                                   */
/*----------------------------------------------------------------------------*/
void Since::vGetTimeOfDay(i8& iSeconds, i8& iMicroseconds, SysCall& oSysCall)
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
/* add                                                                        */
/*----------------------------------------------------------------------------*/
void Since::vAdd(i8 iMicroseconds)
{
  _precondition_(_MAGIC_,CN_SINCE,_FOOTER_);
  
  m_iMicroseconds += iMicroseconds;
  
  if (m_iMicroseconds < C_1M)
    return;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // correction needed
  
  i8 iMultiples = m_iMicroseconds;
  
  m_iMicroseconds = iMultiples % C_1M;
  
  i8 iSecondsDelta = iMultiples / C_1M;
  
  m_iSeconds += iSecondsDelta;
}

/*----------------------------------------------------------------------------*/
/* get delta between now and this since                                       */
/*----------------------------------------------------------------------------*/
i8 Since::iDeltaTillThenMicroseconds(void)
{
  i8 iThisSinceUs = iGetAsMicroseconds();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Since oNow;
  oNow.vNow();

  i8 iNowUs = oNow.iGetAsMicroseconds();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  i8 iDeltaUs = iThisSinceUs - iNowUs;
  
  return iDeltaUs;
}

/*----------------------------------------------------------------------------*/
/* get delta between now and this since                                       */
/*----------------------------------------------------------------------------*/
i8 Since::iDeltaFromThenSeconds(void)
{
  i8 iBegan = iSeconds();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Since oNow;
  oNow.vNow();
  
  i8 iNowSeconds = oNow.iSeconds();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  i8 iDeltaSeconds = iNowSeconds - iBegan;
  
  return iDeltaSeconds;
}

/*----------------------------------------------------------------------------*/
/* formatted                                                                  */
/*----------------------------------------------------------------------------*/
Stz* Since::sFormatted(void)
{
  _precondition_(_MAGIC_,CN_SINCE,_FOOTER_);
  
  Time oDateTime;
  oDateTime.vSetLocal(this);
  
  char* z = m_sFormatted.aChar();
  oDateTime.vAppendLocal(&z);

  return m_sFormatted.aStz();
}

/*----------------------------------------------------------------------------*/
/* get                                                                        */
/*----------------------------------------------------------------------------*/
void Since::vGet(i8& iSeconds, i8& iMicroseconds)
{
  _precondition_(_MAGIC_,CN_SINCE,_FOOTER_);
  
  iSeconds      = m_iSeconds;
  iMicroseconds = m_iMicroseconds;
}

/*----------------------------------------------------------------------------*/
/* get                                                                        */
/*----------------------------------------------------------------------------*/
i8 Since::iGetAsMicroseconds(void)
{
  _precondition_(_MAGIC_,CN_SINCE,_FOOTER_);
  
  i8 iMicroseconds = (m_iSeconds * C_1M) + m_iMicroseconds;
  
  return iMicroseconds;
}

/*----------------------------------------------------------------------------*/
/* set this since as at now                                                   */
/*----------------------------------------------------------------------------*/
void Since::vNow(void)
{
  _precondition_(_MAGIC_,CN_SINCE,_FOOTER_);
  
  SysCall oSysCall;
  
  vGetTimeOfDay(m_iSeconds,m_iMicroseconds,oSysCall);
}

/*----------------------------------------------------------------------------*/
/* vSet: set realtime clock                                                   */
/*----------------------------------------------------------------------------*/
void Since::vSetUTC(Time* pTime)
{
  _precondition_(_MAGIC_,CN_SINCE,_FOOTER_);
  
  struct tm xCalendarDateTime_UTC;
  
  xCalendarDateTime_UTC.tm_year   = pTime->m_iYearDelta;
  xCalendarDateTime_UTC.tm_mon    = pTime->m_iMonth - 1;
  xCalendarDateTime_UTC.tm_mday   = pTime->m_iDay;
  xCalendarDateTime_UTC.tm_hour   = pTime->m_iHour;
  xCalendarDateTime_UTC.tm_min    = pTime->m_iMinute;
  xCalendarDateTime_UTC.tm_sec    = pTime->m_iSecond;
  xCalendarDateTime_UTC.tm_isdst  = 0;
  xCalendarDateTime_UTC.tm_gmtoff = 0;
  xCalendarDateTime_UTC.tm_wday   = 0;
  xCalendarDateTime_UTC.tm_yday   = 0;
  xCalendarDateTime_UTC.tm_zone   = nullptr;
  
  // convert UTC:date&time into seconds since Posix Epoch
  m_iSeconds = timegm(&xCalendarDateTime_UTC);
  
  m_iMicroseconds = pTime->m_iMicro;
}

/*----------------------------------------------------------------------------*/
/* set                                                                        */
/*----------------------------------------------------------------------------*/
void Since::vSet(i8 iSeconds, i8 iMicroseconds)
{
  _precondition_(_MAGIC_,CN_SINCE,_FOOTER_);
  
  ASSERT(iMicroseconds < C_1M);

  m_iSeconds    = iSeconds;
  iMicroseconds = iMicroseconds;
}

/*----------------------------------------------------------------------------*/
/* subtract given since argument                                              */
/*----------------------------------------------------------------------------*/
i8 Since::iSubtract(Since& oArg)
{
  _precondition_(_MAGIC_,CN_SINCE,_FOOTER_);
    
  Since* pGreater;
  Since* pSmaller;

  i8 iSeconds;
  i8 iMicroseconds;
  i8 iMilliseconds;
  
  if (m_iSeconds < oArg.m_iSeconds ||
       (m_iSeconds == oArg.m_iSeconds &&
        m_iMicroseconds < oArg.m_iMicroseconds)) {
    pGreater = &oArg;
    pSmaller = this;
  }
  else {
    pGreater = this;
    pSmaller = &oArg;
  }
  
  if (pSmaller->m_iMicroseconds > pGreater->m_iMicroseconds) {
    iMicroseconds = 1000000 + pGreater->m_iMicroseconds - pSmaller->m_iMicroseconds;
    iSeconds = pGreater->m_iSeconds - 1 - pSmaller->m_iSeconds;
  }
  else {
    iMicroseconds = pGreater->m_iMicroseconds - pSmaller->m_iMicroseconds;
    iSeconds = pGreater->m_iSeconds - pSmaller->m_iSeconds;
  }
  
  iMilliseconds = iMicroseconds / C_1K;

  return iSeconds;
}

/*----------------------------------------------------------------------------*/
/* get number of seconds                                                      */
/*----------------------------------------------------------------------------*/
i8 Since::iSeconds()
{
  _precondition_(_MAGIC_,CN_SINCE,_FOOTER_);
  
  return m_iSeconds;
}

/*----------------------------------------------------------------------------*/
/* get number of microseconds                                                 */
/*----------------------------------------------------------------------------*/
i8 Since::iMicroseconds()
{
  _precondition_(_MAGIC_,CN_SINCE,_FOOTER_);
  
  return m_iMicroseconds;
}

/*----------------------------------------------------------------------------*/
/* vToTime: get clock into given time                                         */
/*----------------------------------------------------------------------------*/
void Since::vToTime(Time* pTime)
{
  _precondition_(_MAGIC_,CN_SINCE,_FOOTER_);
  
  time_t iSecondsSincePosixEpoch = m_iSeconds;
  
  // convert secondsSince into a tm structure for UTC/GMT time zone
  struct tm* aCalendarDateTime_UTC = gmtime(&iSecondsSincePosixEpoch);

  if ( ! aCalendarDateTime_UTC )
    vWarn("gmtime() error; errno=%d"_Stz,errno);
  
  pTime->m_iYearDelta = aCalendarDateTime_UTC->tm_year;
  pTime->m_iMonth     = aCalendarDateTime_UTC->tm_mon + 1;
  pTime->m_iDay       = aCalendarDateTime_UTC->tm_mday;
  pTime->m_iHour      = aCalendarDateTime_UTC->tm_hour;
  pTime->m_iMinute    = aCalendarDateTime_UTC->tm_min;
  pTime->m_iSecond    = aCalendarDateTime_UTC->tm_sec;
  pTime->m_iMicro     = (int)m_iMicroseconds;
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Since::aFooter(void)
{
  _precondition_(_MAGIC_,CN_SINCE);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
