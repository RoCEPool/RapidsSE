/*============================================================================*/
/* Bol.hpp -- boolean datum                                                   */
/*============================================================================*/
#pragma once

namespace Datums
{
  class Bol
  {
  public:
    union {
      i8 m_bBol;
      i8 m_iValue;
    };

  public:
    void* operator new(size_t);
  
    Bol(void);
    ~Bol(void) noexcept(false);
    
    i8   iAsI8(void);
    bool bIsNull(void);
    void vSetNull(void);
    void vSprint(Stz* aDst);

    static i8   nCopy(void* aDst, void* aSrc);
    static void vCopy(void* aDst, void* aSrc);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
