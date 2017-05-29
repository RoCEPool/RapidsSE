/*============================================================================*/
/* Bcd.hpp -- bcd datum                                                       */
/*============================================================================*/
#pragma once

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
  
    void vInitZero(void);
    bool bIsNull(void);
    void vSetNull(void);
  
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
