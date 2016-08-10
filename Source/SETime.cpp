/*============================================================================*/
/* Time.cpp - system time                                                     */
/*============================================================================*/

#include "SETime.hpp"

/*----------------------------------------------------------------------------*/
/* operator new: class cannot be new'd                                        */
/*----------------------------------------------------------------------------*/
void* Time::operator new(size_t)
{
  vFatal("fatal: Time class cannot be new'd");
  
  //vExitFatal();
  
  exit(EXIT_FAILURE);
}

/*----------------------------------------------------------------------------*/
/* Time: constructor                                                          */
/*----------------------------------------------------------------------------*/
Time::Time()
{
}

/*----------------------------------------------------------------------------*/
/* ~Time: destructor                                                          */
/*----------------------------------------------------------------------------*/
Time::~Time()
{
}

/*----------------------------------------------------------------------------*/
/* vAppendUtc: append given time into workspace                               */
/*----------------------------------------------------------------------------*/
void Time::vAppendUtc(char** pz, Time* pTime)
{
  char* z = *pz;
  
  i8 iYear  = pTime->m_iYear + 1900;
  i8 iMilli = DIV(pTime->m_iMicro,1000);
  
  String::vAppendDecimal(&z,iYear);
  String::vAppendChar(&z,'-');
  
  if (pTime->m_iMonth < 10) String::vAppendChar(&z,'0');
  String::vAppendDecimal(&z,pTime->m_iMonth);
  String::vAppendChar(&z,'-');
  
  if (pTime->m_iDay < 10) String::vAppendChar(&z,'0');
  String::vAppendDecimal(&z,pTime->m_iDay);
  String::vAppendChar(&z,'T');
  
  if (pTime->m_iHour < 10) String::vAppendChar(&z,'0');
  String::vAppendDecimal(&z,pTime->m_iHour);
  String::vAppendChar(&z,':');
  
  if (pTime->m_iMinute < 10) String::vAppendChar(&z,'0');
  String::vAppendDecimal(&z,pTime->m_iMinute);
  String::vAppendChar(&z,':');
  
  if (pTime->m_iSecond < 10) String::vAppendChar(&z,'0');
  String::vAppendDecimal(&z,pTime->m_iSecond);
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
bool Time::bExtractUTC(char** ps, Time* pTime)
{
  // refer: http://en.wikipedia.org/wiki/ISO_8601
  
  if ( ! *ps )
    return false;
  
  pTime->m_iYear   = 0; pTime->m_iMonth  = 0; pTime->m_iDay    = 0;
  pTime->m_iHour   = 0; pTime->m_iMinute = 0; pTime->m_iSecond = 0;
  pTime->m_iMicro  = 0;
  
  i8 v;
  
  if ( ! String::bExtractInw(ps,&v) ) return false;
  pTime->m_iYear = (unsigned int)v - 1900;
  
  if ('-' != *(*ps)++) return false;
  
  if ( ! String::bExtractInw(ps,&v) ) return false;
  pTime->m_iMonth = (unsigned int)v;
  
  if ('-' != *(*ps)++) return false;
  
  if ( ! String::bExtractInw(ps,&v) ) return false;
  pTime->m_iDay = (unsigned int)v;
  
  // - - -
  
  if ('T' != **ps && ' ' != **ps) return true;
  
  (*ps)++;
  
  if ( ! String::bExtractInw(ps,&v) ) return true;
  pTime->m_iHour = (unsigned int)v;
  
  if (':' != *(*ps)++) return true;
  
  if ( ! String::bExtractInw(ps,&v) ) return true;
  pTime->m_iMinute = (unsigned int)v;
  
  if (':' != *(*ps)++) return true;
  
  if ( ! String::bExtractInw(ps,&v) ) return true;
  pTime->m_iSecond = (unsigned int)v;
  
  if ('.' == **ps)
  {
    (*ps)++;
    if ( String::bExtractInw(ps,&v) )
      pTime->m_iMicro  = (unsigned int)(v*1000);
  }
  
  if ('Z' == **ps)
    (*ps)++;
  
  return true;
}


/*----------------------------------------------------------------------------*/
/* iToSeconds: convery Time to i8 seconds                                     */
/*----------------------------------------------------------------------------*/
i8 Time::iToSeconds(void)
{
  struct tm xTM;
  
  xTM.tm_year   = m_iYear;
  xTM.tm_mon    = m_iMonth-1;
  xTM.tm_mday   = m_iDay;
  xTM.tm_hour   = m_iHour;
  xTM.tm_min    = m_iMinute;
  xTM.tm_sec    = m_iSecond;
  xTM.tm_isdst  = 0;
  xTM.tm_gmtoff = 0;
  xTM.tm_wday   = 0;
  xTM.tm_yday   = 0;
  xTM.tm_zone   = nullptr;
  
  i8 iSeconds = timegm(&xTM);

  return iSeconds;
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/