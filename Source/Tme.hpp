/*============================================================================*/
/* Tme.hpp -- time datum                                                      */
/*============================================================================*/
#pragma once

namespace Datums
{
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

    static i8   nCopy(void* aDst, void* aSrc);
    static void vCopy(void* aDst, void* aSrc);
#ifdef _ROWFORMAT
    static bool bExtract(char* aSrc, Tme* aTme, i8 iLength, DBobjects::Column* pColumn);
    static bool bValid(char* aSrc, i8 iLength, DBobjects::Column* pColumn);
#else
    static bool bExtract(char** aaSrc, Tme* aTme, DBobjects::Column* pColumn);
    static bool bValid(char** aaSrc, DBobjects::Column* pColumn);
#endif
    static void vSprint(Stz* aDst, Stz* aSrc);
    
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
