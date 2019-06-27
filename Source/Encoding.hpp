/*============================================================================*/
/* Encoding.hpp -- Row Encoding                                               */
/*============================================================================*/
#pragma once

namespace Rows
{
  // how a column is to be encoded
  struct _ColEnc {
    Column* m_pCol;   // column
    eDM  m_eDatum;    // column datum
    eMC  m_eMetaCode; // meta code describing the column value
    Dtm  m_wDataCode; // column value data bytes
    i8   m_nDataSize; // complete number of data code bytes, incl <L> etc
    i8   m_i_Offset;  // first byte offset in head+over where the data code is
    i8   m_i_Till;    // last byte offset in head+over where the data code is
    bool m_bSplit;    // overflow occurs at this point
    
    bool m_bAllotTail;// allot a string tail
    bool m_bUseTail;  // use string tail
    
    TailLine* m_aTailLine; // the alloted string tail
  };
  
  //----------------------------------------------------------------------------
  
  class Encoding : public Object,
                   private RowCoder
  {
  public:
    ColumnVec* m_pColVec;
    i8         m_nColumns;
    i8         m_nHeadSize;
    i8         m_nMetaSize;
  
    i8         m_nBytesNeeded;
    i8         m_nShiftOver;  // by how much a shift occurs

    _ColEnc xaColEnc[C_MAX_COLUMNS];

    Wsp_t<C_MAX_RECORD> m_xWsp;  // sz workspace (multiple vch)
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);

    Encoding(ColumnVec* pColVec, i8 nHeadSize, i8 nMetaSize);
    ~Encoding(void) noexcept(false);
  
    i8   nBytesNeeded(void);
    void vEncode(Row* pRow);
    bool bOverFlowed(void);

  private:
    void vEncodeBol(_ColEnc& xCol, Dtm wBol);
    void vEncodeBcd(_ColEnc& xCol, Dtm wBcd);
    void vEncodeDte(_ColEnc& xCol, Dtm wDte);
    void vEncodeFlt(_ColEnc& xCol, Dtm wFlt);
    void vEncodeInt(_ColEnc& xCol, Dtm wInt);
    void vEncodeIvl(_ColEnc& xCol, Dtm wIvl);
    void vEncodeTme(_ColEnc& xCol, Dtm wTme);
    void vEncodeTsp(_ColEnc& xCol, Dtm wTsp);
    void vEncodeVch(_ColEnc& xCol, Dtm wVch);
  
    void vInitialEncoding(Row* pRow);
    bool bNullEnc(_ColEnc& xCol, Dtm wDtm);
    void vReset(void);
    void vSplitAt(i8 i_Col);
    void vSplitOver(void);
    bool bWithinHead(i8 i_Col);
  
  private:
    Footer* aFooter(void);
  };
  
  Encoding* _pEncoding(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
