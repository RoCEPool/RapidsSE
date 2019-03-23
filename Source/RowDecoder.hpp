/*============================================================================*/
/* RowDecoder.hpp -- Row Decoder                                              */
/*============================================================================*/
#pragma once

namespace Data
{  
  class RowDecoder : public Object
  {
  public:
    RowParts* m_pRowParts;
    i8        m_i_CacheOffset;
  
    RowTray*  m_pRowTray;            // tray of rows

  // must be last
  Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    RowDecoder(RowParts* pRowParts);
    ~RowDecoder(void) noexcept(false);

  
    static void vAsString(s48K& sDataString,
                          ColumnVec* pColumnVec, CacheLine* aCacheLine);
    
    static w8 wDecode(Column* pColumn, CacheLine* aCacheLine);
  
    void vEncodeDatum(Column* pColumn, byte* aDatum);
  
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
  
    void vEncodeBcd(Column* pColumn, byte* aDatum);
    void vEncodeBol(Column* pColumn, byte* aDatum);
    void vEncodeDte(Column* pColumn, byte* aDatum);
    void vEncodeFlt(Column* pColumn, byte* aDatum);
    void vEncodeInt(Column* pColumn, byte* aDatum);
    void vEncodeIvl(Column* pColumn, byte* aDatum);
    void vEncodeTme(Column* pColumn, byte* aDatum);
    void vEncodeTsp(Column* pColumn, byte* aDatum);
    void vEncodeVch(Column* pColumn, byte* aDatum);
  
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
