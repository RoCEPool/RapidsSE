/*============================================================================*/
/* Time.h - time type                                                         */
/*============================================================================*/

#pragma once

#include "Common.h"

namespace Common
{
  class Time
  {
  public:
    struct {
      u8 m_iYear   :  8;  // 256 years, base is 1900
      u8 m_iMonth  :  4;  // 16 months
      u8 m_iDay    :  5;  // 32 days
      u8 m_iHour   :  5;  // 32 hours
      u8 m_iMinute :  6;  // 64 minutes
      u8 m_iSecond :  6;  // 64 seconds
      u8 m_iMicro  : 20;  // 1M useconds
      u8 m_iSpare  : 10;  // 10 bits
                  // 64 bits
    };
    
  public:
    void* operator new(size_t);
    
    Time();
    ~Time();
    
    static void vAppendUtc(char** pz, Time* pTime);
    static bool bExtractUTC(char** ps, Time* pTime);
    
    char* pAppend(char* pWS);
    
    i8 iToSeconds(void);
    
    void operator = (Time xArg)
    {
      m_iYear   = xArg.m_iYear;
      m_iMonth  = xArg.m_iMonth;
      m_iDay    = xArg.m_iDay;
      m_iHour   = xArg.m_iHour;
      m_iMinute = xArg.m_iMinute;
      m_iSecond = xArg.m_iSecond;
      m_iMicro  = xArg.m_iMicro;
      //m_iSpare  = xArg.m_iSpare;
    }
    
    void operator = (i8 i)
    {
      m_iYear   = 0;
      m_iMonth  = 0;
      m_iDay    = 0;
      m_iHour   = 0;
      m_iMinute = 0;
      m_iSecond = 0;
      m_iMicro  = 0;
      //m_iSpare  = 0;
    }
    
    bool operator >= (Time* pArg)
    {
      if (m_iYear > pArg->m_iYear) return true;
      if (m_iYear < pArg->m_iYear) return false;

      if (m_iMonth > pArg->m_iMonth) return true;
      if (m_iMonth < pArg->m_iMonth) return false;

      if (m_iDay > pArg->m_iDay) return true;
      if (m_iDay < pArg->m_iDay) return false;
      
      if (m_iHour > pArg->m_iHour) return true;
      if (m_iHour < pArg->m_iHour) return false;
      
      if (m_iMinute > pArg->m_iMinute) return true;
      if (m_iMinute < pArg->m_iMinute) return false;
      
      if (m_iSecond > pArg->m_iSecond) return true;
      if (m_iSecond < pArg->m_iSecond) return false;
      
      if (m_iMicro > pArg->m_iMicro) return true;
      if (m_iMicro < pArg->m_iMicro) return false;
      
      return true;
    }
    
    bool operator <= (Time* pArg)
    {
      if (m_iYear < pArg->m_iYear) return true;
      if (m_iYear > pArg->m_iYear) return false;
      
      if (m_iMonth < pArg->m_iMonth) return true;
      if (m_iMonth > pArg->m_iMonth) return false;
      
      if (m_iDay < pArg->m_iDay) return true;
      if (m_iDay > pArg->m_iDay) return false;
      
      if (m_iHour < pArg->m_iHour) return true;
      if (m_iHour > pArg->m_iHour) return false;
      
      if (m_iMinute < pArg->m_iMinute) return true;
      if (m_iMinute > pArg->m_iMinute) return false;
      
      if (m_iSecond < pArg->m_iSecond) return true;
      if (m_iSecond > pArg->m_iSecond) return false;
      
      if (m_iMicro < pArg->m_iMicro) return true;
      if (m_iMicro > pArg->m_iMicro) return false;
      
      return true;
    }
          
  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
