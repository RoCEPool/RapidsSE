/*============================================================================*/
/* Dte.hpp -- date datum                                                      */
/*============================================================================*/
#pragma once

namespace Datums
{
  class Dte : public Dtm
  {
  friend class DtmLib;
  
  public:
  
  public:
    void* operator new(size_t);
  
    Dte(void);
    ~Dte(void) noexcept(false);
  
    i8   iAsI8(void);
    void vAsI8(i8 iAsI8);
    bool bIsNull(void);
    void vSetNull(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
