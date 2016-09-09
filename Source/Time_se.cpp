/*============================================================================*/
/* Time.cpp - system time                                                     */
/*============================================================================*/

#include "Common.h"

/*----------------------------------------------------------------------------*/
/* operator new: class cannot be new'd                                        */
/*----------------------------------------------------------------------------*/
void* Time::operator new(size_t iSize)
{
  ASSERT(iSize);
  vFatal("fatal: Time class cannot be new'd");
  
  ASSERT(0);
}

/*----------------------------------------------------------------------------*/
/* Time: constructor                                                          */
/*----------------------------------------------------------------------------*/
Time::Time(void)
: Object(CN_TIME)
{
  m_iYear   = 0;
  m_iMonth  = 0;
  m_iDay    = 0;
  m_iHour   = 0;
  m_iMinute = 0;
  m_iSecond = 0;
  m_iMicro  = 0;
  
  Memory::vCheckInitialised(this,sizeof(Time));
}

/*----------------------------------------------------------------------------*/
/* ~Time: destructor                                                          */
/*----------------------------------------------------------------------------*/
Time::~Time(void)
{
}

/*----------------------------------------------------------------------------*/
/* vSet: set time using since                                                 */
/*----------------------------------------------------------------------------*/
void Time::vSet(Since* pSince)
{
  struct tm xTimeResult;
  
  time_t iSeconds = pSince->iSeconds();
  
  void* pTimeResult = gmtime_r(&iSeconds,&xTimeResult);
  
  if ( ! pTimeResult )
    vError("gmtime_r() error; errno=%d",errno);
  
  m_iYear   = xTimeResult.tm_year;
  m_iMonth  = xTimeResult.tm_mon+1;
  m_iDay    = xTimeResult.tm_mday;
  m_iHour   = xTimeResult.tm_hour;
  m_iMinute = xTimeResult.tm_min;
  m_iSecond = xTimeResult.tm_sec;
  m_iMicro  = (int)pSince->iMicroseconds();
}

/*----------------------------------------------------------------------------*/
/* vAppendUtc: append given time into workspace                               */
/*----------------------------------------------------------------------------*/
void Time::vAppendUtc(char** pz)
{
  // refer: http://en.wikipedia.org/wiki/ISO_8601
  
  char* z = *pz;
  
  i8 iYear  = m_iYear + 1900;
  i8 iMilli = DIV(m_iMicro,1000);
  
  String::vAppendDecimal(&z,iYear);
  String::vAppendChar(&z,'-');
  
  if (m_iMonth < 10) String::vAppendChar(&z,'0');
  String::vAppendDecimal(&z,m_iMonth);
  String::vAppendChar(&z,'-');
  
  if (m_iDay < 10) String::vAppendChar(&z,'0');
  String::vAppendDecimal(&z,m_iDay);
  String::vAppendChar(&z,'T');
  
  if (m_iHour < 10) String::vAppendChar(&z,'0');
  String::vAppendDecimal(&z,m_iHour);
  String::vAppendChar(&z,':');
  
  if (m_iMinute < 10) String::vAppendChar(&z,'0');
  String::vAppendDecimal(&z,m_iMinute);
  String::vAppendChar(&z,':');
  
  if (m_iSecond < 10) String::vAppendChar(&z,'0');
  String::vAppendDecimal(&z,m_iSecond);
  String::vAppendChar(&z,'.');
  
  if (iMilli < 100) String::vAppendChar(&z,'0');
  if (iMilli < 10)  String::vAppendChar(&z,'0');
  String::vAppendDecimal(&z,iMilli);
  String::vAppendChar(&z,'Z');
  
  *z = '\0';
  
  *pz = z;
}

/*----------------------------------------------------------------------------*/
/* bExtractUTC: extract a utc time from substr       yyyy-mm-ddThh:mi:ss.fffZ */
/*----------------------------------------------------------------------------*/
bool Time::bExtractUTC(char** ps)
{
  // refer: http://en.wikipedia.org/wiki/ISO_8601
  
  if ( ! *ps )
    return false;
  
  m_iYear   = 0; m_iMonth  = 0; m_iDay    = 0;
  m_iHour   = 0; m_iMinute = 0; m_iSecond = 0;
  m_iMicro  = 0;
  
  i8 v;
  
  if ( ! String::bExtractInw(ps,&v) ) return false;
  m_iYear = (unsigned int)v - 1900;
  
  if ('-' != *(*ps)++) return false;
  
  if ( ! String::bExtractInw(ps,&v) ) return false;
  m_iMonth = (unsigned int)v;
  
  if ('-' != *(*ps)++) return false;
  
  if ( ! String::bExtractInw(ps,&v) ) return false;
  m_iDay = (unsigned int)v;
  
  // - - -
  
  if ('T' != **ps && ' ' != **ps) return true;
  
  (*ps)++;
  
  if ( ! String::bExtractInw(ps,&v) ) return true;
  m_iHour = (unsigned int)v;
  
  if (':' != *(*ps)++) return true;
  
  if ( ! String::bExtractInw(ps,&v) ) return true;
  m_iMinute = (unsigned int)v;
  
  if (':' != *(*ps)++) return true;
  
  if ( ! String::bExtractInw(ps,&v) ) return true;
  m_iSecond = (unsigned int)v;
  
  if ('.' == **ps)
  {
    (*ps)++;
    if ( String::bExtractInw(ps,&v) )
      m_iMicro  = (unsigned int)(v*1000);
  }
  
  if ('Z' == **ps)
    (*ps)++;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* put time into sUTC as utc string                  yyyy-mm-ddThh:mi:ss.fffZ */
/*----------------------------------------------------------------------------*/
void Time::vUTC(s& sUTC)
{
  char* pz = sUTC.a();
  
  vAppendUtc(&pz);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Time::pFooter(void)
{
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/