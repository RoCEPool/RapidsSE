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
    static void vDecodeBcd(Column* pColumn, CacheLine* aCacheLine, WordView& wDecoded);
    static void vDecodeBol(Column* pColumn, CacheLine* aCacheLine, WordView& wDecoded);
    static void vDecodeDte(Column* pColumn, CacheLine* aCacheLine, WordView& wDecoded);
    static void vDecodeFlt(Column* pColumn, CacheLine* aCacheLine, WordView& wDecoded);
    static void vDecodeInt(Column* pColumn, CacheLine* aCacheLine, WordView& wDecoded);
    static void vDecodeIvl(Column* pColumn, CacheLine* aCacheLine, WordView& wDecoded);
    static void vDecodeTme(Column* pColumn, CacheLine* aCacheLine, WordView& wDecoded);
    static void vDecodeTsp(Column* pColumn, CacheLine* aCacheLine, WordView& wDecoded);
    static void vDecodeVch(Column* pColumn, CacheLine* aCacheLine, WordView& wDecoded);
  
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
  
  private:
    static i8 iDteDecoding(WordView& wTspView, i8 iEncoding);
    static i8 iTmeDecoding(WordView& wTspView, i8 iEncoding);
    static i8 iTspDecoding(WordView& wTspView, i8 iEncoding);
  
    static i8 iDteEncoding(WordView& wTspView);
    static i8 iTmeEncoding(WordView& wTspView);
    static i8 iTspEncoding(WordView& wTspView);

    static i8 iGetEncoding(i8 i_Column, CacheLine* aCacheLine, i8& i_Offset);
 };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
