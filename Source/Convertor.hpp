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
  /*                               - EXTRACT -                                */
  /*--------------------------------------------------------------------------*/

  /*--------------------------------------------------------------------------*/
  /* extract boolean                                                          */
  /*--------------------------------------------------------------------------*/
  inline void vConvertBoolean(void)
  {
    while (*m_acChr && m_cRecEnd != *m_acChr && m_cFldSep != *m_acChr) {

      m_dCalc = (CALC) - 0.0;

      if (m_bTrace)
        vTrace("%c"_Stz,*m_acChr);
      m_acChr++;
      m_nChars++;
    }
  }

  /*--------------------------------------------------------------------------*/
  /* extract date                                                             */
  /*--------------------------------------------------------------------------*/
  inline void vConvertDate(void)
  {
    while (*m_acChr && m_cRecEnd != *m_acChr && m_cFldSep != *m_acChr) {

      m_dCalc = (CALC) - 1.0;

      if (m_bTrace)
        vTrace("%c"_Stz,*m_acChr);
      m_acChr++;
      m_nChars++;
    }
  }

  /*--------------------------------------------------------------------------*/
  /* extract date                                                             */
  /*--------------------------------------------------------------------------*/
  inline void vConvertDecimal(void)
  {
    while (*m_acChr && m_cRecEnd != *m_acChr && m_cFldSep != *m_acChr) {

      m_dCalc = (CALC) - 2.0;

      if (m_bTrace)
        vTrace("%c"_Stz,*m_acChr);
      m_acChr++;
      m_nChars++;
    }
  }

  /*--------------------------------------------------------------------------*/
  /* extract float                                                            */
  /*--------------------------------------------------------------------------*/
  inline void vConvertFloat(void)
  {
    while (*m_acChr && m_cRecEnd != *m_acChr && m_cFldSep != *m_acChr) {

      m_dCalc = (CALC) - 3.0;

      if (m_bTrace)
        vTrace("%c"_Stz,*m_acChr);
      m_acChr++;
      m_nChars++;
    }
  }

  /*--------------------------------------------------------------------------*/
  /* extract integer                                                          */
  /*--------------------------------------------------------------------------*/
  inline void vConvertInteger(void)
  {
    while (*m_acChr && m_cRecEnd != *m_acChr && m_cFldSep != *m_acChr) {

      m_dCalc = (CALC) - 4.0;

      if (m_bTrace)
        vTrace("%c"_Stz,*m_acChr);
      m_acChr++;
      m_nChars++;
    }
  }
  
  /*--------------------------------------------------------------------------*/
  /* extract interval                                                         */
  /*--------------------------------------------------------------------------*/
  inline void vConvertInterval(void)
  {
    while (*m_acChr && m_cRecEnd != *m_acChr && m_cFldSep != *m_acChr) {

      m_dCalc = (CALC) - 5.0;

      if (m_bTrace)
        vTrace("%c"_Stz,*m_acChr);
      m_acChr++;
      m_nChars++;
    }
  }
  
  /*--------------------------------------------------------------------------*/
  /* extract time                                                             */
  /*--------------------------------------------------------------------------*/
  inline void vConvertTime(void)
  {
    while (*m_acChr && m_cRecEnd != *m_acChr && m_cFldSep != *m_acChr) {

      m_dCalc = (CALC) - 6.0;

      if (m_bTrace)
        vTrace("%c"_Stz,*m_acChr);
      m_acChr++;
      m_nChars++;
    }
  }
  
  /*--------------------------------------------------------------------------*/
  /* extract timestamp                                                        */
  /*--------------------------------------------------------------------------*/
  inline void vConvertTimestamp(void)
  {
    while (*m_acChr && m_cRecEnd != *m_acChr && m_cFldSep != *m_acChr) {

      m_dCalc = (CALC) - 7.0;

      if (m_bTrace)
        vTrace("%c"_Stz,*m_acChr);
      m_acChr++;
      m_nChars++;
    }
  }
  
  /*--------------------------------------------------------------------------*/
  /* extract varchar                                                          */
  /*--------------------------------------------------------------------------*/
  inline void vConvertVarchar(void)
  {
    while (*m_acChr && m_cRecEnd != *m_acChr && m_cFldSep != *m_acChr) {

      m_dCalc = (CALC) - 8.0;

      if (m_bTrace)
        vTrace("%c"_Stz,*m_acChr);
      m_acChr++;
      m_nChars++;
    }
  }

  /*--------------------------------------------------------------------------*/
  /*                               - SUPPORT -                                */
  /*--------------------------------------------------------------------------*/

  /*--------------------------------------------------------------------------*/
  /* skip whitespace                                                          */
  /*--------------------------------------------------------------------------*/
  inline void vSkipWhitespace(void)
  {
  }

  
  };

  Convertor* _pConvertor(void* aAddr);

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
