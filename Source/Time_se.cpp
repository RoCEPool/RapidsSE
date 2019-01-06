/*============================================================================*/
/* Time_se.cpp - system time                                                     */
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
  
  m_iYear   = 0;
  m_iMonth  = 0;
  m_iDay    = 0;
  m_iHour   = 0;
  m_iMinute = 0;
  m_iSecond = 0;
  m_iMicro  = 0;
  
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
/* vAppendLocal: append given time into workspace                             */
/*----------------------------------------------------------------------------*/
void Time::vAppendLocal(char** pz)
{
  _precondition_(_MAGIC_,CN_TIME,_FOOTER_);
  
  // refer: http://en.wikipedia.org/wiki/ISO_8601
  
  char* z = *pz;
  
  i8 iYear  = m_iYear + 1900;
  i8 iMilli = DIV(m_iMicro,1000);
  
  StrLib::vAppendDecimal(&z,iYear);
  StrLib::vAppendChar(&z,'-');
  
  if (m_iMonth < 10) StrLib::vAppendChar(&z,'0');
  StrLib::vAppendDecimal(&z,m_iMonth);
  StrLib::vAppendChar(&z,'-');
  
  if (m_iDay < 10) StrLib::vAppendChar(&z,'0');
  StrLib::vAppendDecimal(&z,m_iDay);
  StrLib::vAppendChar(&z,' ');
  
  if (m_iHour < 10) StrLib::vAppendChar(&z,'0');
  StrLib::vAppendDecimal(&z,m_iHour);
  StrLib::vAppendChar(&z,':');
  
  if (m_iMinute < 10) StrLib::vAppendChar(&z,'0');
  StrLib::vAppendDecimal(&z,m_iMinute);
  StrLib::vAppendChar(&z,':');
  
  if (m_iSecond < 10) StrLib::vAppendChar(&z,'0');
  StrLib::vAppendDecimal(&z,m_iSecond);
  
  //  StrLib::vAppendChar(&z,'.');
  //  if (iMilli < 100) StrLib::vAppendChar(&z,'0');
  //  if (iMilli < 10)  StrLib::vAppendChar(&z,'0');
  //  StrLib::vAppendDecimal(&z,iMilli);
  //  StrLib::vAppendChar(&z,'Z');
  
  StrLib::vAppendString(&z," (local)");
  
  *z = '\0';
  
  *pz = z;
}

/*----------------------------------------------------------------------------*/
/* vAppendUtc: append given time into workspace                               */
/*----------------------------------------------------------------------------*/
void Time::vAppendUtc(char** pz)
{
  _precondition_(_MAGIC_,CN_TIME,_FOOTER_);
  
  // refer: http://en.wikipedia.org/wiki/ISO_8601
  
  char* z = *pz;
  
  i8 iYear  = m_iYear + 1900;
  i8 iMilli = DIV(m_iMicro,1000);
  
  StrLib::vAppendDecimal(&z,iYear);
  StrLib::vAppendChar(&z,'-');
  
  if (m_iMonth < 10) StrLib::vAppendChar(&z,'0');
  StrLib::vAppendDecimal(&z,m_iMonth);
  StrLib::vAppendChar(&z,'-');
  
  if (m_iDay < 10) StrLib::vAppendChar(&z,'0');
  StrLib::vAppendDecimal(&z,m_iDay);
  StrLib::vAppendChar(&z,'T');
  
  if (m_iHour < 10) StrLib::vAppendChar(&z,'0');
  StrLib::vAppendDecimal(&z,m_iHour);
  StrLib::vAppendChar(&z,':');
  
  if (m_iMinute < 10) StrLib::vAppendChar(&z,'0');
  StrLib::vAppendDecimal(&z,m_iMinute);
  StrLib::vAppendChar(&z,':');
  
  if (m_iSecond < 10) StrLib::vAppendChar(&z,'0');
  StrLib::vAppendDecimal(&z,m_iSecond);
  StrLib::vAppendChar(&z,'.');
  
  if (iMilli < 100) StrLib::vAppendChar(&z,'0');
  if (iMilli < 10)  StrLib::vAppendChar(&z,'0');
  StrLib::vAppendDecimal(&z,iMilli);
  StrLib::vAppendChar(&z,'Z');
  
  *z = '\0';
  
  *pz = z;
}

/*----------------------------------------------------------------------------*/
/* bExtractUTC: extract a utc time from substr       yyyy-mm-ddThh:mi:ss.fffZ */
/*----------------------------------------------------------------------------*/
bool Time::bExtractUTC(char** ps)
{
  _precondition_(_MAGIC_,CN_TIME,_FOOTER_);
  
  // refer: http://en.wikipedia.org/wiki/ISO_8601
  
  if ( ! *ps )
    return false;
  
  m_iYear   = 0; m_iMonth  = 0; m_iDay    = 0;
  m_iHour   = 0; m_iMinute = 0; m_iSecond = 0;
  m_iMicro  = 0;
  
  i8 v;
  
  if ( ! StrLib::bExtractInteger(ps,&v) ) return false;
  m_iYear = (unsigned int)v - 1900;
  
  if ('-' != *(*ps)++) return false;
  
  if ( ! StrLib::bExtractInteger(ps,&v) ) return false;
  m_iMonth = (unsigned int)v;
  
  if ('-' != *(*ps)++) return false;
  
  if ( ! StrLib::bExtractInteger(ps,&v) ) return false;
  m_iDay = (unsigned int)v;
  
  // - - -
  
  if ('T' != **ps && ' ' != **ps) return true;
  
  (*ps)++;
  
  if ( ! StrLib::bExtractInteger(ps,&v) ) return true;
  m_iHour = (unsigned int)v;
  
  if (':' != *(*ps)++) return true;
  
  if ( ! StrLib::bExtractInteger(ps,&v) ) return true;
  m_iMinute = (unsigned int)v;
  
  if (':' != *(*ps)++) return true;
  
  if ( ! StrLib::bExtractInteger(ps,&v) ) return true;
  m_iSecond = (unsigned int)v;
  
  if ('.' == **ps)
    {
    (*ps)++;
    if ( StrLib::bExtractInteger(ps,&v) )
      m_iMicro  = (unsigned int)(v*1000);
    }
  
  if ('Z' == **ps)
    (*ps)++;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* vSet: set time using since                                                 */
/*----------------------------------------------------------------------------*/
void Time::vSetLocal(Since* pSincePosixEpoch)
{
  _precondition_(_MAGIC_,CN_TIME,_FOOTER_);
  
  struct tm xCalendarDateTime_Local;
  
  time_t iSecondsSincePosixEpoch = pSincePosixEpoch->iSeconds();
  
  // convert secondsSince into a tm structure for local time zone
  void* aResult = localtime_r(&iSecondsSincePosixEpoch,&xCalendarDateTime_Local);
  
  if ( ! aResult )
    vError("localtime_r() error; errno=%d"_Stz,errno);
  
  m_iYear   = xCalendarDateTime_Local.tm_year;
  m_iMonth  = xCalendarDateTime_Local.tm_mon+1;
  m_iDay    = xCalendarDateTime_Local.tm_mday;
  m_iHour   = xCalendarDateTime_Local.tm_hour;
  m_iMinute = xCalendarDateTime_Local.tm_min;
  m_iSecond = xCalendarDateTime_Local.tm_sec;
  m_iMicro  = (int)pSincePosixEpoch->iMicroseconds();
}

/*----------------------------------------------------------------------------*/
/* vSet: set time using since                                                 */
/*----------------------------------------------------------------------------*/
void Time::vSetUTC(Since* pSincePosixEpoch)
{
  _precondition_(_MAGIC_,CN_TIME,_FOOTER_);
  
  struct tm xCalendarDateTime_UTC;
  
  time_t iSecondsSincePosixEpoch = pSincePosixEpoch->iSeconds();
  
  // convert secondsSince into a tm structure for UTC/GMT time zone
  void* aResult = gmtime_r(&iSecondsSincePosixEpoch,&xCalendarDateTime_UTC);
  
  if ( ! aResult )
    vError("gmtime_r() error; errno=%d"_Stz,errno);
  
  m_iYear   = xCalendarDateTime_UTC.tm_year;
  m_iMonth  = xCalendarDateTime_UTC.tm_mon+1;
  m_iDay    = xCalendarDateTime_UTC.tm_mday;
  m_iHour   = xCalendarDateTime_UTC.tm_hour;
  m_iMinute = xCalendarDateTime_UTC.tm_min;
  m_iSecond = xCalendarDateTime_UTC.tm_sec;
  m_iMicro  = (int)pSincePosixEpoch->iMicroseconds();
}

/*----------------------------------------------------------------------------*/
/* set as current local time                                                  */
/*----------------------------------------------------------------------------*/
void Time::vSetNowLocal(void)
{
  _precondition_(_MAGIC_,CN_TIME,_FOOTER_);
  
  time_t iSecondsSincePosixEpoch = time(0);   // get time now
  
  struct tm* xCalendarDateTime_Local = localtime(&iSecondsSincePosixEpoch);
  
  m_iYear   = xCalendarDateTime_Local->tm_year;
  m_iMonth  = xCalendarDateTime_Local->tm_mon+1;
  m_iDay    = xCalendarDateTime_Local->tm_mday;
  m_iHour   = xCalendarDateTime_Local->tm_hour;
  m_iMinute = xCalendarDateTime_Local->tm_min;
  m_iSecond = xCalendarDateTime_Local->tm_sec;
  m_iMicro  = 0;

}

/*----------------------------------------------------------------------------*/
/* set as current UTC time                                                    */
/*----------------------------------------------------------------------------*/
void Time::vSetNowUTC(void)
{
  _precondition_(_MAGIC_,CN_TIME,_FOOTER_);
  
  time_t t1 = time(0);   // get time now
  
  struct tm* xCalendarDateTime_UTC = gmtime( & t1 );
  
  m_iYear   = xCalendarDateTime_UTC->tm_year;
  m_iMonth  = xCalendarDateTime_UTC->tm_mon+1;
  m_iDay    = xCalendarDateTime_UTC->tm_mday;
  m_iHour   = xCalendarDateTime_UTC->tm_hour;
  m_iMinute = xCalendarDateTime_UTC->tm_min;
  m_iSecond = xCalendarDateTime_UTC->tm_sec;
  m_iMicro  = 0;
}

/*----------------------------------------------------------------------------*/
/* put time into sUTC as utc string                  yyyy-mm-ddThh:mi:ss.fffZ */
/*----------------------------------------------------------------------------*/
void Time::vUTC(StrVal& sUTC)
{
  _precondition_(_MAGIC_,CN_TIME,_FOOTER_);

  char* pz = sUTC.aChar();
  
  vAppendUtc(&pz);
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
