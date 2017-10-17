/*============================================================================*/
/* Tsp.hpp -- timestamp datum                                                 */
/*============================================================================*/
#pragma once

namespace Datums
{
  class Tsp
  {
  friend class DtmLib;

  protected:
    struct {
      u8 m_uSpare  : 16 ; //
      u8 m_uMilli  : 10 ; // 0..1023
      u8 m_uSecond :  6 ; // 0..63
      u8 m_uMinute :  6 ; // 0..63
      u8 m_uHour   :  5 ; // 0..31
      u8 m_uDay    :  5 ; // 0..31
      u8 m_uMonth  :  4 ; // 0..15
      u8 m_uYear   : 12 ; // 0..4095
      //            ---
      //             64
      //            ===
    } x;
  
  public:
    void* operator new(size_t);
  
    Tsp(void);
    ~Tsp(void);
  
    bool operator <  (Tsp& xTsp);
    bool operator <= (Tsp& xTsp);
    bool operator == (Tsp& xTsp);
    bool operator != (Tsp& xTsp);
    bool operator >= (Tsp& xTsp);
    bool operator >  (Tsp& xTsp);
    
    void vInitZero(void);
    i8   iAsI8(void);
    bool bIsNull(void);
    void vSetNull(void);
  
    i8   iYear(void);
    i8   iMonth(void);
    i8   iDay(void);
    i8   iHour(void);
    i8   iMinute(void);
    i8   iSecond(void);
    i8   iMilli(void);
 
    static i8   nCopy(void* aDst, void* aSrc);
    static void vCopy(void* aDst, void* aSrc);
    static bool bExtract(char** aaSrc, Tsp* aTsp, DBobjects::Column* pColumn);
    static void vSprint(Stz* aDst, Stz* aSrc);
    static void vSprintNonSQL(Stz* aDst, Stz* aSrc);
    static bool bLessThan(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB);
    static bool bLessEqual(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB);
    static bool bEqualTo(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB);
    static bool bNotEqual(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB);
    static bool bGreaterEqual(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB);
    static bool bGreaterThan(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB);
    
    static i8 iCmp(char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
