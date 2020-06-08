/*============================================================================*/
/* Encoding.hpp -- Row Encoding (intermediate representation of compact row)  */
/*============================================================================*/
#pragma once

namespace CompactBuffers
{
  // how a column is to be compacted
  struct _ColEnc {
    
    Column* m_pCol;    // column
    
    eMC  m_eMetaCode;  // meta code describing the column value
    Dtm  m_wDataCode;  // column value data bytes
    i8   m_nDataSize;  // complete number of data code bytes, incl <L> etc
    i8   m_i_Offset;   // first byte offset in head+over where the data code is
    i8   m_i_Till;     // last byte offset in head+over where the data code is
    bool m_bSplit;     // overflow occurs at this point
    
    i8   m_i_Tail;     // absolute index of tail
    
    TailLine *m_aTailLine;  // the tail line
  };
  
  //----------------------------------------------------------------------------
  
  class Encoding : public Object,
                   private RowCoder
  {
  public:
    StreamDef* m_pStreamDef;
  
    i8  m_nColumns;
    i8  m_nHeadSize;
    i8  m_nMetaSize;
    i8  m_i_BaseOffsetData;
  
    i8  m_nBytesNeeded;
    i8  m_nShiftOver;  // by how much a shift occurs
  
    i8  m_i_Offset_next;

    _ColEnc xaColEnc[C_MAX_COLUMNS];

    Wsp_t<C_MAX_RECORD> m_xWsp;  // sz workspace (multiple vch)
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Encoding(StreamDef* pStreamDef);
   ~Encoding(void) noexcept(false);
  
    i8   nBytesNeeded(void);
  
    void vEncodeIrw(Row* pIrw);
    void vEncodeRsb(Rsb* aRsb);
    void vEncodeSrw(RowSingle* pSrw);
  
    void vMorphEncoding(_ColEnc* aColEnc);
    bool bOverFlowed(void);
    i8   nOverSize(void);
    void vSplitOver(void);

  private:
    // encoding (structural order)
    void vInitialEncodingIrw(Row* pIrw);
    void vInitialEncodingSrw(RowSingle* pSrw);
    void vEncodeBcd(_ColEnc& xColEnc, Dtm wColVal);
    void vEncodeBol(_ColEnc& xColEnc, Dtm wColVal);
    void vEncodeDte(_ColEnc& xColEnc, Dtm wColVal);
    void vEncodeFlt(_ColEnc& xColEnc, Dtm wColVal);
    void vEncodeInt(_ColEnc& xColEnc, Dtm wColVal);
    void vEncodeIvl(_ColEnc& xColEnc, Dtm wColVal);
    void vEncodeTme(_ColEnc& xColEnc, Dtm wColVal);
    void vEncodeTsp(_ColEnc& xColEnc, Dtm wColVal);
    void vEncodeVch(_ColEnc& xColEnc, Dtm wColVal);

    bool bEncodeNul(_ColEnc& xColEnc, Dtm wDtm);
 
    void vOffsetTill(_ColEnc& xColEnc);

    // support functions
    Column* pNonFitting(void);
    void vReset(void);
    void vSplitAt(i8 i_Col);
    bool bWithinHead(i8 i_Col);
  
  private:
    Footer* aFooter(void);
  };
  
  Encoding* _pEncoding(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
