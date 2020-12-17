/*============================================================================*/
/* Dtm.hpp -- Complex Datum                                                   */
/*============================================================================*/
#pragma once

namespace Resources
{
  class Dtm
  {
  public:
    union {

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
      
      i8    m_iWrd;
      d8    m_dWrd;
      char* m_azStr;
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
    void vSetUnknown(void);
    void vSetNull(void);

    void operator =(i8 iInt)    { m_iWrd  = iInt;  }
    void operator =(Stz* szVch) { m_szVch = szVch; }
  
    operator i8(void) { return m_iWrd; } // auto conversion to i8
  
    /*------------------------------------------------------------------------*/
    /* get float column value                                                 */
    /*------------------------------------------------------------------------*/
    inline d8 dGetColVal(void)
    {
      return m_wFlt;
    }

    /*------------------------------------------------------------------------*/
    /* get integer column value                                               */
    /*------------------------------------------------------------------------*/
    inline i8 iGetColVal(void)
    {
      return m_wInt;
    }

    /*------------------------------------------------------------------------*/
    /* get stz via offset                                                     */
    /*------------------------------------------------------------------------*/
    inline Stz* szGetColVal_offset(void)
    {
      Stz* StzBegin = aAdd<Stz*>(this,m_iWrd);
    
      return StzBegin;
    }

    /*------------------------------------------------------------------------*/
    /* test if null                                                           */
    /*------------------------------------------------------------------------*/
    inline bool bIsNull(void)
    {
      if (C_DATUM_NULL == m_iWrd)
        return true;
      else
        return false;
    }

    /*------------------------------------------------------------------------*/
    /* put float column value                                                 */
    /*------------------------------------------------------------------------*/
    inline void vPutColVal(d8 dColVal)
    {
      m_wFlt = dColVal;
    }

    /*------------------------------------------------------------------------*/
    /* put integer column value                                               */
    /*------------------------------------------------------------------------*/
    inline void vPutColVal(i8 iColVal)
    {
      m_wInt = iColVal;
    }

    /*------------------------------------------------------------------------*/
    /* put string column value                                                */
    /*------------------------------------------------------------------------*/
    inline i8 nPutColVal_offset(Stz* szColVal, Stz* StzBegin)
    {
      i8 nStringSize = StrLib::nCopy((char*)StzBegin,(char*)szColVal) + 1;

      vSetStz_offset(StzBegin);
    
      return nStringSize;
    }

    /*------------------------------------------------------------------------*/
    /* set stz offset                                                         */
    /*------------------------------------------------------------------------*/
    inline void vSetStz_offset(Stz* StzBegin)
    {
      m_iWrd = nDifference(this,StzBegin);
    }
  

  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
