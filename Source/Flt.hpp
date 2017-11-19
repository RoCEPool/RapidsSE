/*============================================================================*/
/* Flt.hpp -- float datum                                                     */
/*============================================================================*/
#pragma once

namespace Datums
{
  const i8 C_NULL_FLT = -DBL_MAX;
  
  class Flt
  {
  friend class DtmLib;
  
  protected:
    d8 m_dFlt;
  
  public:
    void* operator new(size_t);
  
    Flt(void);
    ~Flt(void);
  
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
  
    static i8   nCopy(void* aDst, void* aSrc);
    static void vCopy(void* aDst, void* aSrc);
    static bool bExtract(char** aaSrc, Flt* aFlt, DBobjects::Column* pColumn);
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
