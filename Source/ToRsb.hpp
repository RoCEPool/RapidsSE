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
    bool       m_bPtnHash;

    RowSlab*   m_pRowSlab;
    RsbBuffer* m_pRsbBuffer;

    bool       m_bOrdCsv;

    char       m_cRecEnd;
    char       m_cFldSep;

    bool       m_bFirst;

    // --
  
    char*      m_azSrc;
    char*      m_acChr;
  
    i8         m_iLine;
    i8         m_iCol;
    i8         m_i_Fld;
    i8         m_i_Rec;
        
    bool       m_bTrace;
  
  
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
  };

  ToRsb* _pToRsb(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
