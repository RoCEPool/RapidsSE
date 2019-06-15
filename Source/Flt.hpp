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
  public:
    union {
      d8 m_dFlt;
      i8 m_iValue;
    };

  public:
    void* operator new(size_t);
  
    Flt(void);
    ~Flt(void) noexcept(false);
    
    d8   dAsD8(void);
    bool bIsNull(void);
    void vSetNull(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
