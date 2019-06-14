/*============================================================================*/
/* Ivl.hpp -- interval datum                                                  */
/*============================================================================*/
#pragma once

namespace Datums
{
  class Ivl
  {
  public:
    union {
      struct {
        u8 m_uMilli  : 10 ; // 0..1023
        u8 m_uSecond : 54 ; // 0..1.8E16
        //            ---
        //             64
        //            ===
      } x;
      i8 m_iValue;
    };
  
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
    void vSprint(Stz* aDst);

    static i8   nCopy(void* aDst, void* aSrc);
    static void vCopy(void* aDst, void* aSrc);
    static void vSprint(Stz* aDst, Stz* aSrc);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
