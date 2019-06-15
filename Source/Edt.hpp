/*============================================================================*/
/* Edt.hpp -- era based date                                                  */
/*============================================================================*/
#pragma once

namespace Datums
{
  class Edt : public Dtm
  {
  friend class DtmLib;
  
  public:
  
  public:
    void* operator new(size_t);
  
    Edt(void);
    ~Edt(void) noexcept(false);
  
    i8   iAsI8(void);
    void vAsI8(i8 iAsI8);
    bool bIsNull(void);
    void vSetNull(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
