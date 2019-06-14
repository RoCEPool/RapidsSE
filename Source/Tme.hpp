/*============================================================================*/
/* Tme.hpp -- time datum                                                      */
/*============================================================================*/
#pragma once

namespace Datums
{
  class Tme
  {
  public:
    union {
      struct {
        u8 m_uSpare  : 27 ; //
        
        u8 m_uMicro  : 20 ; // 0..1M
        u8 m_uSecond :  6 ; // 0..63
        u8 m_uMinute :  6 ; // 0..63
        u8 m_uHour   :  5 ; // 0..31
        //            ---
        //             64
        //            ===
      } x;
      i8 m_iValue;
    };
  
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

    static i8   nCopy(void* aDst, void* aSrc);
    static void vCopy(void* aDst, void* aSrc);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
