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

    static void vEncodeBcd(CacheLine xaCacheLine[], i8& i_Tail, i8& i_Offset,
                           Column* pColumn, Datum* pDatum, i8& iHash);
    static void vEncodeBol(CacheLine xaCacheLine[], i8& i_Tail, i8& i_Offset,
                           Column* pColumn, Datum* pDatum, i8& iHash);
    static void vEncodeDte(CacheLine xaCacheLine[], i8& i_Tail, i8& i_Offset,
                           Column* pColumn, Datum* pDatum, i8& iHash);
    static void vEncodeFlt(CacheLine xaCacheLine[], i8& i_Tail, i8& i_Offset,
                           Column* pColumn, Datum* pDatum, i8& iHash);
    static void vEncodeInt(CacheLine xaCacheLine[], i8& i_Tail, i8& i_Offset,
                           Column* pColumn, Datum* pDatum, i8& iHash);
    static void vEncodeIvl(CacheLine xaCacheLine[], i8& i_Tail, i8& i_Offset,
                           Column* pColumn, Datum* pDatum, i8& iHash);
    static void vEncodeTme(CacheLine xaCacheLine[], i8& i_Tail, i8& i_Offset,
                           Column* pColumn, Datum* pDatum, i8& iHash);
    static void vEncodeTsp(CacheLine xaCacheLine[], i8& i_Tail, i8& i_Offset,
                           Column* pColumn, Datum* pDatum, i8& iHash);
    static void vEncodeVch(CacheLine xaCacheLine[], i8& i_Tail, i8& i_Offset,
                           Column* pColumn, Datum* pDatum, i8& iHash);
  
    static i8 nEncodedBytes(i8 iEncoding, eDM eDatum);

    static i8 iEncodingBcd(Datum* pDatum, i8& nBytesEncoded);
    static i8 iEncodingBol(Datum* pDatum, i8& nBytesEncoded);
    static i8 iEncodingDte(Datum* pDatum, i8& nBytesEncoded);
    static i8 iEncodingFlt(Datum* pDatum, i8& nBytesEncoded);
    static i8 iEncodingInt(Datum* pDatum, i8& nBytesEncoded);
    static i8 iEncodingIvl(Datum* pDatum, i8& nBytesEncoded);
    static i8 iEncodingTme(Datum* pDatum, i8& nBytesEncoded);
    static i8 iEncodingTsp(Datum* pDatum, i8& nBytesEncoded);
    static i8 iEncodingVch(Datum* pDatum, i8& nBytesEncoded);
  
    static eMN eGetEncoding(i8 i_Column, CacheLine* aCacheLine, i8& i_Offset);

    static void vPrint(CacheLine xaCacheLine[], i8 i_Row, Column* pCol);
  
    static void vRecord(Record* pRec, CacheLine xaCacheLine[], i8 i_Row, ColumnVec* pColVec);
  
    static void vRestoreDte(Datum* pDatum, eMN eEncoding);
    static void vRestoreTme(Datum* pDatum, eMN eEncoding);
    static void vRestoreTsp(Datum* pDatum, eMN eEncoding);
  
    static i8 iStripDte(WordView& wTspView, i8 iEncoding);
    static i8 iStripTme(WordView& wTspView, i8 iEncoding);
    static i8 iStripTsp(WordView& wTspView, i8 iEncoding);
  
    static void vString(s48K& sString, ColumnVec* pColVec, CacheLine* aCacheLine);
  
  //private:
    static void vAppend(Record* pRec, CacheLine xaCacheLine[], i8 i_Row, Column* pCol);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
