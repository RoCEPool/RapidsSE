/*============================================================================*/
/* Encoding.hpp -- Row Encoding                                               */
/*============================================================================*/
#pragma once

namespace Data
{
  class Encoding : public Object,
                   private RowCoder
  {
  public:
    ColumnVec* m_pColVec;
    i8         m_nColumns;
    i8         m_nBytesNeeded;
  
    // - - - - - - - - - - - -
  
    struct {
      eMC  m_eMetaCode;  // meta code for the col value
      Dtm  m_wDataCode;  // bytes of data
      i8   m_nDataSize;  // number of bytes of data
      i8   m_nOffset;    // byte offset in head of dataCode
      bool m_bOverflow;  // overflow at this point
    } xCol[C_MAX_COLUMNS];

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
    void vEncodeBol(Dtm wBol, eMC& eMetaCode_, Dtm& wDataCode_, i8& nDataBytes_);
    void vEncodeBcd(Dtm wBcd, eMC& eMetaCode_, Dtm& wDataCode_, i8& nDataBytes_);
    void vEncodeDte(Dtm wDte, eMC& eMetaCode_, Dtm& wDataCode_, i8& nDataBytes_);
    void vEncodeFlt(Dtm wFlt, eMC& eMetaCode_, Dtm& wDataCode_, i8& nDataBytes_);
    void vEncodeInt(Dtm wInt, eMC& eMetaCode_, Dtm& wDataCode_, i8& nDataBytes_);
    void vEncodeIvl(Dtm wIvl, eMC& eMetaCode_, Dtm& wDataCode_, i8& nDataBytes_);
    void vEncodeTme(Dtm wTme, eMC& eMetaCode_, Dtm& wDataCode_, i8& nDataBytes_);
    void vEncodeTsp(Dtm wTsp, eMC& eMetaCode_, Dtm& wDataCode_, i8& nDataBytes_);
    void vEncodeVch(Dtm wVch, eMC& eMetaCode_, Dtm& wDataCode_, i8& nDataBytes_);
  
    void vReset(void);
  
  private:
    Footer* aFooter(void);
  };
  
  Encoding* _pEncoding(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
