/*============================================================================*/
/* Int.hpp -- int datum                                                       */
/*============================================================================*/
#pragma once

namespace Datums
{
  class Int
  {
  friend class DtmLib;
  
  private:
    i8 m_iInt;
  
  public:
    void* operator new(size_t);
  
    Int(void);
    ~Int(void);
  
    bool operator <  (Int& xInt);
    bool operator <= (Int& xInt);
    bool operator == (Int& xInt);
    bool operator != (Int& xInt);
    bool operator >= (Int& xInt);
    bool operator >  (Int& xInt);
  
    void vInitZero(void);
  
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
