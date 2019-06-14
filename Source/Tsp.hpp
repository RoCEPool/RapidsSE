/*============================================================================*/
/* Tsp.hpp -- timestamp datum                                                 */
/*============================================================================*/
#pragma once

namespace Datums
{
  class Tsp
  {
  friend class DtmLib;

  public:
    union {
      struct {
        u8 m_uSpare       : 10 ; //
        
        u8 m_uMicro       : 20 ; // 0..1M
        u8 m_uSecond      :  6 ; // 0..63
        u8 m_uMinute      :  6 ; // 0..63
        u8 m_uHour        :  5 ; // 0..31
        
        u8 m_uDay         :  5 ; // 0..31
        u8 m_uMonth       :  4 ; // 0..15
        u8 m_uYearDelta   :  8 ; // 0..255  (1900+8b == 2155)
        //                 ---
        //                  64
        //                 ===
      } x;
      i8 m_iValue;
    };
  
  public:
    void* operator new(size_t);
  
    Tsp(void);
    ~Tsp(void) noexcept(false);
      
    i8   iAsI8(void);
    void vAsI8(i8 iAsI8);
    bool bIsNull(void);
    void vSet(Tsp* aTspArg);
    void vSetAsNow(void);
    void vSetNull(void);
    void vSprint(Stz* szDst);

    i8   iYear(void);
    i8   iMonth(void);
    i8   iDay(void);
    i8   iHour(void);
    i8   iMinute(void);
    i8   iSecond(void);
    i8   iMicroseconds(void);
 
    static i8   nCopy(void* aDst, void* aSrc);
    static void vCopy(void* aDst, void* aSrc);
    static void vSprint(Stz* aDst, Stz* aSrc);
    static void vSprintEscaped(Stz* aDst, Stz* aSrc);
    static void vSprintNonSQL(Stz* aDst, Stz* aSrc);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
