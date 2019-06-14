/*============================================================================*/
/* Int.hpp -- int datum                                                       */
/*============================================================================*/
#pragma once

namespace Datums
{  
  class Int
  {
  public:
    union {
    i8 m_iInt;
      i8 m_iValue;
    };

  public:
    void* operator new(size_t);
  
    Int(void);
    ~Int(void) noexcept(false);
  
    bool bIsNull(void);

    static i8   nCopy(void* aDst, void* aSrc);

    static void vSprint(Stz* aDst, Stz* aSrc);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
