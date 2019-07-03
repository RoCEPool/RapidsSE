/*============================================================================*/
/* TspLib.cpp -- TimeStamp Library                                            */
/*============================================================================*/

#include "System.h"

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
  time_t iSecondsSincePosixEpoch = wIvl.xIvl.m_uSecond;
  struct tm* aLocalDateTime = localtime(&iSecondsSincePosixEpoch);
  if (! aLocalDateTime) {
    vWarn("wGetCurrTsp_local: localtime() error; errno=%d"_Stz,errno);
    wTsp.vSetNull();
    return wTsp;
  }
  
  wTsp.xTsp.m_uYearDelta = aLocalDateTime->tm_year;
  wTsp.xTsp.m_uMonth     = aLocalDateTime->tm_mon + 1;
  wTsp.xTsp.m_uDay       = aLocalDateTime->tm_mday;
  wTsp.xTsp.m_uHour      = aLocalDateTime->tm_hour;
  wTsp.xTsp.m_uMinute    = aLocalDateTime->tm_min;
  wTsp.xTsp.m_uSecond    = aLocalDateTime->tm_sec;
  wTsp.xTsp.m_uMicro     = wIvl.xIvl.m_uMicro;
  wTsp.xTsp.m_uSpare     = 0;
  
  return wTsp;
}

/*----------------------------------------------------------------------------*/
/* get current UTC timestamp                                                  */
/*----------------------------------------------------------------------------*/
Dtm TspLib::wGetCurrTsp_utc(void)
{
  Dtm wIvl;
  Dtm wTsp;
  
  // get current epoch interval
  wIvl = IvlLib::wGetCurrIvl();
  
  // convert current epoch interval into utc timestamp
  time_t iSecondsSincePosixEpoch = wIvl.xIvl.m_uSecond;
  struct tm* aUtcDateTime = gmtime(&iSecondsSincePosixEpoch);
  if (! aUtcDateTime) {
    vWarn("wGetCurrTsp_utc: gmtime() error; errno=%d"_Stz,errno);
    wTsp.vSetNull();
    return wTsp;
  }

  wTsp.xTsp.m_uYearDelta = aUtcDateTime->tm_year;
  wTsp.xTsp.m_uMonth     = aUtcDateTime->tm_mon + 1;
  wTsp.xTsp.m_uDay       = aUtcDateTime->tm_mday;
  wTsp.xTsp.m_uHour      = aUtcDateTime->tm_hour;
  wTsp.xTsp.m_uMinute    = aUtcDateTime->tm_min;
  wTsp.xTsp.m_uSecond    = aUtcDateTime->tm_sec;
  wTsp.xTsp.m_uMicro     = wIvl.xIvl.m_uMicro;
  wTsp.xTsp.m_uSpare     = 0;
  
  return wTsp;
}

/*----------------------------------------------------------------------------*/
/* convert interval to local timestamp                                        */
/*----------------------------------------------------------------------------*/
Dtm TspLib::wToTsp_local(Dtm wIvl)
{
  Dtm wDtm;
  
  time_t iSecondsSincePosixEpoch = wIvl.xIvl.m_uSecond;
  
  // convert secondsSince into a tm structure for local time zone
  struct tm* aLocalDateTime = localtime(&iSecondsSincePosixEpoch);

  if (! aLocalDateTime) {
    vWarn("wToTsp_local: localtime_r() error; errno=%d"_Stz,errno);
    wDtm.vSetNull();
    return wDtm;
  }
  
  wDtm.xTsp.m_uYearDelta = aLocalDateTime->tm_year;
  wDtm.xTsp.m_uMonth     = aLocalDateTime->tm_mon + 1;
  wDtm.xTsp.m_uDay       = aLocalDateTime->tm_mday;
  wDtm.xTsp.m_uHour      = aLocalDateTime->tm_hour;
  wDtm.xTsp.m_uMinute    = aLocalDateTime->tm_min;
  wDtm.xTsp.m_uSecond    = aLocalDateTime->tm_sec;
  wDtm.xTsp.m_uMicro     = wIvl.xIvl.m_uMicro;
  wDtm.xTsp.m_uSpare     = 0;

  return wDtm;
}

/*----------------------------------------------------------------------------*/
/* convert interval to utc timestamp                                          */
/*----------------------------------------------------------------------------*/
Dtm TspLib::wToTsp_utc(Dtm wIvl)
{
  Dtm wDtm;
  
  time_t iSecondsSincePosixEpoch = wIvl.xIvl.m_uSecond;
  
  // convert secondsSince into a tm structure for UTC/GMT time zone
  struct tm* aUtcDateTime = gmtime(&iSecondsSincePosixEpoch);
  
  if (! aUtcDateTime) {
    vWarn("wToTsp_utc: gmtime() error; errno=%d"_Stz,errno);
    wDtm.vSetNull();
    return wDtm;
  }

  wDtm.xTsp.m_uYearDelta = aUtcDateTime->tm_year;
  wDtm.xTsp.m_uMonth     = aUtcDateTime->tm_mon + 1;
  wDtm.xTsp.m_uDay       = aUtcDateTime->tm_mday;
  wDtm.xTsp.m_uHour      = aUtcDateTime->tm_hour;
  wDtm.xTsp.m_uMinute    = aUtcDateTime->tm_min;
  wDtm.xTsp.m_uSecond    = aUtcDateTime->tm_sec;
  wDtm.xTsp.m_uMicro     = wIvl.xIvl.m_uMicro;
  wDtm.xTsp.m_uSpare     = 0;

  return wDtm;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
