/*============================================================================*/
/* Dte.hpp -- date datum                                                      */
/*============================================================================*/
#pragma once

namespace Datums
{
  class Dte
  {
  friend class DtmLib;
  
  public:
    union {
      struct {
        u8 m_uSpare      : 47 ; //
        
        u8 m_uDay        :  5 ; // 0..31
        u8 m_uMonth      :  4 ; // 0..15
        u8 m_uYearDelta  :  8 ; // 0..255  (1900+8b == 2155)
        //                 ---
        //                  64
        //                 ===
      } x;
      i8 m_iValue;
    };
  
  public:
    void* operator new(size_t);
  
    Dte(void);
    ~Dte(void) noexcept(false);
  
    i8   iAsI8(void);
    void vAsI8(i8 iAsI8);
    bool bIsNull(void);
    void vSetNull(void);
 
    i8   iYear(void);
    i8   iMonth(void);
    i8   iDay(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
