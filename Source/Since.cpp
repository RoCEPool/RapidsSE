/*============================================================================*/
/* Since.cpp - seconds.microseconds since January 1, 1970                     */
/*============================================================================*/

#include "Common.h"

#include "Pool.h"

extern BufferPool* gpBufferPool;

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Since::Since(void)
: Header(CN_SINCE)
{
  _precondition_(_MAGIC_,CN_SINCE,_FOOTER_);
  
  m_iSeconds      = 0;
  m_iMicroseconds = 0;
  
  Memory::vCheckInitialised(this,sizeof(Since));
  
  _postcondition_(_MAGIC_,CN_SINCE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Since::~Since(void)
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
void Since::vSet(void)
{
  _precondition_(_MAGIC_,CN_SINCE,_FOOTER_);
  
  SysCall xSysCall;
  
  vGetTimeOfDay(m_iSeconds,m_iMicroseconds,xSysCall);
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
  
  void* pTimeResult = gmtime_r(&iSeconds,&xTimeResult);
  
  if ( ! pTimeResult )
    vError("gmtime_r() error; errno=%d",errno);
  
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
void Since::vGetTimeOfDay(i8& iSeconds, i8& iMicroseconds, SysCall& xSysCall)
{
  struct timeval xTimeVal;
  
  xSysCall.vClear();
  
  i8 rCode = gettimeofday(&xTimeVal,NULL);
  
  if (rCode) {
    xSysCall.vError(errno);
    vError("gettimeofday() error; errno=%d",errno);
    return;
  }
  
  xSysCall.vSuccess();
  
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
Footer* Since::pFooter(void)
{
  _precondition_(_MAGIC_,CN_SINCE);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/