/*============================================================================*/
/* Encoding.hpp -- Row Encoding                                               */
/*============================================================================*/
#pragma once

namespace Data
{
  struct _Enc {
    eMC  m_eMetaCode;  // meta code for the col value
    Dtm  m_wDataCode;  // bytes of data
    i8   m_nDataSize;  // number of bytes of data
    i8   m_nOffset;    // byte offset in head of dataCode
    bool m_bOverflow;  // overflow at this point
  };
  
  //----------------------------------------------------------------------------
  
  class Encoding : public Object,
                   private RowCoder
  {
  public:
    ColumnVec* m_pColVec;
    i8         m_nColumns;
    i8         m_nBytesNeeded;
  
    // - - - - - - - - - - - -
  
    _Enc xaCol[C_MAX_COLUMNS];

    Wsp_t<C_MAX_RECORD> m_xWsp;  // sz workspace (multiple vch)
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);

    Encoding(ColumnVec* pColVec);
    ~Encoding(void) noexcept(false);
  
    i8   nBytesNeeded(void);
    void vEncode(Row* pRow);
    void vSplitOver(i8 nHeadSize, i8 nMetaBytes);

  private:
    void vEncodeBol(_Enc& xCol, Dtm wBol);
    void vEncodeBcd(_Enc& xCol, Dtm wBcd);
    void vEncodeDte(_Enc& xCol, Dtm wDte);
    void vEncodeFlt(_Enc& xCol, Dtm wFlt);
    void vEncodeInt(_Enc& xCol, Dtm wInt);
    void vEncodeIvl(_Enc& xCol, Dtm wIvl);
    void vEncodeTme(_Enc& xCol, Dtm wTme);
    void vEncodeTsp(_Enc& xCol, Dtm wTsp);
    void vEncodeVch(_Enc& xCol, Dtm wVch);
  
    void vReset(void);
  
  private:
    Footer* aFooter(void);
  };
  
  Encoding* _pEncoding(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
