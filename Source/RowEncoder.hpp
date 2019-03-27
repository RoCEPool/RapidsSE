/*============================================================================*/
/* RowEncoder.hpp -- Row Encoder                                              */
/*============================================================================*/
#pragma once

namespace Data
{
  static const i8 C_CACHELINES = 256;
  
  typedef TrayNL_t<CN_ROWTRAY,CacheLine,C_CACHELINES> RowParts;
  
  class RowEncoder : public Object
  {
  public:
    RowParts* m_pRowParts;
    i8        m_i_CacheOffset;
  
  // must be last
  Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    RowEncoder(RowParts* pRowParts);
    ~RowEncoder(void) noexcept(false);

  
    static void vAsString(s48K& sDataString,
                          ColumnVec* pColumnVec, CacheLine* aCacheLine);
    
    static w8 wDecode(Column* pColumn, CacheLine* aCacheLine);
  
    void vEncode(Column* pColumn, Datum* pDatum, i8& iHash);
  
    static void vPrint(Column* pColumn, CacheLine* aCacheLine);
  
  private:
    static w8 wDecodeBcd(Column* pColumn, CacheLine* aCacheLine);
    static w8 wDecodeBol(Column* pColumn, CacheLine* aCacheLine);
    static w8 wDecodeDte(Column* pColumn, CacheLine* aCacheLine);
    static w8 wDecodeFlt(Column* pColumn, CacheLine* aCacheLine);
    static w8 wDecodeInt(Column* pColumn, CacheLine* aCacheLine);
    static w8 wDecodeIvl(Column* pColumn, CacheLine* aCacheLine);
    static w8 wDecodeTme(Column* pColumn, CacheLine* aCacheLine);
    static w8 wDecodeTsp(Column* pColumn, CacheLine* aCacheLine);
    static w8 wDecodeVch(Column* pColumn, CacheLine* aCacheLine);
  
    void vEncodeBcd(Column* pColumn, Datum* pDatum, i8& iHash);
    void vEncodeBol(Column* pColumn, Datum* pDatum, i8& iHash);
    void vEncodeDte(Column* pColumn, Datum* pDatum, i8& iHash);
    void vEncodeFlt(Column* pColumn, Datum* pDatum, i8& iHash);
    void vEncodeInt(Column* pColumn, Datum* pDatum, i8& iHash);
    void vEncodeIvl(Column* pColumn, Datum* pDatum, i8& iHash);
    void vEncodeTme(Column* pColumn, Datum* pDatum, i8& iHash);
    void vEncodeTsp(Column* pColumn, Datum* pDatum, i8& iHash);
    void vEncodeVch(Column* pColumn, Datum* pDatum, i8& iHash);
  
    static i8 iBcdEncoding(WordView& wBcdView, i8& nBytesEncoded);
    static i8 iBolEncoding(WordView& wBolView, i8& nBytesEncoded);
    static i8 iDteEncoding(WordView& wTspView, i8& nBytesEncoded);
    static i8 iFltEncoding(WordView& wFltView, i8& nBytesEncoded);
    static i8 iIntEncoding(WordView& wIntView, i8& nBytesEncoded);
    static i8 iIvlEncoding(WordView& wIntView, i8& nBytesEncoded);
    static i8 iTmeEncoding(WordView& wTmeView, i8& nBytesEncoded);
    static i8 iTspEncoding(WordView& wTspView, i8& nBytesEncoded);
    static i8 iVchEncoding(WordView& wVchView, i8& nBytesEncoded);
  
    static void vPrint(Column* pColumn, WordView& wDatum);
  
    Footer*      aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
