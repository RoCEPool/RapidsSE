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
  
    void vEncode(Column* pColumn, Datum* pDatum);
  
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
  
    void vEncodeBcd(Column* pColumn, Datum* pDatum);
    void vEncodeBol(Column* pColumn, Datum* pDatum);
    void vEncodeDte(Column* pColumn, Datum* pDatum);
    void vEncodeFlt(Column* pColumn, Datum* pDatum);
    void vEncodeInt(Column* pColumn, Datum* pDatum);
    void vEncodeIvl(Column* pColumn, Datum* pDatum);
    void vEncodeTme(Column* pColumn, Datum* pDatum);
    void vEncodeTsp(Column* pColumn, Datum* pDatum);
    void vEncodeVch(Column* pColumn, Datum* pDatum);
  
    static i8 iDteDecoding(WordView& wTspView, i8 iEncoding);
    static i8 iTmeDecoding(WordView& wTspView, i8 iEncoding);
    static i8 iTspDecoding(WordView& wTspView, i8 iEncoding);
  
    static i8 iBcdEncoding(WordView& wBcdView, i8& nBytesEncoded);
    static i8 iBolEncoding(WordView& wBolView, i8& nBytesEncoded);
    static i8 iDteEncoding(WordView& wTspView, i8& nBytesEncoded);
    static i8 iFltEncoding(WordView& wFltView, i8& nBytesEncoded);
    static i8 iIntEncoding(WordView& wIntView, i8& nBytesEncoded);
    static i8 iIvlEncoding(WordView& wIntView, i8& nBytesEncoded);
    static i8 iTmeEncoding(WordView& wTmeView, i8& nBytesEncoded);
    static i8 iTspEncoding(WordView& wTspView, i8& nBytesEncoded);
    static i8 iVchEncoding(WordView& wVchView, i8& nBytesEncoded);

    static i8 iGetEncoding(i8 i_Column, CacheLine* aCacheLine, i8& i_Offset);
  
    static i8 nEncodedBytes(i8 iEncoding, eDM eDatum);
 
    static void vPrint(Column* pColumn, WordView& wDatum);
  
    Footer*      aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
