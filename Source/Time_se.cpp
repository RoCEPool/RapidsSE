/*============================================================================*/
/* Time_se.cpp - system date&time                                             */
/*============================================================================*/

#include "System.h"

#include "Pool.h"

/*----------------------------------------------------------------------------*/
/* operator new: class cannot be new'd                                        */
/*----------------------------------------------------------------------------*/
void* Time::operator new(size_t)
{
  vFatal("fatal: Time class cannot be new'd"_Stz);
  
  NEVER_REACH;
}

/*----------------------------------------------------------------------------*/
/* Time: constructor                                                          */
/*----------------------------------------------------------------------------*/
Time::Time(void)
: Object(CN_TIME)
{
  _precondition_(_MAGIC_,CN_TIME,_FOOTER_);
  
  m_iYearDelta = 0;
  m_iMonth     = 0;
  m_iDay       = 0;
  m_iHour      = 0;
  m_iMinute    = 0;
  m_iSecond    = 0;
  m_iMicro     = 0;
  
  _checkInitialised_(CN_TIME);
  
  _postcondition_(_MAGIC_,CN_TIME,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* Time: destructor                                                           */
/*----------------------------------------------------------------------------*/
Time::~Time(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_TIME,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* set time using since                                                       */
/*----------------------------------------------------------------------------*/
void Time::vSetLocal(Since* pSincePosixEpoch)
{
  _precondition_(_MAGIC_,CN_TIME,_FOOTER_);
  
  time_t iSecondsSincePosixEpoch = pSincePosixEpoch->iSeconds();
  
  // convert secondsSince into a tm structure for local time zone
  struct tm* aLocalDateTime = localtime(&iSecondsSincePosixEpoch);

  if (! aLocalDateTime)
    vWarn("localtime_r() error; errno=%d"_Stz,errno);
  
  m_iYearDelta = aLocalDateTime->tm_year;
  m_iMonth     = aLocalDateTime->tm_mon + 1;
  m_iDay       = aLocalDateTime->tm_mday;
  m_iHour      = aLocalDateTime->tm_hour;
  m_iMinute    = aLocalDateTime->tm_min;
  m_iSecond    = aLocalDateTime->tm_sec;
  m_iMicro     = (int)pSincePosixEpoch->iMicroseconds();
}

/*----------------------------------------------------------------------------*/
/* set as current local time                                                  */
/*----------------------------------------------------------------------------*/
void Time::vSetLocalNow(void)
{
  _precondition_(_MAGIC_,CN_TIME,_FOOTER_);
  
  // get current epoch time
  time_t iSecondsSincePosixEpoch = time(0);
  
  // convert current epoch time into LOCAL:date&time
  struct tm* aLocalDateTime = localtime(&iSecondsSincePosixEpoch);
  
  if ( ! aLocalDateTime )
    vWarn("localtime() error; errno=%d"_Stz,errno);
  
  m_iYearDelta = aLocalDateTime->tm_year;
  m_iMonth     = aLocalDateTime->tm_mon + 1;
  m_iDay       = aLocalDateTime->tm_mday;
  m_iHour      = aLocalDateTime->tm_hour;
  m_iMinute    = aLocalDateTime->tm_min;
  m_iSecond    = aLocalDateTime->tm_sec;
  m_iMicro     = 0;
}

/*----------------------------------------------------------------------------*/
/* vSet: set time using since                                                 */
/*----------------------------------------------------------------------------*/
void Time::vSetUTC(Since* pSincePosixEpoch)
{
  _precondition_(_MAGIC_,CN_TIME,_FOOTER_);
  
  time_t iSecondsSincePosixEpoch = pSincePosixEpoch->iSeconds();
  
  // convert secondsSince into a tm structure for UTC/GMT time zone
  struct tm* aUtcDateTime = gmtime(&iSecondsSincePosixEpoch);

  if (! aUtcDateTime)
    vWarn("gmtime() error; errno=%d"_Stz,errno);
  
  m_iYearDelta = aUtcDateTime->tm_year;
  m_iMonth     = aUtcDateTime->tm_mon + 1;
  m_iDay       = aUtcDateTime->tm_mday;
  m_iHour      = aUtcDateTime->tm_hour;
  m_iMinute    = aUtcDateTime->tm_min;
  m_iSecond    = aUtcDateTime->tm_sec;
  m_iMicro     = (int)pSincePosixEpoch->iMicroseconds();
}

/*----------------------------------------------------------------------------*/
/* set as current UTC time                                                    */
/*----------------------------------------------------------------------------*/
void Time::vSetUTCNow(void)
{
  _precondition_(_MAGIC_,CN_TIME,_FOOTER_);
  
  // get current epoch time
  time_t iSecondsSincePosixEpoch = time(0);
  
  // convert current epoch time into UTC:date&time
  struct tm* aUtcDateTime = gmtime(&iSecondsSincePosixEpoch);
  
  if (! aUtcDateTime)
    vWarn("gmtime() error; errno=%d"_Stz,errno);
  
  m_iYearDelta = aUtcDateTime->tm_year;
  m_iMonth     = aUtcDateTime->tm_mon + 1;
  m_iDay       = aUtcDateTime->tm_mday;
  m_iHour      = aUtcDateTime->tm_hour;
  m_iMinute    = aUtcDateTime->tm_min;
  m_iSecond    = aUtcDateTime->tm_sec;
  m_iMicro     = 0;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Time::aFooter(void)
{
  _precondition_(_MAGIC_,CN_TIME);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
