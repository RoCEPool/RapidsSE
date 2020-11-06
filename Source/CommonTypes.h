/*============================================================================*/
/* CommonTypes.h -- Common Types                                              */
/*============================================================================*/
#pragma once

typedef i8 tPN; // opaque type for partition number in grid

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// bcd / decimal
class Bcd
{
public:
  union {
    i8 m_iBcd;
    i8 m_iAsI8;
  };
  
  operator i8(void)                    { return m_iAsI8; }  // auto conversion
  
  void operator = (i8 iArg)            { m_iAsI8 = iArg; }  // assign from i8
  
  void vSetMax(void)                { m_iBcd = C_I8_MAX; }  // set max value
  void vSetMid(void)                { m_iBcd = C_I8_MID; }  // set mid value
  void vSetMin(void)                { m_iBcd = C_I8_MIN; }  // set min value
  void vSetNull(void)           { m_iBcd = C_DATUM_NULL; }  // set to be null
  void vSetUsual(void)                    { m_iBcd = 73; }  // set usual value

  bool bIsNull(void) { return (C_DATUM_NULL == m_iAsI8); }  // test for null
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
  
  operator i8(void)                    { return m_iAsI8; }  // auto conversion
  
  void operator = (i8 iArg)            { m_iAsI8 = iArg; }  // assign from i8
  
  void vSetMax(void)                    { m_iBol = true; }  // set max value
  void vSetMid(void)                    { m_iBol = true; }  // set mid value
  void vSetMin(void)                   { m_iBol = false; }  // set min value
  void vSetNull(void)          { m_iAsI8 = C_DATUM_NULL; }  // set to be null
  void vSetUsual(void)                  { m_iBol = true; }  // set usual value

  bool bIsNull(void) { return (C_DATUM_NULL == m_iAsI8); }  // test for null
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// date (since 1900)
class Dte
{
public:
  union {
    struct {
      u8 m_uDay       :  5 ; // 0..31       same as _Tsp.dte
      u8 m_uMonth     :  4 ; // 0..15
      u8 m_uYearDelta :  8 ; // 0..255  17b (1900+8b == 2155)
      
      u8 m_uSpare     : 47 ; //         47b
      //               ---
      //                64
      //               ===
    };
    i8 m_iAsI8;
  };
  
  operator i8(void)                    { return m_iAsI8; }  // auto conversion
  
  void operator = (i8 iArg)            { m_iAsI8 = iArg; }  // assign from i8
  
  void vSetMax(void)                      { m_iAsI8 = 0;
                            m_uYearDelta = C_YEAR_RANGE;
                                          m_uMonth = 12;
                                            m_uDay = 31; }  // set max value
  void vSetMid(void)                      { m_iAsI8 = 0;
                        m_uYearDelta = C_YEAR_RANGE / 2;
                                           m_uMonth = 6;
                                            m_uDay = 30; }  // set mid value
  void vSetMin(void)                      { m_iAsI8 = 0;
                                       m_uYearDelta = 0;
                                           m_uMonth = 1;
                                             m_uDay = 1; }  // set min value
  void vSetNull(void)          { m_iAsI8 = C_DATUM_NULL; }  // set to be null
  void vSetUsual(void)                    { m_iAsI8 = 0;
                                      m_uYearDelta = 35;
                                          m_uMonth = 11;
                                            m_uDay = 29; }  // set usual value

  bool bIsNull(void) { return (C_DATUM_NULL == m_iAsI8); }  // test for null
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
  
  operator i8(void)                    { return m_iAsI8; }  // auto conversion
  
  void operator = (i8 iArg)            { m_iAsI8 = iArg; }  // assign from i8
  
  void vSetMax(void)                                   { }  // set max value
  void vSetMid(void)                                   { }  // set mid value
  void vSetMin(void)                                   { }  // set min value
  void vSetNull(void)          { m_iAsI8 = C_DATUM_NULL; }  // set to be null
  void vSetUsual(void)                                 { }  // set usual value

  bool bIsNull(void) { return (C_DATUM_NULL == m_iAsI8); }  // test for null
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
  
  
  operator d8(void)                    { return m_dFlt;  }  // auto conversion
  operator i8(void)                    { return m_iAsI8; }  // auto conversion
  
  void operator = (i8 iArg)            { m_iAsI8 = iArg; }  // assign from i8
  void operator = (d8 dArg)            { m_dFlt  = dArg; }  // assign from d8

  void vSetMax(void)                { m_dFlt = C_D8_MAX; }  // set max value
  void vSetMid(void)                { m_dFlt = C_D8_MID; }  // set mid value
  void vSetMin(void)                { m_dFlt = C_D8_MIN; }  // set min value
  void vSetNull(void)          { m_iAsI8 = C_DATUM_NULL; }  // set to be null
  void vSetUsual(void)                 { m_dFlt = 3.141; }  // set usual value

  bool bIsNull(void) { return (C_DATUM_NULL == m_iAsI8); }  // test for null
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// integer
class Int
{
public:
  union {
    i8 m_iInt;
    i8 m_iAsI8;
  };
  
  operator i8(void)                    { return m_iAsI8; }  // auto conversion
  
  void operator = (i8 iArg)            { m_iAsI8 = iArg; }  // assign from i8
  
  void vSetMax(void)               { m_iAsI8 = C_I8_MAX; }  // set max value
  void vSetMid(void)               { m_iAsI8 = C_I8_MID; }  // set mid value
  void vSetMin(void)               { m_iAsI8 = C_I8_MIN; }  // set min value
  void vSetNull(void)          { m_iAsI8 = C_DATUM_NULL; }  // set to be null
  void vSetUsual(void)                   { m_iAsI8 = 42; }  // set usual value

  bool bIsNull(void) { return (C_DATUM_NULL == m_iAsI8); }  // test for null
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
  
  operator i8(void)                    { return m_iAsI8; }  // auto conversion
  
  void operator = (i8 iArg)            { m_iAsI8 = iArg; }  // assign from i8
  
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
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// timestamp
class Tsp
{
public:
  union {
    struct {
      u8 m_uLocal     :  1 ; //        1b
      
      u8 m_uMicro     : 20 ; // 0..1M
      u8 m_uSecond    :  6 ; // 0..63
      u8 m_uMinute    :  6 ; // 0..63
      u8 m_uHour      :  5 ; // 0..31   37b
      
      u8 m_uDay       :  5 ; // 0..31
      u8 m_uMonth     :  4 ; // 0..15
      u8 m_uYearDelta :  8 ; // 0..255  17b  (1900+8b == 2155)
      
      u8 m_uSpare     :  9 ; //        9b
      //               ---
      //                64
      //               ===
    };
    i8 m_iAsI8;
  };
  
  operator i8(void)                    { return m_iAsI8; }  // auto conversion
  
  void operator = (i8 iArg)            { m_iAsI8 = iArg; }  // assign from i8
  
  void vSetMax(void)                      { m_iAsI8 = 0;
                            m_uYearDelta = C_YEAR_RANGE;
                                          m_uMonth = 12;
                                            m_uDay = 31;
                                           m_uHour = 23;
                                         m_uMinute = 59;
                                         m_uSecond = 59;
                                      m_uMicro = 999999; }  // set max value
  void vSetMid(void)                      { m_iAsI8 = 0;
                        m_uYearDelta = C_YEAR_RANGE / 2;
                                           m_uMonth = 6;
                                            m_uDay = 30;
                                           m_uHour = 12;
                                          m_uMinute = 0;
                                          m_uSecond = 0;
                                           m_uMicro = 0; }  // set mid value
  void vSetMin(void)                      { m_iAsI8 = 0;
                                       m_uYearDelta = 0;
                                           m_uMonth = 1;
                                             m_uDay = 1;
                                            m_uHour = 0;
                                          m_uMinute = 0;
                                          m_uSecond = 0;
                                           m_uMicro = 0; }  // set min value
  void vSetNull(void)          { m_iAsI8 = C_DATUM_NULL; }  // set to be null
  void vSetUsual(void)                                 { }  // set usual value

  bool bIsNull(void) { return (C_DATUM_NULL == m_iAsI8); }  // test for null
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
