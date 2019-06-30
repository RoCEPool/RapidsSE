/*============================================================================*/
/* TspLib.cpp -- TimeStamp Library                                            */
/*============================================================================*/

#include "System.h"

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* set time using since                                                       */
/*----------------------------------------------------------------------------*/
Dtm TspLib::wToLocal(Since* pSincePosixEpoch)
{
  time_t iSecondsSincePosixEpoch = pSincePosixEpoch->iSeconds();
  
  // convert secondsSince into a tm structure for local time zone
  struct tm* aLocalDateTime = localtime(&iSecondsSincePosixEpoch);

  if (! aLocalDateTime)
    vWarn("localtime_r() error; errno=%d"_Stz,errno);
  
  Dtm wDtm;
  
  wDtm.xTsp.m_uYearDelta = aLocalDateTime->tm_year;
  wDtm.xTsp.m_uMonth     = aLocalDateTime->tm_mon + 1;
  wDtm.xTsp.m_uDay       = aLocalDateTime->tm_mday;
  wDtm.xTsp.m_uHour      = aLocalDateTime->tm_hour;
  wDtm.xTsp.m_uMinute    = aLocalDateTime->tm_min;
  wDtm.xTsp.m_uSecond    = aLocalDateTime->tm_sec;
  wDtm.xTsp.m_uMicro     = (int)pSincePosixEpoch->iMicroseconds();
  wDtm.xTsp.m_uSpare     = 0;

  return wDtm;
}

/*----------------------------------------------------------------------------*/
/* vSet: set time using since                                                 */
/*----------------------------------------------------------------------------*/
Dtm TspLib::wToUTC(Since* pSincePosixEpoch)
{
  time_t iSecondsSincePosixEpoch = pSincePosixEpoch->iSeconds();
  
  // convert secondsSince into a tm structure for UTC/GMT time zone
  struct tm* aUtcDateTime = gmtime(&iSecondsSincePosixEpoch);
  
  if (! aUtcDateTime)
    vWarn("gmtime() error; errno=%d"_Stz,errno);
  
  Dtm wDtm;

  wDtm.xTsp.m_uYearDelta = aUtcDateTime->tm_year;
  wDtm.xTsp.m_uMonth     = aUtcDateTime->tm_mon + 1;
  wDtm.xTsp.m_uDay       = aUtcDateTime->tm_mday;
  wDtm.xTsp.m_uHour      = aUtcDateTime->tm_hour;
  wDtm.xTsp.m_uMinute    = aUtcDateTime->tm_min;
  wDtm.xTsp.m_uSecond    = aUtcDateTime->tm_sec;
  wDtm.xTsp.m_uMicro     = (int)pSincePosixEpoch->iMicroseconds();
  wDtm.xTsp.m_uSpare     = 0;

  return wDtm;
}

/*----------------------------------------------------------------------------*/
/* set as current local time                                                  */
/*----------------------------------------------------------------------------*/
Dtm TspLib::wGetLocalNow(void)
{
  // get current epoch time
  time_t iSecondsSincePosixEpoch = time(0);
  
  // convert current epoch time into LOCAL:date&time
  struct tm* aLocalDateTime = localtime(&iSecondsSincePosixEpoch);
  
  if ( ! aLocalDateTime )
    vWarn("localtime() error; errno=%d"_Stz,errno);
  
  Dtm wDtm;
  
  wDtm.xTsp.m_uYearDelta = aLocalDateTime->tm_year;
  wDtm.xTsp.m_uMonth     = aLocalDateTime->tm_mon + 1;
  wDtm.xTsp.m_uDay       = aLocalDateTime->tm_mday;
  wDtm.xTsp.m_uHour      = aLocalDateTime->tm_hour;
  wDtm.xTsp.m_uMinute    = aLocalDateTime->tm_min;
  wDtm.xTsp.m_uSecond    = aLocalDateTime->tm_sec;
  wDtm.xTsp.m_uMicro     = 0;
  wDtm.xTsp.m_uSpare     = 0;

  return wDtm;
}

/*----------------------------------------------------------------------------*/
/* set as current UTC time                                                    */
/*----------------------------------------------------------------------------*/
Dtm TspLib::wGetUTCNow(void)
{  
  // get current epoch time
  time_t iSecondsSincePosixEpoch = time(0);
  
  // convert current epoch time into UTC:date&time
  struct tm* aUtcDateTime = gmtime(&iSecondsSincePosixEpoch);
  
  if (! aUtcDateTime)
    vWarn("gmtime() error; errno=%d"_Stz,errno);
  
  Dtm wDtm;
  
  wDtm.xTsp.m_uYearDelta = aUtcDateTime->tm_year;
  wDtm.xTsp.m_uMonth     = aUtcDateTime->tm_mon + 1;
  wDtm.xTsp.m_uDay       = aUtcDateTime->tm_mday;
  wDtm.xTsp.m_uHour      = aUtcDateTime->tm_hour;
  wDtm.xTsp.m_uMinute    = aUtcDateTime->tm_min;
  wDtm.xTsp.m_uSecond    = aUtcDateTime->tm_sec;
  wDtm.xTsp.m_uMicro     = 0;
  wDtm.xTsp.m_uSpare     = 0;

  return wDtm;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
