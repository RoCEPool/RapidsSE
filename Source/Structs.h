/*============================================================================*/
/* Structs.h -- Structs                                                       */
/*============================================================================*/
#pragma once

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // date (since 1900)
  struct _Dte {
    u8 m_uSpare     : 47 ; //         47b
    
    u8 m_uDay       :  5 ; // 0..31       same as xTsp.dte
    u8 m_uMonth     :  4 ; // 0..15
    u8 m_uYearDelta :  8 ; // 0..255  17b (1900+8b == 2155)
    //               ---
    //                64
    //               ===
  };


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // era-based date
  struct _Edt {
    
    
    u8 m_uDay       :  5 ; // 0..31
    u8 m_uMonth     :  4 ; // 0..15
    u8 m_uYearInEra :  7 ; // 0..127  16b (eraBase+7b == 127)
    
    u8 m_uSpare     : 48 ; //         48b
    //               ---
    //                64
    //               ===
  };


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // interval (seconds & milliseconds)
  struct _Ivl {
    u8 m_uMilli     : 10 ; // 0..1023
    u8 m_uSecond    : 54 ; // 0..1.8E16
    //               ---
    //                64
    //               ===
  };


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // time (down to microseconds)
  struct _Tme {
    u8 m_uSpare     : 27 ; //         27b
    
    u8 m_uMicro     : 20 ; // 0..1M       same as xTsp.tme
    u8 m_uSecond    :  6 ; // 0..63
    u8 m_uMinute    :  6 ; // 0..63
    u8 m_uHour      :  5 ; // 0..31   37b
    //               ---
    //                64
    //               ===
  };


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // timestamp (down to microseconds) date & time
  struct _Tsp {
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
  };

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
