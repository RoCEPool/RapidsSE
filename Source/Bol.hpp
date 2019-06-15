/*============================================================================*/
/* Bol.hpp -- boolean datum                                                   */
/*============================================================================*/
#pragma once

namespace Datums
{
  class Bol : public Dtm
  {
  public:

  public:
    void* operator new(size_t);
  
    Bol(void);
    ~Bol(void) noexcept(false);
    
    i8   iAsI8(void);
    bool bIsNull(void);
    void vSetNull(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
