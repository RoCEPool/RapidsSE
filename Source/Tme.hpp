/*============================================================================*/
/* Tme.hpp -- time datum                                                      */
/*============================================================================*/
#pragma once

namespace Datums
{
  class Tme : public Dtm
  {
  public:
  
  public:
    void* operator new(size_t);
  
    Tme(void);
    ~Tme(void) noexcept(false);
     
    i8   iAsI8(void);
    void vAsI8(i8 iAsI8);
    bool bIsNull(void);
    void vSetNull(void);

    i8   iHour(void);
    i8   iMinute(void);
    i8   iSecond(void);
    i8   iMicroseconds(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
