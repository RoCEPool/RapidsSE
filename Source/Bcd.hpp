/*============================================================================*/
/* Bcd.hpp -- bcd datum                                                       */
/*============================================================================*/
#pragma once

namespace MetaData
{
  class Column;
}

namespace Datums
{
  class Bcd
  {
    
  public:
    i8 m_iBcd;
  
  public:
    void* operator new(size_t);
  
    Bcd(void);
    ~Bcd(void) noexcept(false);
  
    bool operator <  (Bcd& xBcd);
    bool operator <= (Bcd& xBcd);
    bool operator == (Bcd& xBcd);
    bool operator != (Bcd& xBcd);
    bool operator >= (Bcd& xBcd);
    bool operator >  (Bcd& xBcd);
  
    i8   iAsI8(void);
    void vAsI8(i8 iAsI8);
    void vInitZero(void);
    bool bIsNull(void);
    void vSetNull(void);
    void vSprint(Stz* aDst, MetaData::Column* pColumn);
    
    static i8   nCopy(void* aDst, void* aSrc);
    static void vCopy(void* aDst, void* aSrc);

    static void vSprint(Stz* aDst, Stz* aSrc, MetaData::Column* pColumn);
    static bool bIsValidSize(i8 iPrecision, i8 iScale);
    
    static bool bLessThan(char* aArgA, char* aArgB);
    static bool bLessEqual(char* aArgA, char* aArgB);
    static bool bEqualTo(char* aArgA, char* aArgB);
    static bool bNotEqual(char* aArgA, char* aArgB);
    static bool bGreaterEqual(char* aArgA, char* aArgB);
    static bool bGreaterThan(char* aArgA, char* aArgB);
    
    static i8 iCmp(char* aArgA, char* aArgB);  
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
