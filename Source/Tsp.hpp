/*============================================================================*/
/* Tsp.hpp -- timestamp datum                                                 */
/*============================================================================*/
#pragma once

namespace Datums
{
  class Tsp
  {
  friend class DtmLib;

  private:
  
  struct {
  
    u8 m_uYear   : 12 ; // 0..4095
    u8 m_uMonth  :  4 ; // 0..15
    u8 m_uDay    :  5 ; // 0..31
    u8 m_uHour   :  5 ; // 0..31
    u8 m_uMinute :  6 ; // 0..63
    u8 m_uSecond :  6 ; // 0..63
    u8 m_uMilli  : 10 ; // 0..1023
    u8 m_uSpare  : 16 ; //
    //            ---
    //             64
    //            ===
  };
  
  public:
    void* operator new(size_t);
  
    Tsp(void);
    ~Tsp(void);
  
    void vInitZero(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
