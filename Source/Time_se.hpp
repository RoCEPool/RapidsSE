/*============================================================================*/
/* Time_se.h - system time                                                    */
/*============================================================================*/
#pragma once

namespace System
{
  static const i8 C_YEAR_BASE = 1900;
  
  class Since;
  
  class Time : public Object
  {
  public:
    struct {
      u8 m_iYearDelta :  8;  // 256 delta years, base is 1900
      u8 m_iMonth     :  4;  // 16 months
      u8 m_iDay       :  5;  // 32 days
      u8 m_iHour      :  5;  // 32 hours
      u8 m_iMinute    :  6;  // 64 minutes
      u8 m_iSecond    :  6;  // 64 seconds
      u8 m_iMicro     : 20;  // 1M microseconds
      u8 m_iSpare     : 10;  // 10 bits
                     // 64 bits
    };
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    
    Time(void);
    ~Time(void) noexcept(false);
    
    void vAppendLocal(char** pz);
    void vAppendUtc(char** pz);
    bool bExtractUTC(char** ps);
    void vSetLocalNow(void);
    void vSetLocal(Since* pSince);
    void vSetUTCNow(void);
    void vSetUTC(Since* pSince);
    void vUTC(s32& sUTC);
    
    Footer* aFooter(void);
  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
