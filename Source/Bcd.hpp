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
    
    i8   iAsI8(void);
    void vAsI8(i8 iAsI8);
    bool bIsNull(void);
    void vSetNull(void);
    void vSprint(Stz* aDst, MetaData::Column* pColumn);
    
    static i8   nCopy(void* aDst, void* aSrc);
    static void vCopy(void* aDst, void* aSrc);

    static void vSprint(Stz* aDst, Stz* aSrc, MetaData::Column* pColumn);
    static bool bIsValidSize(i8 iPrecision, i8 iScale);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
