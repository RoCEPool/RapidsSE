/*============================================================================*/
/* Dtm.hpp -- Complex Datum                                                   */
/*============================================================================*/
#pragma once

namespace BASE
{
  class Dtm
  {
//  friend class S T O R E::Encoding;
//  friend class S T O R E::SP_Encoding;
//  friend class S T O R E::CrwPacker;
//  friend class S  TO R E::Row;

  friend class BASE::SN_Bcd;
  friend class BASE::SN_Bol;
  friend class BASE::SN_Dte;
  friend class BASE::SN_Flt;
  friend class BASE::SN_Int;
  friend class BASE::SN_Ivl;
  friend class BASE::SN_Tme;
  friend class BASE::SN_Tsp;

  friend class BASE::SP_Bcd;
  friend class BASE::SP_Bol;
  friend class BASE::SP_Dte;
  friend class BASE::SP_Flt;
  friend class BASE::SP_Int;
  friend class BASE::SP_Ivl;
  friend class BASE::SP_Tme;
  friend class BASE::SP_Tsp;

  friend class BASE::Birth;
  friend class BASE::Now;
  
  friend class PRIMITIVES::Mutex;
  friend class BASE::IvlLib;
  

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

      Object*  m_pObject;

      CodeCell*  m_aFip;
      i8     m_iFip;

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
    /* get float column value                                                 */
    /*------------------------------------------------------------------------*/
    inline CodeCell* aGetColVal(void)
    {
      return m_aFip;
    }

   /*------------------------------------------------------------------------*/
    /* get float column value                                                 */
    /*------------------------------------------------------------------------*/
    inline Object* pGetColVal(void)
    {
      return m_pObject;
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
    /* put integer column value                                               */
    /*------------------------------------------------------------------------*/
    inline void vPutColVal(CodeCell* aFip)
    {
      m_aFip = aFip;
    }

    /*------------------------------------------------------------------------*/
    /* put integer column value                                               */
    /*------------------------------------------------------------------------*/
    inline void vPutColVal(Object* pObject)
    {
      m_pObject = pObject;
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
