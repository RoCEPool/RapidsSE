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
  
    static void vDecode(CacheLine xaCacheLine[], i8 i_Row,
                        Column* pCol, Datum* pDatum);
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

    static void vRestoreDte(Datum* pDatum, eMN eEncoding);
    static void vRestoreTme(Datum* pDatum, eMN eEncoding);
    static void vRestoreTsp(Datum* pDatum, eMN eEncoding);
  
    static i8 iStripDte(WordView& wTspView, i8 iEncoding);
    static i8 iStripTme(WordView& wTspView, i8 iEncoding);
    static i8 iStripTsp(WordView& wTspView, i8 iEncoding);
  
    static i8 iBcdEncoding(Datum* pDatum, i8& nBytesEncoded);
    static i8 iBolEncoding(Datum* pDatum, i8& nBytesEncoded);
    static i8 iDteEncoding(Datum* pDatum, i8& nBytesEncoded);
    static i8 iFltEncoding(Datum* pDatum, i8& nBytesEncoded);
    static i8 iIntEncoding(Datum* pDatum, i8& nBytesEncoded);
    static i8 iIvlEncoding(Datum* pDatum, i8& nBytesEncoded);
    static i8 iTmeEncoding(Datum* pDatum, i8& nBytesEncoded);
    static i8 iTspEncoding(Datum* pDatum, i8& nBytesEncoded);
    static i8 iVchEncoding(Datum* pDatum, i8& nBytesEncoded);
  
    static void vString(s48K& sString, ColumnVec* pColVec, CacheLine* aCacheLine);
  

  private:
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
