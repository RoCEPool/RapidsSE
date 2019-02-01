/*============================================================================*/
/* Vch.hpp -- varchar datum                                                   */
/*============================================================================*/
#pragma once

namespace Datums
{
  const i8 C_MAX_SIZE_VCH = C_64KB;
  const i8 C_NULL_VCH = LLONG_MAX;
  
  class Vch
  {
  friend class DtmLib;
  private:
    
  public:
    void* operator new(size_t);
  
    Vch(void);
    ~Vch(void) noexcept(false);
  
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
    static void vSprintQuotes(Stz* aDst, Stz* aSrc);
    static void vSprintEscaped(Stz* aDst, Stz* aSrc);
    static bool bIsValidSize(i8 iSize);
    
    static bool bExtract(char** aaSrc, Stz* aDst,
                          DBsuper::Column* pColumn, Formatters* pFormatters);
    static bool bExtractDefault(char** aaSrc, Stz* aDst,
                          DBsuper::Column* pColumn, Formatters* pFormatters);
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
