/*============================================================================*/
/* Bcd.hpp -- bcd datum                                                       */
/*============================================================================*/
#pragma once

namespace DBobjects
{
  class Column;
}

namespace Datums
{
  class Bcd
  {  
  protected:
    i8 m_iBcd;
  
  public:
    void* operator new(size_t);
  
    Bcd(void);
    ~Bcd(void);
  
    bool operator <  (Bcd& xBcd);
    bool operator <= (Bcd& xBcd);
    bool operator == (Bcd& xBcd);
    bool operator != (Bcd& xBcd);
    bool operator >= (Bcd& xBcd);
    bool operator >  (Bcd& xBcd);
  
    i8   iAsI8(void);
    void vInitZero(void);
    bool bIsNull(void);
    void vSetNull(void);
  
    static i8   nCopy(void* aDst, void* aSrc);
    static void vCopy(void* aDst, void* aSrc);
#ifdef _ROWFORMAT
    static bool bExtract(char* aSrc, Bcd* aBcd, i8 iLength, DBobjects::Column* pColumn);
#else
    static bool bExtract(char** aaSrc, Bcd* aBcd, DBobjects::Column* pColumn);
#endif
    static void vSprint(Str* aDst, Str* aSrc, DBobjects::Column* pColumn);
#ifdef _ROWFORMAT
    static bool bValid(char* aSrc, i8 iLength, DBobjects::Column* pColumn);
#else
    static bool bValid(char** aaSrc, DBobjects::Column* pColumn);
#endif
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
