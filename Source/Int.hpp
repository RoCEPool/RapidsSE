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
  const i8 C_MAX_PRECISION_INT = 19;
  
  class Int
  {
  friend class DtmLib;
  
  protected:
    i8 m_iInt;
  
  public:
    void* operator new(size_t);
  
    Int(void);
    ~Int(void) noexcept(false);
  
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
    static i8   nCopyVLQ(void* aDst, void* aSrc);
    static void vCopy(void* aDst, void* aSrc);
    static bool bExtract(char** aaSrc, Stz* aDst,
                         DBobjects::Column* pColumn, Formatters* pFormatters);
    static void vSprint(Stz* aDst, Stz* aSrc);
    static void vSprintVLQ(Stz* aDst, Stz* aSrc);
    
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
