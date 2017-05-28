/*============================================================================*/
/* Tme.hpp -- time datum                                                      */
/*============================================================================*/
#pragma once

namespace Datums
{
  const i8 C_NULL_TME = LLONG_MIN;
  
  class Tme
  {
  friend class DtmLib;
  
  protected:
    struct {
      u8 m_uSpare  : 30 ; //
      u8 m_uMilli  : 10 ; // 0..1023
      u8 m_uSecond :  8 ; // 0..63
      u8 m_uMinute :  8 ; // 0..63
      u8 m_uHour   :  8 ; // 0..31
      //            ---
      //             64
      //            ===
    } x;
  
  public:
    void* operator new(size_t);
  
    Tme(void);
    ~Tme(void);
 
    bool operator <  (Tme& xTme);
    bool operator <= (Tme& xTme);
    bool operator == (Tme& xTme);
    bool operator != (Tme& xTme);
    bool operator >= (Tme& xTme);
    bool operator >  (Tme& xTme);
    
    void vInitZero(void);
    i8   iAsI8(void);
    bool bIsNull(void);
    void vSetNull(void);
 
    i8   iHour(void);
    i8   iMinute(void);
    i8   iSecond(void);
    i8   iMilli(void);
    static bool bExtract(Str* aDst, Str* aSrc, i8& nBytesTaken);
    static void vSprint(Str* aDst, Str* aSrc, i8& nBytesTaken);
    
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
