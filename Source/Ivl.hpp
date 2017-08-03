/*============================================================================*/
/* Ivl.hpp -- interval datum                                                  */
/*============================================================================*/
#pragma once

namespace Datums
{
  class Ivl
  {
  friend class DtmLib;
  
  protected:
    struct {
      u8 m_uSecond : 54 ; // 0..1.8E16
      u8 m_uMilli  : 10 ; // 0..1023
      //           ---
      //            64
      //           ===
    } x;
  
  public:
    void* operator new(size_t);
  
    Ivl(void);
    ~Ivl(void);
  
    bool operator <  (Ivl& xIvl);
    bool operator <= (Ivl& xIvl);
    bool operator == (Ivl& xIvl);
    bool operator != (Ivl& xIvl);
    bool operator >= (Ivl& xIvl);
    bool operator >  (Ivl& xIvl);

    void vInitZero(void);
    i8   iAsI8(void);
    bool bIsNull(void);
    void vSetNull(void);
  
    i8   iSecond(void);
    i8   iMilli(void);
  
    void vSet(u8 uSecond, u8 uMilli);
  
    static i8   nCopy(void* aDst, void* aSrc);
    static void vCopy(void* aDst, void* aSrc);
#ifdef _ROWFORMAT
    static bool bExtract(char* aSrc, Ivl* aIvl, i8 iLength, DBobjects::Column* pColumn);
    static bool bValid(char* aSrc, i8 iLength, DBobjects::Column* pColumn);
#else
    static bool bExtract(char** aaSrc, Ivl* aIvl, DBobjects::Column* pColumn);
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
