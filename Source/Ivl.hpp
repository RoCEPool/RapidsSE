/*============================================================================*/
/* Ivl.hpp -- interval datum                                                  */
/*============================================================================*/
#pragma once

namespace Datums
{
  class Ivl : public Dtm
  {
  public:
  
  public:
    void* operator new(size_t);
  
    Ivl(void);
    ~Ivl(void) noexcept(false);
  
    i8   iAsI8(void);
    bool bIsNull(void);
    void vSetNull(void);
  
    i8   iSecond(void);
    i8   iMilli(void);
  
    void vSet(u8 uSecond, u8 uMilli);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
