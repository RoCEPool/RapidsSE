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
    static w8 wDecode(Column* pColumn, CacheLine* aCacheLine);
    static w8 wDecodeBcd(Column* pColumn, CacheLine* aCacheLine);
    static w8 wDecodeBol(Column* pColumn, CacheLine* aCacheLine);
    static w8 wDecodeDte(Column* pColumn, CacheLine* aCacheLine);
    static w8 wDecodeFlt(Column* pColumn, CacheLine* aCacheLine);
    static w8 wDecodeInt(Column* pColumn, CacheLine* aCacheLine);
    static w8 wDecodeIvl(Column* pColumn, CacheLine* aCacheLine);
    static w8 wDecodeTme(Column* pColumn, CacheLine* aCacheLine);
    static w8 wDecodeTsp(Column* pColumn, CacheLine* aCacheLine);
    static w8 wDecodeVch(Column* pColumn, CacheLine* aCacheLine);
  
    static void vEncodeBegin(CacheLine* aCacheLine, i8& i_CacheOffset);
    static void vEncode(Column* pColumn, byte* aDatum,
                        CacheLine* aCacheLine, i8& i_CacheOffset);
    static i8 iEncode(Column* pColumn, byte* aDatum, WordView& wEncoded);
    static i8 iEncodeBcd(Column* pColumn, byte* aDatum, WordView& wEncoded);
    static i8 iEncodeBol(Column* pColumn, byte* aDatum, WordView& wEncoded);
    static i8 iEncodeDte(Column* pColumn, byte* aDatum, WordView& wEncoded);
    static i8 iEncodeFlt(Column* pColumn, byte* aDatum, WordView& wEncoded);
    static i8 iEncodeInt(Column* pColumn, byte* aDatum, WordView& wEncoded);
    static i8 iEncodeIvl(Column* pColumn, byte* aDatum, WordView& wEncoded);
    static i8 iEncodeTme(Column* pColumn, byte* aDatum, WordView& wEncoded);
    static i8 iEncodeTsp(Column* pColumn, byte* aDatum, WordView& wEncoded);
    static i8 iEncodeVch(Column* pColumn, byte* aDatum, WordView& wEncoded);
  
    static void vPrint(Column* pColumn, WordView& wDatum);
  
  private:
    static i8 iDteDecoding(WordView& wTspView, i8 iEncoding);
    static i8 iTmeDecoding(WordView& wTspView, i8 iEncoding);
    static i8 iTspDecoding(WordView& wTspView, i8 iEncoding);
  
    static i8 iDteEncoding(WordView& wTspView);
    static i8 iTmeEncoding(WordView& wTspView);
    static i8 iTspEncoding(WordView& wTspView);

    static i8 iGetEncoding(i8 i_Column, CacheLine* aCacheLine, i8& i_Offset);
  
    static i8 nEncodedBytes(i8 iEncoding, eDM eDatum);
 };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
