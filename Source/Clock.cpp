/*============================================================================*/
/* Clock -- walltime clock type (seconds.microseconds)                        */
/*============================================================================*/

#include "Clock.hpp"

/*----------------------------------------------------------------------------*/
/* operator new: class cannot be new'd                                        */
/*----------------------------------------------------------------------------*/
void* Clock::operator new(size_t)
{
  vFatal("fatal: Clock class cannot be new'd");
  
  //vExitFatal();
  
  exit(EXIT_FAILURE);
}

/*----------------------------------------------------------------------------*/
/* Clock: constructor                                                         */
/*----------------------------------------------------------------------------*/
Clock::Clock()
{
  i8 i = sizeof(Time);
  ASSERT(sizeof(Time) == 8); // logic check
  
  vUpdate();
}

/*----------------------------------------------------------------------------*/
/* ~Clock: destructor                                                         */
/*----------------------------------------------------------------------------*/
Clock::~Clock()
{
}

/*----------------------------------------------------------------------------*/
/* vAdd: add nano-seconds                                                     */
/*----------------------------------------------------------------------------*/
void Clock::vAdd(i8 iNanoSeconds)
{
  ASSERT(iNanoSeconds < C_1Bn);
  
  // convert to microseconds
  i8 iMicroSeconds = DIV(iNanoSeconds,C_1K);
  
  // add microseconds to this clock
  m_iMicro += iMicroSeconds;
  
  // correct if we are >= a microseconds
  if (m_iMicro >= C_1M)
  {
    i8 iSeconds = DIV(iMicroSeconds,C_1M);
    m_iSeconds += iSeconds;
    
    m_iMicro = m_iMicro % C_1M;
  }
}

/*----------------------------------------------------------------------------*/
/* vUpdate: get current realtime clock                                        */
/*----------------------------------------------------------------------------*/
void Clock::vUpdate()
{
//  struct timeval xTimeVal;
//  //struct timezone xTimeZone;
//  
//  i8 rCode = gettimeofday(&xTimeVal, NULL);
//  
//  if (rCode)
//    vReportError("gettimeofday() error; errno=%d",errno);
//  
//  m_iSeconds = xTimeVal.tv_sec;
//  m_iMicro   = xTimeVal.tv_usec;
//  ASSERT(m_iMicro < 1000000); // logic check
  
  SysCall SysCall;
  Today::vGet(&m_iSeconds,&m_iMicro,SysCall);
}

/*----------------------------------------------------------------------------*/
/* iElapsed: get number of seconds since Lock last updated                    */
/*----------------------------------------------------------------------------*/
i8 Clock::iElapsed()
{
  struct timeval xTimeVal;
  
  i8 rCode = gettimeofday(&xTimeVal, NULL);
  
  if (rCode)
    vError("gettimeofday() error; errno=%d",errno);
  
  i8 iSeconds = xTimeVal.tv_sec - m_iSeconds;
  
  return iSeconds;
}

/*----------------------------------------------------------------------------*/
/* vSet: set realtime clock                                                   */
/*----------------------------------------------------------------------------*/
void Clock::vSet(Time* pTime)
{
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
  m_iMicro   = pTime->m_iMicro;
}

/*----------------------------------------------------------------------------*/
/* vToTime: get clock into given time                                         */
/*----------------------------------------------------------------------------*/
void Clock::vToTime(Time* pTime)
{
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
  pTime->m_iMicro  = (int)m_iMicro;
}

/*----------------------------------------------------------------------------*/
/* sToUTC: convert Lock to utc string                                         */
/*----------------------------------------------------------------------------*/
char* Clock::sToUTC(char* aWorkspace)
{
  Time xTime;
  
  vToTime(&xTime);
  
  char* z = aWorkspace;
  
  Time::vAppendUtc(&z,&xTime);
  
  return aWorkspace;
}

/*============================================================================*/
/* end of file                                                                */
/*============================================================================*/