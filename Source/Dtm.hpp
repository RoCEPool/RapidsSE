/*============================================================================*/
/* Dtm.hpp -- Abstract Datum                                                  */
/*============================================================================*/
#pragma once

namespace Values
{
  class Dtm
  {
  public:
    union {
      
      i8   m_iValue;
      
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      i8   m_iBcd;
      i8   m_iBol;
      i8   m_iDte;
      i8   m_iEdt;
      d8   m_dFlt;
      i8   m_iInt;
      i8   m_iIvl;
      i8   m_iTme;
      i8   m_iTsp;
      
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // date (since 1900)
//      struct {
//        u8 m_uSpare     : 47 ; //         47b
//
//        u8 m_uDay       :  5 ; // 0..31       same as xTsp.dte
//        u8 m_uMonth     :  4 ; // 0..15
//        u8 m_uYearDelta :  8 ; // 0..255  17b (1900+8b == 2155)
//                               //               ---
//                               //                64
//                               //               ===
//      } xDte;
      
      _Dte xDte;
      
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // era-based date
//      struct {
//
//
//        u8 m_uDay       :  5 ; // 0..31
//        u8 m_uMonth     :  4 ; // 0..15
//        u8 m_uYearInEra :  7 ; // 0..127  16b (eraBase+7b == 127)
//
//        u8 m_uSpare     : 48 ; //         48b
//                               //               ---
//                               //                64
//                               //               ===
//      } xEdt;

      _Edt xEdt;

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // interval (seconds & milliseconds)
//      struct {
//        u8 m_uMilli     : 10 ; // 0..1023
//        u8 m_uSecond    : 54 ; // 0..1.8E16
//                               //               ---
//                               //                64
//                               //               ===
//      } xIvl;
 
      _Ivl xIvl;

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // time (down to microseconds)
//      struct {
//        u8 m_uSpare     : 27 ; //         27b
//
//        u8 m_uMicro     : 20 ; // 0..1M       same as xTsp.tme
//        u8 m_uSecond    :  6 ; // 0..63
//        u8 m_uMinute    :  6 ; // 0..63
//        u8 m_uHour      :  5 ; // 0..31   37b
//                               //               ---
//                               //                64
//                               //               ===
//      } xTme;
 
      _Tme xTme;

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // timestamp (date & time)
//      struct {
//        u8 m_uSpare     : 10 ; //         10b
//
//        u8 m_uMicro     : 20 ; // 0..1M
//        u8 m_uSecond    :  6 ; // 0..63
//        u8 m_uMinute    :  6 ; // 0..63
//        u8 m_uHour      :  5 ; // 0..31   37b
//
//        u8 m_uDay       :  5 ; // 0..31
//        u8 m_uMonth     :  4 ; // 0..15
//        u8 m_uYearDelta :  8 ; // 0..255  17b  (1900+8b == 2155)
//                               //               ---
//                               //                64
//                               //               ===
//      } xTsp;
      
      _Tsp xTsp;

    };
  
  public:
    void* operator new(size_t);
  
    Dtm(void);
   ~Dtm(void) noexcept(false);
  
    static i8   nCopy(byte* ayDst, byte* aySrc);
    static i8   nCopy(byte* ayDst, i8 iValue);
    static void vCopy(byte* ayDst, byte* aySrc);
    static void vCopy(byte* ayDst, i8 iValue);

    static i8 iDte(i8 iYear, i8 iMonth, i8 iDay);
    static i8 iIvl(i8 iSecond, i8 iMilli);
    static i8 iTme(i8 iHour, i8 iSecond, i8 iMinute, i8 iMicro);
    static i8 iTsp(i8 iYear, i8 iMonth,  i8 iDay,
                   i8 iHour, i8 iSecond, i8 iMinute, i8 iMicro);
  
    i8   iAsI8(void);
    void vAsI8(i8 iAsI8);
    bool bIsNull(void);
    void vSetNull(void);
  };
  
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
