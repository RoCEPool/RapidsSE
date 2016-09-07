/*============================================================================*/
/* Time.h - time type                                                         */
/*============================================================================*/

#pragma once

#include "Common.h"

namespace Common
{
  class Since;
  
  class Time : public Object
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
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    
    Time(void);
    ~Time(void);
    
    void vSet(Since* pSince);

    void vAppendUtc(char** pz);
    bool bExtractUTC(char** ps);
    
    void vUTC(s& sUTC);
  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/