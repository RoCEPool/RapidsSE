/*============================================================================*/
/* Since.cpp - seconds.microseconds since January 1, 1970                     */
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
  
  i8 iMicroseconds = m_iSeconds*C_1M + m_iMicroseconds;
  
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
void Since::vSet(Time* pTime)
{
  _precondition_(_MAGIC_,CN_SINCE,_FOOTER_);
  
  struct tm xTM;
  
  xTM.tm_year   = pTime->m_iYear;
  xTM.tm_mon    = pTime->m_iMonth-1;
  xTM.tm_mday   = pTime->m_iDay;
  xTM.tm_hour   = pTime->m_iHour;
  xTM.tm_min    = pTime->m_iMinute;
  xTM.tm_sec    = pTime->m_iSecond;
  xTM.tm_isdst  = 0;
  xTM.tm_gmtoff = 0;
  xTM.tm_wday   = 0;
  xTM.tm_yday   = 0;
  xTM.tm_zone   = nullptr;
  
  m_iSeconds = timegm(&xTM);
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
Ivl Since::wSubtract(Since& oArg)
{
  _precondition_(_MAGIC_,CN_SINCE,_FOOTER_);
  
  Ivl wDiff;
  
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
  wDiff.vSet(iSeconds,iMilliseconds);

  return wDiff;
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
  
  struct tm xTimeResult;
  
  time_t iSeconds = m_iSeconds;
  
  void* aTimeResult = gmtime_r(&iSeconds,&xTimeResult);
  
  if ( ! aTimeResult )
    vError("gmtime_r() error; errno=%d"_Stz,errno);
  
  pTime->m_iYear   = xTimeResult.tm_year;
  pTime->m_iMonth  = xTimeResult.tm_mon+1;
  pTime->m_iDay    = xTimeResult.tm_mday;
  pTime->m_iHour   = xTimeResult.tm_hour;
  pTime->m_iMinute = xTimeResult.tm_min;
  pTime->m_iSecond = xTimeResult.tm_sec;
  pTime->m_iMicro  = (int)m_iMicroseconds;
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get time of day (current realtime clock)                                   */
/*----------------------------------------------------------------------------*/
void Since::vGetTimeOfDay(i8& iSeconds, i8& iMicroseconds, SysCall& oSysCall)
{
  struct timeval xTimeVal;
  
  oSysCall.vClear();
  
  i8 rCode = gettimeofday(&xTimeVal,NULL);
  
  if (rCode) {
    oSysCall.vError(errno);
    vError("gettimeofday() error; errno=%d"_Stz,errno);
    return;
  }
  
  oSysCall.vSuccess();
  
  iSeconds      = xTimeVal.tv_sec;
  iMicroseconds = xTimeVal.tv_usec;
  
  ASSERT(iMicroseconds < C_1M); // logic check
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
