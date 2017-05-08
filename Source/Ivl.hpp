/*============================================================================*/
/* Ivl.hpp -- interval datum                                                  */
/*============================================================================*/
#pragma once

namespace Datums
{
  class Ivl
  {
  friend class DtmLib;
  private:
 
  struct {
    
    u8 m_uSec   : 54 ; // 0..1.8E16
    u8 m_uMsec  : 10 ; // 0..1023
    //           ---
    //            64
    //           ===
  } x;
  
  public:
    void* operator new(size_t);
  
    Ivl(void);
    ~Ivl(void);
  
    bool operator < (Ivl& xIvl);
  
    void vInitZero(void);
  
  void vSet(u8 uSec, u8 uMsec);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
