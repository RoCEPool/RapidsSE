/*============================================================================*/
/* Bcd.hpp -- bcd datum                                                       */
/*============================================================================*/
#pragma once

namespace MetaData
{
  class Column;
}

namespace Datums
{
  class Bcd : public Dtm
  {
  public:

  public:
    void* operator new(size_t);
  
    Bcd(void);
    ~Bcd(void) noexcept(false);
    
    i8   iAsI8(void);
    void vAsI8(i8 iAsI8);
    bool bIsNull(void);
    void vSetNull(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
