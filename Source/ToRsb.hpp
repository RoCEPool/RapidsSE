/*============================================================================*/
/* ToRsb.hpp -- Csv Record to Rsb                                             */
/*============================================================================*/
#pragma once

namespace Load
{
  static i8 iDaysPerMonth[12]     = { 31,28,31,30,31,30,31,31,30,31,30,31};
  static i8 iDaysPerMonthLeap[12] = { 31,29,31,30,31,30,31,31,30,31,30,31};
  
  class ToRsb : public Object
  {
  private:
    StreamDef* m_pStreamDef;
    Format*    m_pFmt;
    bool       m_bOrdCsv;

    char*      m_azSrc;
    char*      m_acChr;

    char       m_cRecEnd;
    char       m_cFldSep;
  
    i8         m_iLine;
    i8         m_iCol;
    i8         m_i_Fld;
    i8         m_i_Rec;
      
    Column*    m_pCol;
    bool       m_bPtnHash;
  
    bool       m_bTrace;
    d8         m_dCalc;
    i8         m_nChars;
    bool       m_bSawFldSep;
  
    Datum      m_oDatum;
  
    RowSlab*   m_pRowSlab;
    RsbBuffer* m_pRsbBuffer;
  
    bool       m_bFirst;

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    ToRsb(StreamDef* pStreamDef, Format* pFmt, bool bPtnHash);
   ~ToRsb(void) noexcept(false);
  
    bool bConvertCrw(CrwBuffer* pCrwBuffer);
    bool bConvertCsv(char* azSrc);

    RsbBuffer* pDanglingRsbBuffer(void);
    RsbBuffer* pFullRsbBuffer(void);
    void vNewRsbBuffer(void);

    RsbBuffer* pRsbBuffer(void);
    void vRsbBuffer(RsbBuffer* pRsbBuffer);
  

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
    if ( (*m_acChr && bIsBlankspace(*m_acChr)) ) {
      while (*m_acChr && StrLib::bIsWhite(*m_acChr)) {
        m_acChr++;
        m_iCol++;
        m_nChars++;
      }
    }
  }

  /*--------------------------------------------------------------------------*/
  /* extract boolean                                                          */
  /*--------------------------------------------------------------------------*/
  inline void vFieldTrace(eCT eCol)
  {
    if (m_bTrace)
      vTrace("[%lld,%lld @%02lld,%02lld %10s] "_Stz,
             m_i_Rec,
             m_i_Fld,
             
             m_iLine,
             m_iCol,

             aCT[eCol]);
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
  
    vTrace("Decimal : %s\n"_Stz,
           spBcd.sz(m_oDatum.iBcd(),
                    (i8)m_pRowSlab->axaColPix()[m_i_Fld].m_uScale));
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
  
  }; // end class --------------------------------------------------------------

  ToRsb* _pToRsb(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
