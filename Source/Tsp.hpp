/*============================================================================*/
/* Tsp.hpp -- timestamp datum                                                 */
/*============================================================================*/
#pragma once

namespace Datums
{
  class Tsp : public Dtm
  {
  public:
  
  public:
    void* operator new(size_t);
  
    Tsp(void);
    ~Tsp(void) noexcept(false);
      
    i8   iAsI8(void);
    void vAsI8(i8 iAsI8);
    bool bIsNull(void);
    void vSetNull(void);

    i8   iYear(void);
    i8   iMonth(void);
    i8   iDay(void);
    i8   iHour(void);
    i8   iMinute(void);
    i8   iSecond(void);
    i8   iMicroseconds(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
