/*============================================================================*/
/* Vch.hpp -- varchar datum                                                   */
/*============================================================================*/
#pragma once

namespace Datums
{
  const i8 C_MAX_SIZE_VCH = C_64KB;
  const i8 C_NULL_VCH = LLONG_MIN;
  
  class Vch
  {
  friend class DtmLib;
  private:
    
  public:
    void* operator new(size_t);
  
    Vch(void);
    ~Vch(void);
  
    bool operator <  (Vch& xVch);
    bool operator <= (Vch& xVch);
    bool operator == (Vch& xVch);
    bool operator != (Vch& xVch);
    bool operator >= (Vch& xVch);
    bool operator >  (Vch& xVch);
    
    void vInitZero(void);
    bool bIsNull(void);
    void vSetNull(void);
    
    static i8   nCopy(void* aDst, void* aSrc);
    static void vCopy(void* aDst, void* aSrc);
    static i8   iSize(char* aSrc);
    static void vSprint(Stz* aDst, Stz* aSrc);
    static bool bIsValidSize(i8 iSize);
    
    static bool bExtract(char** aaSrc, Vch* aVch,
                         DBobjects::Column* pColumn, Formatters* pFormatters);
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
