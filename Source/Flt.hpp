/*============================================================================*/
/* Flt.hpp -- float datum                                                     */
/*============================================================================*/
#pragma once

namespace Datums
{
  class Flt : public Dtm
  {
  public:

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
