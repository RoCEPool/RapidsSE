/*============================================================================*/
/* Vch.hpp -- varchar datum                                                   */
/*============================================================================*/
#pragma once

namespace Datums
{
  const i8 C_MAX_SIZE_VCH = C_64KB;
  
  class Vch
  {
  public:
    void* operator new(size_t);
  
    Vch(void);
    ~Vch(void) noexcept(false);
      
    bool bIsNull(void);
    void vSetNull(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
