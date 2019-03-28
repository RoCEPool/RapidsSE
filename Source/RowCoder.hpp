/*============================================================================*/
/* RowCoder.hpp -- Row Coder                                                  */
/*============================================================================*/
#pragma once

namespace Data
{  
  class RowCoder
  {
  public:
    static void vCacheLineInit(CacheLine* aCacheLine);
  
    static void vDecodeBcd(CacheLine xaCacheLine[], i8 i_Row,
                           Column* pCol, Datum* pDatum);
    static void vDecodeBol(CacheLine xaCacheLine[], i8 i_Row,
                           Column* pCol, Datum* pDatum);
    static void vDecodeDte(CacheLine xaCacheLine[], i8 i_Row,
                           Column* pCol, Datum* pDatum);
    static void vDecodeFlt(CacheLine xaCacheLine[], i8 i_Row,
                           Column* pCol, Datum* pDatum);
    static void vDecodeInt(CacheLine xaCacheLine[], i8 i_Row,
                           Column* pCol, Datum* pDatum);
    static void vDecodeIvl(CacheLine xaCacheLine[], i8 i_Row,
                           Column* pCol, Datum* pDatum);
    static void vDecodeTme(CacheLine xaCacheLine[], i8 i_Row,
                           Column* pCol, Datum* pDatum);
    static void vDecodeTsp(CacheLine xaCacheLine[], i8 i_Row,
                           Column* pCol, Datum* pDatum);
    static void vDecodeVch(CacheLine xaCacheLine[], i8 i_Row,
                           Column* pCol, Datum* pDatum);

    static i8 nEncodedBytes(i8 iEncoding, eDM eDatum);

    static eMN eGetEncoding(i8 i_Column, CacheLine* aCacheLine, i8& i_Offset);

    static w8 wRestoreDte(w8 wDteStripped, eMN eEncoding);
    static w8 wRestoreTme(w8 wTmeStripped, eMN eEncoding);
    static w8 wRestoreTsp(w8 wTspStripped, eMN eEncoding);
  
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

    static void vString(s48K& sString, ColumnVec* pColVec, CacheLine* aCacheLine);
  

  private:
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
