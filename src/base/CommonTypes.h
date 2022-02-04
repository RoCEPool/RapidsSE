/*============================================================================*/
/* CommonTypes.h -- Common Types                                              */
/*============================================================================*/
#pragma once

#include "TccStuff.h"

typedef i8 tXN; // opaque type for transaction number


//static const i8 C_ERA_0 =  0; // 1900
//static const i8 C_ERA_1 = 92; // 1992
//static const i8 C_ERA_2 = 99; // 1999

static const u8 C_YEAR_BASE  = 1900;
static const u8 C_YEAR_RANGE = 255;
static const u8 C_YEAR_MAX   = C_YEAR_BASE + C_YEAR_RANGE;
static const u8 C_YEAR_MIN   = C_YEAR_BASE;

static const u8 C_IVL_SEC_MAX    = 999999999;
static const u8 C_IVL_USEC_MAX   = 999999;
static const u8 C_IVL_USEC_SHIFT = 1000000;

static const u8 C_TSP_USEC_MIN = 0;
static const u8 C_TSP_USEC_MAX = 999999;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// bcd / decimal
class Bcd
{
public:
  Bcd_T T;

//  union {
//    i8 m_iBcd;
//    i8 m_iAsI8;
//    struct Bcd_T m_xBcd;
//  };

  static i8 iMinValue(void)            { return C_I8_MIN; }
  static i8 iMaxValue(void)            { return C_I8_MAX; }

  operator i8(void)                    { return T.m_iAsI8; }  // auto conversion
  
  void operator = (i8 iArg)            { T.m_iAsI8 = iArg; }  // assign from i8
  
  void vSetMax(void)                { T.m_iBcd = C_I8_MAX; }  // set max value
  void vSetMid(void)                { T.m_iBcd = C_I8_MID; }  // set mid value
  void vSetMin(void)                { T.m_iBcd = C_I8_MIN; }  // set min value
  void vSetNull(void)           { T.m_iBcd = C_DATUM_NULL; }  // set to be null
  void vSetUsual(void)                    { T.m_iBcd = 73; }  // set usual value

  bool bIsNull(void) { return (C_DATUM_NULL == T.m_iAsI8); }  // test for null
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// boolean
class Bol
{
public:
//  union {
//    i8 m_iBol;
//    i8 m_iAsI8;
//  };
  Bol_T T;

  static i8 iMinValue(void)            { return false; }
  static i8 iMaxValue(void)            { return true; }

  operator i8(void)                    { return T.m_iAsI8; }  // auto conversion
  
  void operator = (i8 iArg)            { T.m_iAsI8 = iArg; }  // assign from i8
  
  void vSetMax(void)                   { T.m_iBol = true; }  // set max value
  void vSetMid(void)                   { T.m_iBol = true; }  // set mid value
  void vSetMin(void)                   { T.m_iBol = false; }  // set min value
  void vSetNull(void)                  { T.m_iAsI8 = C_DATUM_NULL; }  // set to be null
  void vSetUsual(void)                 { T.m_iBol = true; }  // set usual value

  bool bIsNull(void) { return (C_DATUM_NULL == T.m_iAsI8); }  // test for null
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// date (since 1900)
class Dte
{
public:
//  union {
//    struct {
//      u8 m_uDay       :  5 ; // 0..31       same as _Tsp.dte
//      u8 m_uMonth     :  4 ; // 0..15
//      u8 m_uYearDelta :  8 ; // 0..255  17b (1900+8b == 2155)
//
//      u8 m_uSpare     : 47 ; //         47b
//      //               ---
//      //                64
//      //               ===
//    };
//    i8 m_iAsI8;
//  };
  
  Dte_T T;

  static i8 iMinValue(void)            { Dte dte; dte.vSetMin(); return dte.T.m_iAsI8; }
  static i8 iMaxValue(void)            { Dte dte; dte.vSetMax(); return dte.T.m_iAsI8; }

  operator i8(void)                    { return T.m_iAsI8; }  // auto conversion
  
  void operator = (i8 iArg)            { T.m_iAsI8 = iArg; }  // assign from i8

  i8 iYear()                            { return C_YEAR_BASE + T.m_uYearDelta; }
  i8 iMonth()                           { return T.m_uMonth; }
  i8 iDay()                             { return T.m_uDay; }

  void vSet(i8 iYear, i8 iMonth, i8 iDay) {
                     T.m_uYearDelta = iYear - C_YEAR_BASE;
                                      T.m_uMonth = iMonth;
                                          T.m_uDay = iDay;
                                           T.m_uSpare = 0; }

  void vSetMax(void)                      { T.m_iAsI8 = 0;
                            T.m_uYearDelta = C_YEAR_RANGE;
                                          T.m_uMonth = 12;
                                            T.m_uDay = 31; }  // set max value
  void vSetMid(void)                      { T.m_iAsI8 = 0;
                        T.m_uYearDelta = C_YEAR_RANGE / 2;
                                           T.m_uMonth = 6;
                                            T.m_uDay = 30; }  // set mid value
  void vSetMin(void)                      { T.m_iAsI8 = 0;
                                       T.m_uYearDelta = 0;
                                           T.m_uMonth = 1;
                                             T.m_uDay = 1; }  // set min value
  void vSetNull(void)          { T.m_iAsI8 = C_DATUM_NULL; }  // set to be null
  void vSetUsual(void)                    { T.m_iAsI8 = 0;
                                      T.m_uYearDelta = 35;
                                          T.m_uMonth = 11;
                                            T.m_uDay = 29; }  // set usual value

  bool bIsNull(void) { return (C_DATUM_NULL == T.m_iAsI8); }  // test for null

  // Tests if the given value is a valid date and if it is within range.
  // Writes to sErrorBuf if not
  static b1 bIsValid(i8 iYear, i8 iMonth, i8 iDay, s256& sErrorBuf);
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// era-based date (date given some known / understood era)
class Edt
{
public:
//  union {
//    struct {
//      u8 m_uDay       :  5 ; // 0..31
//      u8 m_uMonth     :  4 ; // 0..15
//      u8 m_uYearInEra :  7 ; // 0..127  16b (eraBase+7b == 127)
//
//      u8 m_uSpare     : 48 ; //         48b
//      //               ---
//      //                64
//      //               ===
//    };
//    i8 m_iAsI8;
//  };
  
  Edt_T T;
  
  operator i8(void)                    { return T.m_iAsI8; }  // auto conversion
  
  void operator = (i8 iArg)            { T.m_iAsI8 = iArg; }  // assign from i8
  
  void vSetMax(void)                                   { }  // set max value
  void vSetMid(void)                                   { }  // set mid value
  void vSetMin(void)                                   { }  // set min value
  void vSetNull(void)          { T.m_iAsI8 = C_DATUM_NULL; }  // set to be null
  void vSetUsual(void)                                 { }  // set usual value

  bool bIsNull(void) { return (C_DATUM_NULL == T.m_iAsI8); }  // test for null
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// float
class Flt
{
public:
//  union {
//    d8 m_dFlt;
//    i8 m_iAsI8;
//  };

  Flt_T T;

  static i8 iMinValue(void)            { Flt flt; flt.vSetMin(); return flt.T.m_iAsI8; }
  static i8 iMaxValue(void)            { Flt flt; flt.vSetMax(); return flt.T.m_iAsI8; }

  operator d8(void)                    { return T.m_dFlt;  }  // auto conversion
  operator i8(void)                    { return T.m_iAsI8; }  // auto conversion
  
  void operator = (i8 iArg)            { T.m_iAsI8 = iArg; }  // assign from i8
  void operator = (d8 dArg)            { T.m_dFlt  = dArg; }  // assign from d8

  void vSetMax(void)                { T.m_dFlt = C_D8_MAX; }  // set max value
  void vSetMid(void)                { T.m_dFlt = C_D8_MID; }  // set mid value
  void vSetMin(void)                { T.m_dFlt = C_D8_MIN; }  // set min value
  void vSetNull(void)          { T.m_iAsI8 = C_DATUM_NULL; }  // set to be null
  void vSetUsual(void)                 { T.m_dFlt = 3.141; }  // set usual value

  bool bIsNull(void) { return (C_DATUM_NULL == T.m_iAsI8); }  // test for null
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// integer
class Int
{
public:
//  union {
//    i8 m_iInt;
//    i8 m_iAsI8;
//  };

  Int_T T;

  static i8 iMinValue(void)            { return C_I8_MIN; }
  static i8 iMaxValue(void)            { return C_I8_MAX; }

  operator i8(void)                    { return T.m_iAsI8; }  // auto conversion
  
  void operator = (i8 iArg)            { T.m_iAsI8 = iArg; }  // assign from i8
  
  void vSetMax(void)               { T.m_iAsI8 = C_I8_MAX; }  // set max value
  void vSetMid(void)               { T.m_iAsI8 = C_I8_MID; }  // set mid value
  void vSetMin(void)               { T.m_iAsI8 = C_I8_MIN; }  // set min value
  void vSetNull(void)          { T.m_iAsI8 = C_DATUM_NULL; }  // set to be null
  void vSetUsual(void)                   { T.m_iAsI8 = 42; }  // set usual value

  bool bIsNull(void) { return (C_DATUM_NULL == T.m_iAsI8); }  // test for null
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

  static i8 iMinValue(void)            { Dte dte; dte.vSetMin(); return dte.T.m_iAsI8; }
  static i8 iMaxValue(void)            { Dte dte; dte.vSetMax(); return dte.T.m_iAsI8; }

  operator i8(void)                    { return m_iAsI8; }  // auto conversion
  
  void operator = (i8 iArg)            { m_iAsI8 = iArg; }  // assign from i8
  
  void vSetMax(void)               { m_uSecond = 999999;
                                      m_uMicro = 999999; }  // set max value
  void vSetMid(void)                  { m_uSecond = 999;
                                         m_uMicro = 999; }  // set mid value
  void vSetMin(void)                    { m_uSecond = 0;
                                           m_uMicro = 0; }  // set min value
  void vSetNull(void)          { m_iAsI8 = C_DATUM_NULL; }  // set to be null
  void vSetUsual(void)                                 { }  // set usual value

  bool bIsNull(void) { return (C_DATUM_NULL == m_iAsI8); }  // test for null
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// time
class Tme
{
public:
  union {
    struct {
      u8 m_uMicro     : 20 ; // 0..1M       same as _Tsp.tme
      u8 m_uSecond    :  6 ; // 0..63
      u8 m_uMinute    :  6 ; // 0..63
      u8 m_uHour      :  5 ; // 0..31   37b
      
      u8 m_uSpare     : 27 ; //         27b
      //               ---
      //                64
      //               ===
    };
    i8 m_iAsI8;
  };

  static i8 iMinValue(void)            { Tme tme; tme.vSetMin(); return tme.m_iAsI8; }
  static i8 iMaxValue(void)            { Tme tme; tme.vSetMax(); return tme.m_iAsI8; }

  operator i8(void)                    { return m_iAsI8; }  // auto conversion
  
  void operator = (i8 iArg)            { m_iAsI8 = iArg; }  // assign from i8

  i8 iHour()                            { return m_uHour;   }
  i8 iMin()                             { return m_uMinute; }
  i8 iSec()                             { return m_uSecond; }

  void vSet(i8 iHour, i8 iMinute, i8 iSecond, i8 iMicro) {
                                        m_uHour = iHour;
                                    m_uMinute = iMinute;
                                    m_uSecond = iSecond;
                                      m_uMicro = iMicro;
                                          m_uSpare  = 0; }
  void vSetMax(void)                      { m_iAsI8 = 0;
                                           m_uHour = 23;
                                         m_uMinute = 59;
                                         m_uSecond = 59;
                                      m_uMicro = 999999; }  // set max value
  void vSetMid(void)                      { m_iAsI8 = 0;
                                           m_uHour = 12;
                                          m_uMinute = 0;
                                          m_uSecond = 0;
                                           m_uMicro = 0; }  // set mid value
  void vSetMin(void)                      { m_iAsI8 = 0;
                                            m_uHour = 0;
                                          m_uMinute = 0;
                                          m_uSecond = 0;
                                           m_uMicro = 0; }  // set min value
  void vSetNull(void)          { m_iAsI8 = C_DATUM_NULL; }  // set to be null
  void vSetUsual(void)                                 { }  // set usual value

  bool bIsNull(void) { return (C_DATUM_NULL == m_iAsI8); }  // test for null

  // Tests if the given value is a valid time and if it is within range.
  // Writes to sErrorBuf if not
  static b1 bIsValid(i8 iHour, i8 iMin, i8 iSec, s256& sErrorBuf);
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// timestamp
class Tsp
{
public:
//  union {
//    struct {
//      u8 m_uLocal     :  1 ; //        1b
//
//      u8 m_uMicro     : 20 ; // 0..1M
//      u8 m_uSecond    :  6 ; // 0..63
//      u8 m_uMinute    :  6 ; // 0..63
//      u8 m_uHour      :  5 ; // 0..31   37b
//
//      u8 m_uDay       :  5 ; // 0..31
//      u8 m_uMonth     :  4 ; // 0..15
//      u8 m_uYearDelta :  8 ; // 0..255  17b  (1900+8b == 2155)
//
//      u8 m_uSpare     :  9 ; //        9b
//      //               ---
//      //                64
//      //               ===
//    };
//    i8 m_iAsI8;
//  };

  Tsp_T T;

  static i8 iMinValue(void)            { Tsp tsp; tsp.vSetMin(); return tsp.T.m_iAsI8; }
  static i8 iMaxValue(void)            { Tsp tsp; tsp.vSetMax(); return tsp.T.m_iAsI8; }

  operator i8(void)                    { return T.m_iAsI8; }  // auto conversion
  
  void operator = (i8 iArg)            { T.m_iAsI8 = iArg; }  // assign from i8

  i8 iYear()                            { return C_YEAR_BASE + T.m_uYearDelta; }
  i8 iMonth()                           { return T.m_uMonth;  }
  i8 iDay()                             { return T.m_uDay;    }
  i8 iHour()                            { return T.m_uHour;   }
  i8 iMin()                             { return T.m_uMinute; }
  i8 iSec()                             { return T.m_uSecond; }
  i8 iMicro()                           { return T.m_uMicro;  }

  void vSet(i8 iYear, i8 iMonth,  i8 iDay,
            i8 iHour, i8 iMinute, i8 iSecond, i8 iMicro) {
                                        T.m_uYearDelta = iYear - C_YEAR_BASE;
                                        T.m_uMonth    = iMonth;
                                        T.m_uDay    = iDay;
                                        T.m_uHour    = iHour;
                                        T.m_uMinute    = iMinute;
                                        T.m_uSecond    = iSecond;
                                        T.m_uMicro    = iMicro;
                                        T.m_uLocal    = C_TSP_UTC;
                                        T.m_uSpare    = 0; }
void vSetMax(void)                    { T.m_iAsI8 = 0;
                                        T.m_uYearDelta = C_YEAR_RANGE;
                                        T.m_uMonth = 12;
                                        T.m_uDay = 31;
                                        T.m_uHour = 23;
                                        T.m_uMinute = 59;
                                        T.m_uSecond = 59;
                                        T.m_uMicro = 999999; }  // set max value
void vSetMid(void)                    { T.m_iAsI8 = 0;
                                        T.m_uYearDelta = C_YEAR_RANGE / 2;
                                        T.m_uMonth = 6;
                                        T.m_uDay = 30;
                                        T.m_uHour = 12;
                                        T.m_uMinute = 0;
                                        T.m_uSecond = 0;
                                        T.m_uMicro = 0; }  // set mid value
void vSetMin(void)                    { T.m_iAsI8 = 0;
                                        T.m_uYearDelta = 0;
                                        T.m_uMonth = 1;
                                        T.m_uDay = 1;
                                        T.m_uHour = 0;
                                        T.m_uMinute = 0;
                                        T.m_uSecond = 0;
                                        T.m_uMicro = 0; }  // set min value
  void vSetNull(void)          { T.m_iAsI8 = C_DATUM_NULL; }  // set to be null
  void vSetUsual(void)                                 { }  // set usual value

  bool bIsNull(void) { return (C_DATUM_NULL == T.m_iAsI8); }  // test for null

  // Tests if the given value is a valid time and if it is within range.
  // Writes to sErrorBuf if not
  static b1 bIsValid(i8 iYear, i8 iMonth, i8 iDay, i8 iHour, i8 iMin, i8 iSec, i8 iMicro, s256& sErrorBuf);
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// unknown
class Unk
{
public:
  union {
    i8 m_iInt;
    i8 m_iAsI8;
  };

  static i8 iMinValue(void)            { return C_I8_MAX; }
  static i8 iMaxValue(void)            { return C_I8_MIN; }

  operator i8(void)                    { return m_iAsI8; }  // auto conversion
  
  void operator = (i8 iArg)            { m_iAsI8 = iArg; }  // assign from i8
  
  void vSetMax(void)               { m_iAsI8 = C_I8_MAX; }  // set max value
  void vSetMid(void)               { m_iAsI8 = C_I8_MID; }  // set mid value
  void vSetMin(void)               { m_iAsI8 = C_I8_MIN; }  // set min value
  void vSetNull(void)          { m_iAsI8 = C_DATUM_NULL; }  // set to be null
  void vSetUsual(void)                   { m_iAsI8 = 42; }  // set usual value

  bool bIsNull(void) { return (C_DATUM_NULL == m_iAsI8); }  // test for null
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
