/*============================================================================*/
/* Vch.hpp -- varchar datum                                                   */
/*============================================================================*/
#pragma once

namespace Datums
{
  const i8 C_MAX_SIZE_VCH = C_64KB;
  
  class Vch
  {
  friend class DtmLib;
  private:
    
  public:
    void* operator new(size_t);
  
    Vch(void);
    ~Vch(void) noexcept(false);
      
    void vInitZero(void);
    bool bIsNull(void);
    void vSetNull(void);
    void vSprint(Stz* aDst, Stz* aSrc, bool bQuoted);

    static i8   nCopy(void* aDst, void* aSrc);
    static void vCopy(void* aDst, void* aSrc);
    static i8   iSize(char* aSrc);
    static void vSprint(Stz* aDst, Stz* aSrc);
    static void vSprintQuotes(Stz* aDst, Stz* aSrc);
    static void vSprintEscaped(Stz* aDst, Stz* aSrc);
    static bool bIsValidSize(i8 iSize);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
