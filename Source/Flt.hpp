/*============================================================================*/
/* Flt.hpp -- float datum                                                     */
/*============================================================================*/
#pragma once

namespace Datums
{
  const d8 C_D8_MIN = DBL_MIN + DBL_EPSILON;
  const d8 C_D8_MAX = DBL_MAX;
  
  class Flt
  {
  friend class DtmLib;
    
  public:
    d8 m_dFlt;
  
  public:
    void* operator new(size_t);
  
    Flt(void);
    ~Flt(void) noexcept(false);
  
    bool operator <  (Flt& xFlt);
    bool operator <= (Flt& xFlt);
    bool operator == (Flt& xFlt);
    bool operator != (Flt& xFlt);
    bool operator >= (Flt& xFlt);
    bool operator >  (Flt& xFlt);
  
    void vInitZero(void);
    d8   dAsD8(void);
    bool bIsNull(void);
    void vSetNull(void);
    void vSprint(Stz* aDst);

    static i8   nCopy(void* aDst, void* aSrc);
    static void vCopy(void* aDst, void* aSrc);
    static void vSprint(Stz* aDst, Stz* aSrc);
    static bool bIsValidSize(i8 iPrecision);
    
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
