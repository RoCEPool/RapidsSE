/*============================================================================*/
/* Dtm.hpp -- Complex Datum                                                   */
/*============================================================================*/
#pragma once

namespace Library
{
  class Dtm
  {
  public:
    union {

      i8    m_iBcd;
      i8    m_iBol;
      i8    m_iDte;
      i8    m_iEdt;
      d8    m_dFlt;
      i8    m_iInt;
      i8    m_iIvl;
      i8    m_iTme;
      i8    m_iTsc;
      i8    m_iTsp;
      
      i8    m_iWrd;
      
      char* m_azStr;
      
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      Bcd   m_wBcd;
      Bol   m_wBol;
      Dte   m_wDte;
      Edt   m_wEdt;
      Flt   m_wFlt;
      Int   m_wInt;
      Ivl   m_wIvl;
      Tme   m_wTme;
      Tsp   m_wTsp;
      
      Stz*  m_szVch;

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      
      byte  m_yaByte[C_BYTES_PER_WORD];
    };
  
  public:
    Dtm(void);
   ~Dtm(void) noexcept(false);
  
    static i8   nCopy(byte* ayDst, byte* aySrc);
    static i8   nCopy(byte* ayDst, i8 iValue);
    static void vCopy(byte* ayDst, byte* aySrc);
    static void vCopy(byte* ayDst, i8 iValue);

    static i8   iDte(i8 iYear, i8 iMonth, i8 iDay);
    static i8   iIvl(i8 iSecond, i8 iMilli);
    static i8   iTme(i8 iHour, i8 iSecond, i8 iMinute, i8 iMicro);
    static i8   iTsp(i8 iYear, i8 iMonth,  i8 iDay,
                     i8 iHour, i8 iSecond, i8 iMinute, i8 iMicro);
  
    i8   iAsI8(void);
    void vAsI8(i8 iAsI8);
    i8   iByteMagn(void);
    bool bIsNull(void);
    void vSetUnknown(void);
    void vSetNull(void);

    void operator =(i8 iInt)    { m_iInt  = iInt;  }
    void operator =(Stz* szVch) { m_szVch = szVch; }
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
