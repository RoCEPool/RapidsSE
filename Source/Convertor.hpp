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
  
    Datum      m_oDatum;
  
    bool       m_bTrace;
    d8         m_dCalc;
    i8         m_nChars;
  
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
  /*                            - FIELD CONVERSION -                          */
  /*--------------------------------------------------------------------------*/

  /*--------------------------------------------------------------------------*/
  /* extract boolean                                                          */
  /*--------------------------------------------------------------------------*/
  inline void vConvertBoolean(void)
  {
    vFieldTrace();
             
    while (bInField()) {

      m_dCalc = (CALC) - 0.0;

      if (m_bTrace)
        vTrace("%c"_Stz,*m_acChr);
      m_acChr++;
      m_iCol++;
      m_nChars++;
    }
  }

  /*--------------------------------------------------------------------------*/
  /* extract date                                                             */
  /*--------------------------------------------------------------------------*/
  inline void vConvertDate(void)
  {
    vFieldTrace();

    while (bInField()) {

      m_dCalc = (CALC) - 1.0;

      if (m_bTrace)
        vTrace("%c"_Stz,*m_acChr);
      m_acChr++;
      m_iCol++;
      m_nChars++;
    }
  }

  /*--------------------------------------------------------------------------*/
  /* extract date                                                             */
  /*--------------------------------------------------------------------------*/
  inline void vConvertDecimal(void)
  {
    vFieldTrace();

    while (bInField()) {

      m_dCalc = (CALC) - 2.0;

      if (m_bTrace)
        vTrace("%c"_Stz,*m_acChr);
      m_acChr++;
      m_iCol++;
      m_nChars++;
    }
  }

  /*--------------------------------------------------------------------------*/
  /* extract float                                                            */
  /*--------------------------------------------------------------------------*/
  inline void vConvertFloat(void)
  {
    vFieldTrace();

    while (bInField()) {

      m_dCalc = (CALC) - 3.0;

      if (m_bTrace)
        vTrace("%c"_Stz,*m_acChr);
      m_acChr++;
      m_iCol++;
      m_nChars++;
    }
  }

  /*--------------------------------------------------------------------------*/
  /* extract integer                                                          */
  /*--------------------------------------------------------------------------*/
  inline void vConvertInteger(void)
  {
    vFieldTrace();

    while (bInField()) {

      m_dCalc = (CALC) - 4.0;

      if (m_bTrace)
        vTrace("%c"_Stz,*m_acChr);
      m_acChr++;
      m_iCol++;
      m_nChars++;
    }
  }
  
  /*--------------------------------------------------------------------------*/
  /* extract interval                                                         */
  /*--------------------------------------------------------------------------*/
  inline void vConvertInterval(void)
  {
    vFieldTrace();

    while (bInField()) {

      m_dCalc = (CALC) - 5.0;

      if (m_bTrace)
        vTrace("%c"_Stz,*m_acChr);
      m_acChr++;
      m_iCol++;
      m_nChars++;
    }
  }
  
  /*--------------------------------------------------------------------------*/
  /* extract time                                                             */
  /*--------------------------------------------------------------------------*/
  inline void vConvertTime(void)
  {
    vFieldTrace();

    while (bInField()) {

      m_dCalc = (CALC) - 6.0;

      if (m_bTrace)
        vTrace("%c"_Stz,*m_acChr);
      m_acChr++;
      m_iCol++;
      m_nChars++;
    }
  }
  
  /*--------------------------------------------------------------------------*/
  /* extract timestamp                                                        */
  /*--------------------------------------------------------------------------*/
  inline void vConvertTimestamp(void)
  {
    vFieldTrace();

    while (bInField()) {

      m_dCalc = (CALC) - 7.0;

      if (m_bTrace)
        vTrace("%c"_Stz,*m_acChr);
      m_acChr++;
      m_iCol++;
      m_nChars++;
    }
  }
  
  /*--------------------------------------------------------------------------*/
  /* extract varchar                                                          */
  /*--------------------------------------------------------------------------*/
  inline void vConvertVarchar(void)
  {
    vFieldTrace();
             
    while (bInField()) {

      m_dCalc = (CALC) - 8.0;

      if (m_bTrace)
        vTrace("%c"_Stz,*m_acChr);
      m_acChr++;
      m_iCol++;
      m_nChars++;
    }
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
