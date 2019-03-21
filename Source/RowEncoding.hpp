/*============================================================================*/
/* RowEncoding.hpp -- Row Encoding                                            */
/*============================================================================*/
#pragma once

namespace Data
{
  class RowEncoding
  {
  public:
  
  public:
    static void vAsString(s48K& sDataString,
                          ColumnVec* pColumnVec, CacheLine* aCacheLine);
    
    static w8 wDecode(Column* pColumn, CacheLine* aCacheLine);
  
    static void vEncodeBegin(CacheLine* aCacheLine, i8& i_CacheOffset);
    static void vEncode(Column* pColumn, byte* aDatum,
                        CacheLine* xaCacheLine, i8& i_CacheOffset,
                        i8& nLinesUsed);
  
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
  
    static void vEncodeBcd(Column* pColumn, byte* aDatum,
                           CacheLine* xaCacheLine, i8& i_CacheOffset, i8& nLinesUsed);
    static void vEncodeBol(Column* pColumn, byte* aDatum,
                           CacheLine* xaCacheLine, i8& i_CacheOffset, i8& nLinesUsed);
    static void vEncodeDte(Column* pColumn, byte* aDatum,
                           CacheLine* xaCacheLine, i8& i_CacheOffset, i8& nLinesUsed);
    static void vEncodeFlt(Column* pColumn, byte* aDatum,
                           CacheLine* xaCacheLine, i8& i_CacheOffset, i8& nLinesUsed);
    static void vEncodeInt(Column* pColumn, byte* aDatum,
                           CacheLine* xaCacheLine, i8& i_CacheOffset, i8& nLinesUsed);
    static void vEncodeIvl(Column* pColumn, byte* aDatum,
                           CacheLine* xaCacheLine, i8& i_CacheOffset, i8& nLinesUsed);
    static void vEncodeTme(Column* pColumn, byte* aDatum,
                           CacheLine* xaCacheLine, i8& i_CacheOffset, i8& nLinesUsed);
    static void vEncodeTsp(Column* pColumn, byte* aDatum,
                           CacheLine* xaCacheLine, i8& i_CacheOffset, i8& nLinesUsed);
    static void vEncodeVch(Column* pColumn, byte* aDatum,
                           CacheLine* xaCacheLine, i8& i_CacheOffset, i8& nLinesUsed);
  
    static i8 iDteDecoding(WordView& wTspView, i8 iEncoding);
    static i8 iTmeDecoding(WordView& wTspView, i8 iEncoding);
    static i8 iTspDecoding(WordView& wTspView, i8 iEncoding);
  
    static i8 iDteEncoding(WordView& wTspView);
    static i8 iTmeEncoding(WordView& wTspView);
    static i8 iTspEncoding(WordView& wTspView);

    static i8 iGetEncoding(i8 i_Column, CacheLine* aCacheLine, i8& i_Offset);
  
    static i8 nEncodedBytes(i8 iEncoding, eDM eDatum);
 
    static void vPrint(Column* pColumn, WordView& wDatum);
};
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
