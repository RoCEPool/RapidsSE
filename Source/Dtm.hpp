/*============================================================================*/
/* Dtm.hpp -- Abstract Datum                                                  */
/*============================================================================*/
#pragma once

namespace Values
{
  class Dtm
  {
  public:
    union {
      
      Object* m_pObject;
      i8      m_nSize;    // size of stz
      i8      m_iValue;

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      i8      m_iBcd;
      i8      m_iBol;
      i8      m_iDte;
      i8      m_iEdt;
      d8      m_dFlt;
      i8      m_iInt;
      i8      m_iIvl;
      i8      m_iTme;
      i8      m_iTsp;
      
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      _Dte    xDte;
      _Edt    xEdt;
      _Ivl    xIvl;
      _Tme    xTme;
      _Tsp    xTsp;

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      Stz*    m_szVch;
    };
  
  public:
    void* operator new(size_t);
  
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
    bool bIsNull(void);
    void vSetNull(void);
  
    void operator =(i8 iInt)    { m_iInt  = iInt;  }
    void operator =(Stz* szVch) { m_szVch = szVch; }
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
