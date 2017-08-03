/*============================================================================*/
/* Int.hpp -- int datum                                                       */
/*============================================================================*/
#pragma once

namespace DBobjects
{
  class Column;
}

namespace Datums
{
  class Int
  {
  friend class DtmLib;
  
  protected:
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
    i8   iAsI8(void);
  
    bool bIsNull(void);
    void vSetNull(void);
  
    void vIntToVlq(char* aVlq);
    void vVlqToInt(char* aVlq);
 
    static i8   nCopy(void* aDst, void* aSrc);
    static void vCopy(void* aDst, void* aSrc);
#ifdef _ROWFORMAT
    static bool bExtract(char* aSrc, Int* aInt, i8 iLength, DBobjects::Column* pColumn);
#else
    static bool bExtract(char** aaSrc, Int* aInt, DBobjects::Column* pColumn);
#endif
    static void vSprint(Stz* aDst, Stz* aSrc);
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
