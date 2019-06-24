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
    void vEncodeBol(Dtm wBol, Column* pCol);
    void vEncodeBcd(Dtm wBcd, Column* pCol);
    void vEncodeDte(Dtm wDte, Column* pCol);
    void vEncodeFlt(Dtm wFlt, Column* pCol);
    void vEncodeInt(Dtm wInt, Column* pCol);
    void vEncodeIvl(Dtm wIvl, Column* pCol);
    void vEncodeTme(Dtm wTme, Column* pCol);
    void vEncodeTsp(Dtm wTsp, Column* pCol);
    void vEncodeVch(Dtm wVch, Column* pCol);
  
    void vReset(void);
  
  private:
    Footer* aFooter(void);
  };
  
  Encoding* _pEncoding(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
