/*============================================================================*/
/* Word.hpp -- Word                                                           */
/*============================================================================*/
#pragma once

namespace Base
{
  class Word
  {
  public:
    union {
      byte  m_yaByte[C_BYTES_PER_WORD];
      
      char* m_aChar;
      
      d8    m_dF8;
      
      i1    m_iaI1[8];
      i2    m_iaI2[4];
      i4    m_iaI4[2];
      i8    m_iI8;
      u8    m_uI8;
      
      w8    m_wWord;
      
      // - - - - - - - - - - - - - - - -
            
      // date (since 1900)
      struct {
        u8 m_uSpare     : 47 ; //         47b
        
        u8 m_uDay       :  5 ; // 0..31       same as xTsp.dte
        u8 m_uMonth     :  4 ; // 0..15
        u8 m_uYearDelta :  8 ; // 0..255  17b (1900+8b == 2155)
                               //               ---
                               //                64
                               //               ===
      } xDte;
      
      // era-based date
      struct {
        
        
        u8 m_uDay       :  5 ; // 0..31
        u8 m_uMonth     :  4 ; // 0..15
        u8 m_uYearInEra :  7 ; // 0..127  16b (eraBase+7b == 127)
        
        u8 m_uSpare     : 48 ; //         48b
                               //               ---
                               //                64
                               //               ===
      } xEdt;
      
      // interval (seconds & milliseconds)
      struct {
        u8 m_uMilli     : 10 ; // 0..1023
        u8 m_uSecond    : 54 ; // 0..1.8E16
                               //               ---
                               //                64
                               //               ===
      } xIvl;
      
      // time (down to microseconds)
      struct {
        u8 m_uSpare     : 27 ; //         27b
        
        u8 m_uMicro     : 20 ; // 0..1M       same as xTsp.tme
        u8 m_uSecond    :  6 ; // 0..63
        u8 m_uMinute    :  6 ; // 0..63
        u8 m_uHour      :  5 ; // 0..31   37b
                               //               ---
                               //                64
                               //               ===
      } xTme;
      
      // date & time
      struct {
        u8 m_uSpare     : 10 ; //         10b
        
        u8 m_uMicro     : 20 ; // 0..1M
        u8 m_uSecond    :  6 ; // 0..63
        u8 m_uMinute    :  6 ; // 0..63
        u8 m_uHour      :  5 ; // 0..31   37b
        
        u8 m_uDay       :  5 ; // 0..31
        u8 m_uMonth     :  4 ; // 0..15
        u8 m_uYearDelta :  8 ; // 0..255  17b  (1900+8b == 2155)
                               //               ---
                               //                64
                               //               ===
      } xTsp;
    };
  
  public:
    Word(void);
    Word(i8 iArg);
    Word(w8 wArg);
    ~Word(void) noexcept(false);
  
    void operator =(i8 iArg);
    void operator =(w8 wArg);
  
    operator w8(void) { return m_wWord; }
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
