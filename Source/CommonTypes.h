/*============================================================================*/
/* CommonTypes.h -- Common Types                                              */
/*============================================================================*/
#pragma once

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// bcd / decimal
class Bcd
{
public:
  i8 m_iBcd;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// boolean
class Bol
{
public:
  i8 m_iBol;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// date (since 1900)
class Dte
{
public:
  u8 m_uSpare     : 47 ; //         47b
  
  u8 m_uDay       :  5 ; // 0..31       same as _Tsp.dte
  u8 m_uMonth     :  4 ; // 0..15
  u8 m_uYearDelta :  8 ; // 0..255  17b (1900+8b == 2155)
  //               ---
  //                64
  //               ===
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// era-based date (date given some known / understood era)
class Edt
{
public:
  u8 m_uDay       :  5 ; // 0..31
  u8 m_uMonth     :  4 ; // 0..15
  u8 m_uYearInEra :  7 ; // 0..127  16b (eraBase+7b == 127)
  
  u8 m_uSpare     : 48 ; //         48b
  //               ---
  //                64
  //               ===
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// float
class Flt
{
public:
  d8 m_dFlt;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// integer
class Int
{
public:
  i8 m_iInt;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// interval
class Ivl
{
public:
  u8 m_uMicro     : 20 ; // 0..1M
  u8 m_uSecond    : 44 ; // 0..1.7E13
  //               ---
  //                64
  //               ===
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// time
class Tme
{
public:
  u8 m_uSpare     : 27 ; //         27b
  
  u8 m_uMicro     : 20 ; // 0..1M       same as _Tsp.tme
  u8 m_uSecond    :  6 ; // 0..63
  u8 m_uMinute    :  6 ; // 0..63
  u8 m_uHour      :  5 ; // 0..31   37b
  //               ---
  //                64
  //               ===
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// timestamp
class Tsp
{
public:
  u8 m_uSpare     :  9 ; //        9b
  
  u8 m_uLocal     :  1 ; //        1b
  
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
