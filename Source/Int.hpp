/*============================================================================*/
/* Int.hpp -- int datum                                                       */
/*============================================================================*/
#pragma once

namespace MetaData
{
  class Column;
}

namespace Datums
{  
  class Int
  {
  friend class DtmLib;
  
  public:
    i8 m_iInt;
  
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
