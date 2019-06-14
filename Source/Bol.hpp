/*============================================================================*/
/* Bol.hpp -- boolean datum                                                   */
/*============================================================================*/
#pragma once

namespace Datums
{
  class Bol
  {
  friend class DtmLib;
  
  public:
    i8 m_bBol;
  
  public:
    void* operator new(size_t);
  
    Bol(void);
    ~Bol(void) noexcept(false);
  
    bool operator <  (Bol& xBol);
    bool operator <= (Bol& xBol);
    bool operator == (Bol& xBol);
    bool operator != (Bol& xBol);
    bool operator >= (Bol& xBol);
    bool operator >  (Bol& xBol);
  
    void vInitZero(void);
    i8   iAsI8(void);
    bool bIsNull(void);
    void vSetNull(void);
    void vSprint(Stz* aDst);

    static i8   nCopy(void* aDst, void* aSrc);
    static void vCopy(void* aDst, void* aSrc);
    static void vSprint(Stz* aDst, Stz* aSrc);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
