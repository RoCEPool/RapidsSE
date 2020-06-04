/*============================================================================*/
/* Convertor.hpp -- Csv Record Convertor                                      */
/*============================================================================*/
#pragma once

namespace Load
{
  class Convertor : public Object
  {
  private:
    StreamDef* m_pStreamDef;
    Format*    m_pFmt;

    char*      m_azSrc;
    char*      m_acChr;

    char       m_cRecEnd;
    char       m_cFldSep;
  
    i8         m_iLine;
    i8         m_iCol;
    i8         m_i_Fld;
    i8         m_i_Rec;
    
    Column*    m_pCol;
    eCT        m_eCol;
  
  
    bool       m_bTrace;
    d8         m_dCalc;
    i8         m_nChars;
    bool       m_bSawFldSep;
  
    Datum      m_oDatum;
  
    RowSlab*   m_pRowSlab;
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Convertor(StreamDef* pStreamDef, Format* pFmt);
   ~Convertor(void) noexcept(false);
  
    bool bConvert(char* azSrc);
    bool bInspect(char* azSrc);

  private:
    Footer* aFooter(void);

  #define CALC ((m_dCalc * 3.141 + (d8)*m_acChr) / 1.618)

  /*--------------------------------------------------------------------------*/
  /*                               - SUPPORT -                                */
  /*--------------------------------------------------------------------------*/

  /*--------------------------------------------------------------------------*/
  /* test for more data                                                       */
  /*--------------------------------------------------------------------------*/
  inline bool bMoreData(void)
  {
    bool bMore = (*m_acChr);
    
    return bMore;
  }
  
  /*--------------------------------------------------------------------------*/
  /* test for more fields                                                     */
  /*--------------------------------------------------------------------------*/
  inline bool bMoreFields(void)
  {
    bool bMore = (*m_acChr && m_cRecEnd != *m_acChr);
    
    return bMore;
  }
  
  /*--------------------------------------------------------------------------*/
  /* test for more fields                                                     */
  /*--------------------------------------------------------------------------*/
  inline bool bInField(void)
  {
    bool bMore = (*m_acChr && m_cFldSep != *m_acChr && m_cRecEnd != *m_acChr);
    
    return bMore;
  }
  
  /*--------------------------------------------------------------------------*/
  /* test blankspace                                                          */
  /*--------------------------------------------------------------------------*/
  inline bool bIsBlankspace(char cChr)
  {
    if (cChr == ' ')  return true;
    if (cChr == '\t') return true;
    
    return false;
  }
  
  /*--------------------------------------------------------------------------*/
  /* skip whitespace                                                          */
  /*--------------------------------------------------------------------------*/
  inline void vSkipBlankspace(void)
  {
    if ( ! (*m_acChr && bIsBlankspace(*m_acChr)) )
      return;

    if (m_bTrace)
      vTrace("Skipping: "_Stz);
  
    while (*m_acChr && StrLib::bIsWhite(*m_acChr)) {

      if (m_bTrace)
        vTrace("%c"_Stz,*m_acChr);
      
      m_acChr++;
      m_iCol++;
      m_nChars++;
    }
  
    if (m_bTrace)
      vTrace("\n"_Stz);
  }

  /*--------------------------------------------------------------------------*/
  /* extract boolean                                                          */
  /*--------------------------------------------------------------------------*/
  inline void vFieldTrace(void)
  {
    if (m_bTrace)
      vTrace("[%lld,%lld @%02lld,%02lld %10s] "_Stz,
             m_i_Rec,
             m_i_Fld,
             
             m_iLine,
             m_iCol,

             aCT[m_eCol]);
  }
  
  /*--------------------------------------------------------------------------*/
  /*                              - FIELD SPRINT -                            */
  /*--------------------------------------------------------------------------*/

  /*--------------------------------------------------------------------------*/
  /* sprint boolean                                                           */
  /*--------------------------------------------------------------------------*/
  inline void vSprintBoolean(void)
  {
    if (! m_bTrace)
      return;
  
    SP_Bol spBol;
  
    vTrace("Boolean : %s\n"_Stz,spBol.sz(m_oDatum.iBol()));
  }

  /*--------------------------------------------------------------------------*/
  /* sprint date                                                              */
  /*--------------------------------------------------------------------------*/
  inline void vSprintDate(void)
  {
    if (! m_bTrace)
      return;
  
    SP_Dte spDte;
  
    vTrace("Date : %s\n"_Stz,spDte.sz(m_oDatum.iDte()));
  }

  /*--------------------------------------------------------------------------*/
  /* sprint decimal                                                           */
  /*--------------------------------------------------------------------------*/
  inline void vSprintDecimal(void)
  {
    if (! m_bTrace)
      return;
  
    SP_Bcd spBcd;
  
    vTrace("Decimal : %s\n"_Stz,spBcd.sz(m_oDatum.iBcd(),m_pCol->iColumnScale()));
  }

  /*--------------------------------------------------------------------------*/
  /* sprint float                                                             */
  /*--------------------------------------------------------------------------*/
  inline void vSprintFloat(void)
  {
    if (! m_bTrace)
      return;
  
    SP_Flt spFlt;
  
    vTrace("Float : %s\n"_Stz,spFlt.sz(m_oDatum.dFlt()));
  }

  /*--------------------------------------------------------------------------*/
  /* sprint integer                                                           */
  /*--------------------------------------------------------------------------*/
  inline void vSprintInteger(void)
  {
    if (! m_bTrace)
      return;
  
    SP_Int spInt;
  
    vTrace("Integer : %s\n"_Stz,spInt.sz(m_oDatum.iInt()));
  }

  /*--------------------------------------------------------------------------*/
  /* sprint interval                                                          */
  /*--------------------------------------------------------------------------*/
  inline void vSprintInterval(void)
  {
    if (! m_bTrace)
      return;
  
    SP_Ivl spIvl;
  
    vTrace("Interval : %s\n"_Stz,spIvl.sz(m_oDatum.iIvl()));
  }

  /*--------------------------------------------------------------------------*/
  /* sprint time                                                              */
  /*--------------------------------------------------------------------------*/
  inline void vSprintTime(void)
  {
    if (! m_bTrace)
      return;
  
    SP_Tme spTme;
  
    vTrace("Time : %s\n"_Stz,spTme.sz(m_oDatum.iTme()));
  }

  /*--------------------------------------------------------------------------*/
  /* sprint timestamp                                                         */
  /*--------------------------------------------------------------------------*/
  inline void vSprintTimestamp(void)
  {
    if (! m_bTrace)
      return;
  
    SP_Tsp spTsp;
  
    vTrace("Timestamp : %s\n"_Stz,spTsp.sz(m_oDatum.iTsp()));
  }

  /*--------------------------------------------------------------------------*/
  /* sprint varchar                                                           */
  /*--------------------------------------------------------------------------*/
  inline void vSprintVarchar(void)
  {
    if (! m_bTrace)
      return;
  
    SP_Vch spVch;
  
    vTrace("Varchar : %s (%lldB)\n"_Stz,
           m_oDatum.szStz(),
           StrLib::nLength(m_oDatum.azChar())
           );
  }

  /*--------------------------------------------------------------------------*/
  /*                            - FIELD CONVERSION -                          */
  /*--------------------------------------------------------------------------*/

  /*--------------------------------------------------------------------------*/
  /* extract boolean                                                          */
  /*--------------------------------------------------------------------------*/
  inline void vConvertBoolean(void)
  {
    vFieldTrace();
               
    Bol xBol;
    SN_Bol snBol;

    xBol = snBol.i(&m_acChr,m_pCol,m_pFmt);
  
    m_pRowSlab->vDeposit(xBol);
    m_oDatum.vBol(xBol);

    vSprintBoolean();
  }

  /*--------------------------------------------------------------------------*/
  /* extract date                                                             */
  /*--------------------------------------------------------------------------*/
  inline void vConvertDate(void)
  {
    vFieldTrace();

    Dte xDte;
    SN_Dte snDte;

    xDte = snDte.i(&m_acChr,m_pCol,m_pFmt);
  
    m_pRowSlab->vDeposit(xDte);
    m_oDatum.vDte(xDte);

    vSprintDate();
  }

  /*--------------------------------------------------------------------------*/
  /* extract date                                                             */
  /*--------------------------------------------------------------------------*/
  inline void vConvertDecimal(void)
  {
    vFieldTrace();
  
    Bcd xBcd;
    SN_Bcd snBcd;

    xBcd = snBcd.i(&m_acChr,m_pCol,m_pFmt);
  
    m_pRowSlab->vDeposit(xBcd);
    m_oDatum.vBcd(xBcd);
  
    vSprintDecimal();
  }

  /*--------------------------------------------------------------------------*/
  /* extract float                                                            */
  /*--------------------------------------------------------------------------*/
  inline void vConvertFloat(void)
  {
    vFieldTrace();

    Flt xFlt;
    SN_Flt snFlt;

    xFlt = snFlt.d(&m_acChr,m_pCol,m_pFmt);
  
    m_pRowSlab->vDeposit(xFlt.m_dFlt);
    m_oDatum.vFlt(xFlt);

    vSprintFloat();
  }

  /*--------------------------------------------------------------------------*/
  /* extract integer                                                          */
  /*--------------------------------------------------------------------------*/
  inline void vConvertInteger(void)
  {
    vFieldTrace();

//    while (bInField()) {
//
//      m_dCalc = (CALC) - 4.0;
//
//      if (m_bTrace)
//        vTrace("%c"_Stz,*m_acChr);
//      m_acChr++;
//      m_iCol++;
//      m_nChars++;
//    }
  
//    i8 iInt;
//    StrLib::bExtractInteger(&m_acChr,&iInt,true);
//
//    m_oDatum.vInt(iInt);
  
    Int xInt;
    SN_Int snInt;

    xInt = snInt.i(&m_acChr,m_pCol,m_pFmt);
  
    m_pRowSlab->vDeposit(xInt);
    m_oDatum.vInt(xInt);

    vSprintInteger();
  }
  
  /*--------------------------------------------------------------------------*/
  /* extract interval                                                         */
  /*--------------------------------------------------------------------------*/
  inline void vConvertInterval(void)
  {
    vFieldTrace();

    Ivl xIvl;
    SN_Ivl snIvl;

    xIvl = snIvl.i(&m_acChr,m_pCol,m_pFmt);
  
    m_pRowSlab->vDeposit(xIvl);
    m_oDatum.vIvl(xIvl);

    vSprintInterval();
  }
  
  /*--------------------------------------------------------------------------*/
  /* extract time                                                             */
  /*--------------------------------------------------------------------------*/
  inline void vConvertTime(void)
  {
    vFieldTrace();

    Tme xTme;
    SN_Tme snTme;

    xTme = snTme.i(&m_acChr,m_pCol,m_pFmt);
  
    m_pRowSlab->vDeposit(xTme);
    m_oDatum.vTme(xTme);

    vSprintTime();
  }
  
  /*--------------------------------------------------------------------------*/
  /* extract timestamp                                                        */
  /*--------------------------------------------------------------------------*/
  inline void vConvertTimestamp(void)
  {
    vFieldTrace();

    Tsp xTsp;
    SN_Tsp snTsp;

    xTsp = snTsp.i(&m_acChr,m_pCol,m_pFmt);
  
    m_pRowSlab->vDeposit(xTsp);
    m_oDatum.vTsp(xTsp);

    vSprintTimestamp();
  }
  
  /*--------------------------------------------------------------------------*/
  /* extract varchar                                                          */
  /*--------------------------------------------------------------------------*/
  inline void vConvertVarchar(void)
  {
    vFieldTrace();
             
    SN_Vch snVch;

    i8 nChars = 0;
    bool bSawEnclosedLeading = CsvValue::bIsEnclosedString(&m_acChr,m_pFmt);

    m_oDatum = snVch.sz(&m_acChr,m_pCol,m_pFmt,bSawEnclosedLeading,nChars);
  
    char* szStr = m_pRowSlab->azDepositString();
    StrLib::vCopy(szStr,m_oDatum.azChar());
    m_pRowSlab->vDepositedString(nChars+1);
    ASSERT(nChars + 1 == StrLib::nSize(szStr));

    vSprintVarchar();
  }

  /*--------------------------------------------------------------------------*/
  /*                            - FIELD SUPPORT -                             */
  /*--------------------------------------------------------------------------*/

  /*--------------------------------------------------------------------------*/
  /* pre-field actions                                                        */
  /*--------------------------------------------------------------------------*/
  inline void vPreField(void)
  {
    if (CT_VARCHAR == m_eCol)
      return;
  
    vSkipBlankspace();
  }
  
  /*--------------------------------------------------------------------------*/
  /* post-field actions                                                       */
  /*--------------------------------------------------------------------------*/
  inline void vPostField(void)
  {
    if (m_cFldSep == *m_acChr) {
      m_bSawFldSep = true;
  
      if (m_bTrace)
        vTrace(" + fldSep\n"_Stz);
    }
  
    while (bInField()) {
      m_acChr++;
    }
  }
  
  }; // end class --------------------------------------------------------------

  Convertor* _pConvertor(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
