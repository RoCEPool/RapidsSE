/*============================================================================*/
/* Encoding_v2.hpp -- Row Encoding_v2 (intermediate representation of compact row)  */
/*============================================================================*/
#pragma once

namespace CompactBuffers
{
  // how a column is to be compacted
  struct _ColEnc_v2 {
    
    Column* m_pCol;    // column
    
    eMC  m_eMetaCode;  // meta code describing the column value
    eMN  m_eMetaNbl;   // meta nibble describing the column value
    Dtm  m_wDataCode;  // column value data bytes
    i8   m_nDataSize;  // complete number of data code bytes, incl <L> etc
    i8   m_i_Offset;   // first byte offset in head+over where the data code is
    i8   m_i_Till;     // last byte offset in head+over where the data code is
    bool m_bSplit;     // overflow occurs at this point
    
    i8   m_i_Tail;     // absolute index of tail
    
    TailLine *m_aTailLine;  // the tail line
  };
  
  //----------------------------------------------------------------------------
  
  class Encoding_v2 : public Object,
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

    _ColEnc_v2 xaColEnc[C_MAX_COLUMNS];

    SingleWord m__PadToCacheLine[1]; // padding to cache line

    Wsp_t<C_MAX_RECORD> m_xWsp;  // sz workspace (multiple vch)
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Encoding_v2(StreamDef* pStreamDef);
   ~Encoding_v2(void) noexcept(false);
  
    i8   nBytesNeeded(void);
  
    void vEncodeIrw(Row* pIrw);
    void vEncodeRsb(Rsb* aRsb);
    void vEncodeSrw(RowSingle* pSrw);
  
    void vMorphEncoding(_ColEnc_v2* aColEnc);
    bool bOverFlowed(void);
    i8   nOverSize(void);
    void vSplitOver(void);

  private:
    // encoding (structural order)
    void vInitialEncodingIrw(Row* pIrw);
    void vInitialEncodingRsb(Rsb* aRsb);
    void vInitialEncodingSrw(RowSingle* pSrw);
    void vEncodeBcd(_ColEnc_v2& xColEnc, Dtm wColVal);
    void vEncodeBol(_ColEnc_v2& xColEnc, Dtm wColVal);
    void vEncodeDte(_ColEnc_v2& xColEnc, Dtm wColVal);
    void vEncodeFlt(_ColEnc_v2& xColEnc, Dtm wColVal);
    void vEncodeInt(_ColEnc_v2& xColEnc, Dtm wColVal);
    void vEncodeIvl(_ColEnc_v2& xColEnc, Dtm wColVal);
    void vEncodeTme(_ColEnc_v2& xColEnc, Dtm wColVal);
    void vEncodeTsp(_ColEnc_v2& xColEnc, Dtm wColVal);
    void vEncodeVch(_ColEnc_v2& xColEnc, Dtm wColVal);

    bool bEncodeNul(_ColEnc_v2& xColEnc, Dtm wDtm);
 
    void vOffsetTill(_ColEnc_v2& xColEnc);

    // support functions
    Column* pNonFitting(void);
    void vReset(void);
    void vSplitAt(i8 i_Col);
    bool bWithinHead(i8 i_Col);
  
  private:
    Footer* aFooter(void);
  };
  
  Encoding_v2* _pEncoding_v2(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
