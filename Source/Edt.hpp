/*============================================================================*/
/* Edt.hpp -- era based date                                                  */
/*============================================================================*/
#pragma once

namespace Datums
{
  class Edt
  {
  friend class DtmLib;
  
  public:
    union {
      struct {
        u8 m_uSpare      : 48 ; //
        
        u8 m_uDay        :  5 ; // 0..31
        u8 m_uMonth      :  4 ; // 0..15
        u8 m_uYearInEra  :  7 ; // 0..127  16b (eraBase+7b == 127)
        //                 ---
        //                  64
        //                 ===
      } x;
      i8 m_iValue;
    };
  
  public:
    void* operator new(size_t);
  
    Edt(void);
    ~Edt(void) noexcept(false);
  
    i8   iAsI8(void);
    void vAsI8(i8 iAsI8);
    bool bIsNull(void);
    void vSetNull(void);
    void vSprint(Stz* szDst);

    i8   iYear(void);
    i8   iMonth(void);
    i8   iDay(void);
  
    static i8   nCopy(void* aDst, void* aSrc);
    static void vCopy(void* aDst, void* aSrc);
    static void vSprint(Stz* aDst, Stz* aSrc);
    static void vSprintEscaped(Stz* aDst, Stz* aSrc);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
