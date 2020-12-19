/*============================================================================*/
/* Dtm.hpp -- Complex Datum                                                   */
/*============================================================================*/
#pragma once

namespace Resources
{
  class Dtm
  {
  friend class CompactBuffers::Encoding_v2;
  friend class CompactBuffers::SP_Encoding_v2;
  friend class CompactBuffers::Packing_v2;
  
  friend class Datums::SN_Bcd;
  friend class Datums::SN_Bol;
  friend class Datums::SN_Dte;
  friend class Datums::SN_Flt;
  friend class Datums::SN_Int;
  friend class Datums::SN_Ivl;
  friend class Datums::SN_Tme;
  friend class Datums::SN_Tsp;

  friend class Datums::SP_Bcd;
  friend class Datums::SP_Bol;
  friend class Datums::SP_Dte;
  friend class Datums::SP_Flt;
  friend class Datums::SP_Int;
  friend class Datums::SP_Ivl;
  friend class Datums::SP_Tme;
  friend class Datums::SP_Tsp;

  friend class Resources::Birth;
  friend class Resources::Now;
  
  friend class Resources::Mutex;
  friend class Resources::IvlLib;
  
  friend class Rows::Row;

  private:
  
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
