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
Dtm TspLib::wGetCurrTsp_local(void)
{
  Dtm wIvl;
  Dtm wTsp;
  
  // get current epoch interval
  wIvl = IvlLib::wGetCurrIvl();
  
  // convert current epoch interval into local timestamp
  time_t iSecondsSincePosixEpoch = wIvl.m_wIvl.m_uSecond;
  struct tm* aLocalDateTime = localtime(&iSecondsSincePosixEpoch);
  if (! aLocalDateTime) {
    vWarn("wGetCurrTsp_local: localtime() error; errno=%d"_Stz,errno);
    wTsp.vSetNull();
    return wTsp;
  }
  
  wTsp.m_wTsp.m_uYearDelta = aLocalDateTime->tm_year;
  wTsp.m_wTsp.m_uMonth     = aLocalDateTime->tm_mon + 1;
  wTsp.m_wTsp.m_uDay       = aLocalDateTime->tm_mday;
  wTsp.m_wTsp.m_uHour      = aLocalDateTime->tm_hour;
  wTsp.m_wTsp.m_uMinute    = aLocalDateTime->tm_min;
  wTsp.m_wTsp.m_uSecond    = aLocalDateTime->tm_sec;
  wTsp.m_wTsp.m_uMicro     = wIvl.m_wIvl.m_uMicro;
  wTsp.m_wTsp.m_uLocal     = C_TSP_LOCAL;
  wTsp.m_wTsp.m_uSpare     = 0;
  
  return wTsp;
}

/*----------------------------------------------------------------------------*/
/* get current UTC timestamp                                                  */
/*----------------------------------------------------------------------------*/
Dtm TspLib::wGetCurrTsp_utc(void)
{
  
  // currently we don't generate any UTC tsp's
  RASSERT(0);
  
  Dtm wIvl;
  Dtm wTsp;
  
  // get current epoch interval
  wIvl = IvlLib::wGetCurrIvl();
  
  // convert current epoch interval into utc timestamp
  time_t iSecondsSincePosixEpoch = wIvl.m_wIvl.m_uSecond;
  struct tm* aUtcDateTime = gmtime(&iSecondsSincePosixEpoch);
  if (! aUtcDateTime) {
    vWarn("wGetCurrTsp_utc: gmtime() error; errno=%d"_Stz,errno);
    wTsp.vSetNull();
    return wTsp;
  }

  wTsp.m_wTsp.m_uYearDelta = aUtcDateTime->tm_year;
  wTsp.m_wTsp.m_uMonth     = aUtcDateTime->tm_mon + 1;
  wTsp.m_wTsp.m_uDay       = aUtcDateTime->tm_mday;
  wTsp.m_wTsp.m_uHour      = aUtcDateTime->tm_hour;
  wTsp.m_wTsp.m_uMinute    = aUtcDateTime->tm_min;
  wTsp.m_wTsp.m_uSecond    = aUtcDateTime->tm_sec;
  wTsp.m_wTsp.m_uMicro     = wIvl.m_wIvl.m_uMicro;
  wTsp.m_wTsp.m_uLocal     = C_TSP_UTC;
  wTsp.m_wTsp.m_uSpare     = 0;
  
  return wTsp;
}

/*----------------------------------------------------------------------------*/
/* convert interval to local timestamp                                        */
/*----------------------------------------------------------------------------*/
Dtm TspLib::wToTsp_local(Ivl wIvl)
{
  Dtm wDtm;
  
  time_t iSecondsSincePosixEpoch = wIvl.m_uSecond;
  
  // convert secondsSince into a tm structure for local time zone
  struct tm* aLocalDateTime = localtime(&iSecondsSincePosixEpoch);

  if (! aLocalDateTime) {
    vWarn("wToTsp_local: localtime_r() error; errno=%d"_Stz,errno);
    wDtm.vSetNull();
    return wDtm;
  }
  
  wDtm.m_wTsp.m_uYearDelta = aLocalDateTime->tm_year;
  wDtm.m_wTsp.m_uMonth     = aLocalDateTime->tm_mon + 1;
  wDtm.m_wTsp.m_uDay       = aLocalDateTime->tm_mday;
  wDtm.m_wTsp.m_uHour      = aLocalDateTime->tm_hour;
  wDtm.m_wTsp.m_uMinute    = aLocalDateTime->tm_min;
  wDtm.m_wTsp.m_uSecond    = aLocalDateTime->tm_sec;
  wDtm.m_wTsp.m_uMicro     = wIvl.m_uMicro;
  wDtm.m_wTsp.m_uLocal     = C_TSP_LOCAL;
  wDtm.m_wTsp.m_uSpare     = 0;

  return wDtm;
}

/*----------------------------------------------------------------------------*/
/* convert interval to utc timestamp                                          */
/*----------------------------------------------------------------------------*/
Dtm TspLib::wToTsp_utc(Ivl wIvl)
{
  
  // currently we don't generate any UTC tsp's
  RASSERT(0);
  
  Dtm wDtm;
  
  time_t iSecondsSincePosixEpoch = wIvl.m_uSecond;
  
  // convert secondsSince into a tm structure for UTC/GMT time zone
  struct tm* aUtcDateTime = gmtime(&iSecondsSincePosixEpoch);
  
  if (! aUtcDateTime) {
    vWarn("wToTsp_utc: gmtime() error; errno=%d"_Stz,errno);
    wDtm.vSetNull();
    return wDtm;
  }

  wDtm.m_wTsp.m_uYearDelta = aUtcDateTime->tm_year;
  wDtm.m_wTsp.m_uMonth     = aUtcDateTime->tm_mon + 1;
  wDtm.m_wTsp.m_uDay       = aUtcDateTime->tm_mday;
  wDtm.m_wTsp.m_uHour      = aUtcDateTime->tm_hour;
  wDtm.m_wTsp.m_uMinute    = aUtcDateTime->tm_min;
  wDtm.m_wTsp.m_uSecond    = aUtcDateTime->tm_sec;
  wDtm.m_wTsp.m_uMicro     = wIvl.m_uMicro;
  wDtm.m_wTsp.m_uLocal     = C_TSP_UTC;
  wDtm.m_wTsp.m_uSpare     = 0;

  return wDtm;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
