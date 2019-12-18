/*============================================================================*/
/* CommonTypes.h -- Common Types                                              */
/*============================================================================*/
#pragma once

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// bcd / decimal
class Bcd
{
public:
  union {
    i8 m_iBcd;
    i8 m_iAsI8;
  };
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// boolean
class Bol
{
public:
  union {
    i8 m_iBol;
    i8 m_iAsI8;
  };
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// date (since 1900)
class Dte
{
public:
  union {
    struct {
      u8 m_uSpare     : 47 ; //         47b
      
      u8 m_uDay       :  5 ; // 0..31       same as _Tsp.dte
      u8 m_uMonth     :  4 ; // 0..15
      u8 m_uYearDelta :  8 ; // 0..255  17b (1900+8b == 2155)
      //               ---
      //                64
      //               ===
    };
    i8 m_iAsI8;
  };
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// era-based date (date given some known / understood era)
class Edt
{
public:
  union {
    struct {
      u8 m_uDay       :  5 ; // 0..31
      u8 m_uMonth     :  4 ; // 0..15
      u8 m_uYearInEra :  7 ; // 0..127  16b (eraBase+7b == 127)
      
      u8 m_uSpare     : 48 ; //         48b
      //               ---
      //                64
      //               ===
    };
    i8 m_iAsI8;
  };
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// float
class Flt
{
public:
  union {
    d8 m_dFlt;
    i8 m_iAsI8;
  };
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// integer
class Int
{
public:
  union {
    d8 m_iInt;
    i8 m_iAsI8;
  };
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// interval
class Ivl
{
public:
  union {
    struct {
      u8 m_uMicro     : 20 ; // 0..1M
      u8 m_uSecond    : 44 ; // 0..1.7E13
      //               ---
      //                64
      //               ===
    };
    i8 m_iAsI8;
  };
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// time
class Tme
{
public:
  union {
    struct {
      u8 m_uSpare     : 27 ; //         27b
      
      u8 m_uMicro     : 20 ; // 0..1M       same as _Tsp.tme
      u8 m_uSecond    :  6 ; // 0..63
      u8 m_uMinute    :  6 ; // 0..63
      u8 m_uHour      :  5 ; // 0..31   37b
      //               ---
      //                64
      //               ===
    };
    i8 m_iAsI8;
  };
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// timestamp
class Tsp
{
public:
  union {
    struct {
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
    i8 m_iAsI8;
  };
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
