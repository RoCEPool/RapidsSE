/*============================================================================*/
/* TspLib.cpp -- TimeStamp Library                                            */
/*============================================================================*/

#include "Library.h"

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get current local timestamp                                                */
/*----------------------------------------------------------------------------*/
Tsp TspLib::wGetCurrTsp_local(void)
{
  Tsp wIvl;
  Tsp wTsp;
  
  // get current epoch interval
  wIvl = IvlLib::wGetCurrIvl();
  
  // convert current epoch interval into local timestamp
  time_t iSecondsSincePosixEpoch = wIvl.m_uSecond;
  struct tm* aLocalDateTime = localtime(&iSecondsSincePosixEpoch);
  if (! aLocalDateTime) {
    vWarn("wGetCurrTsp_local: localtime() error; errno=%d"_Stz,errno);
    wTsp.vSetNull();
    return wTsp;
  }
  
  wTsp.m_uYearDelta = aLocalDateTime->tm_year;
  wTsp.m_uMonth     = aLocalDateTime->tm_mon + 1;
  wTsp.m_uDay       = aLocalDateTime->tm_mday;
  wTsp.m_uHour      = aLocalDateTime->tm_hour;
  wTsp.m_uMinute    = aLocalDateTime->tm_min;
  wTsp.m_uSecond    = aLocalDateTime->tm_sec;
  wTsp.m_uMicro     = wIvl.m_uMicro;
  wTsp.m_uLocal     = C_TSP_LOCAL;
  wTsp.m_uSpare     = 0;
  
  return wTsp;
}

/*----------------------------------------------------------------------------*/
/* get current UTC timestamp                                                  */
/*----------------------------------------------------------------------------*/
Tsp TspLib::wGetCurrTsp_utc(void)
{
  
  // currently we don't generate any UTC tsp's
  RASSERT(0);
  
  Ivl wIvl;
  Tsp wTsp;
  
  // get current epoch interval
  wIvl = IvlLib::wGetCurrIvl();
  
  // convert current epoch interval into utc timestamp
  time_t iSecondsSincePosixEpoch = wIvl.m_uSecond;
  struct tm* aUtcDateTime = gmtime(&iSecondsSincePosixEpoch);
  if (! aUtcDateTime) {
    vWarn("wGetCurrTsp_utc: gmtime() error; errno=%d"_Stz,errno);
    wTsp.vSetNull();
    return wTsp;
  }

  wTsp.m_uYearDelta = aUtcDateTime->tm_year;
  wTsp.m_uMonth     = aUtcDateTime->tm_mon + 1;
  wTsp.m_uDay       = aUtcDateTime->tm_mday;
  wTsp.m_uHour      = aUtcDateTime->tm_hour;
  wTsp.m_uMinute    = aUtcDateTime->tm_min;
  wTsp.m_uSecond    = aUtcDateTime->tm_sec;
  wTsp.m_uMicro     = wIvl.m_uMicro;
  wTsp.m_uLocal     = C_TSP_UTC;
  wTsp.m_uSpare     = 0;
  
  return wTsp;
}

/*----------------------------------------------------------------------------*/
/* convert interval to local timestamp                                        */
/*----------------------------------------------------------------------------*/
Tsp TspLib::wToTsp_local(Ivl wIvl)
{
  Tsp wTsp;
  
  time_t iSecondsSincePosixEpoch = wIvl.m_uSecond;
  
  // convert secondsSince into a tm structure for local time zone
  struct tm* aLocalDateTime = localtime(&iSecondsSincePosixEpoch);

  if (! aLocalDateTime) {
    vWarn("wToTsp_local: localtime_r() error; errno=%d"_Stz,errno);
    wTsp.vSetNull();
    return wTsp;
  }
  
  wTsp.m_uYearDelta = aLocalDateTime->tm_year;
  wTsp.m_uMonth     = aLocalDateTime->tm_mon + 1;
  wTsp.m_uDay       = aLocalDateTime->tm_mday;
  wTsp.m_uHour      = aLocalDateTime->tm_hour;
  wTsp.m_uMinute    = aLocalDateTime->tm_min;
  wTsp.m_uSecond    = aLocalDateTime->tm_sec;
  wTsp.m_uMicro     = wIvl.m_uMicro;
  wTsp.m_uLocal     = C_TSP_LOCAL;
  wTsp.m_uSpare     = 0;

  return wTsp;
}

/*----------------------------------------------------------------------------*/
/* convert interval to utc timestamp                                          */
/*----------------------------------------------------------------------------*/
Tsp TspLib::wToTsp_utc(Ivl wIvl)
{
  
  // currently we don't generate any UTC tsp's
  RASSERT(0);
  
  Tsp wTsp;
  
  time_t iSecondsSincePosixEpoch = wIvl.m_uSecond;
  
  // convert secondsSince into a tm structure for UTC/GMT time zone
  struct tm* aUtcDateTime = gmtime(&iSecondsSincePosixEpoch);
  
  if (! aUtcDateTime) {
    vWarn("wToTsp_utc: gmtime() error; errno=%d"_Stz,errno);
    wTsp.vSetNull();
    return wTsp;
  }

  wTsp.m_uYearDelta = aUtcDateTime->tm_year;
  wTsp.m_uMonth     = aUtcDateTime->tm_mon + 1;
  wTsp.m_uDay       = aUtcDateTime->tm_mday;
  wTsp.m_uHour      = aUtcDateTime->tm_hour;
  wTsp.m_uMinute    = aUtcDateTime->tm_min;
  wTsp.m_uSecond    = aUtcDateTime->tm_sec;
  wTsp.m_uMicro     = wIvl.m_uMicro;
  wTsp.m_uLocal     = C_TSP_UTC;
  wTsp.m_uSpare     = 0;

  return wTsp;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
