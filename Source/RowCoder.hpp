/*============================================================================*/
/* RowCoder.hpp -- Row Coder                                                  */
/*============================================================================*/
#pragma once

namespace Data
{  
  class RowCoder
  {
  public:
    static void vDecode(CacheLine xaCacheLine[], i8 i_Row,
                        Column* pCol, Datum* pDatum);
    static void vEncode(CacheLine xaCacheLine[], i8& i_Tail, i8& i_Offset,
                        Column* pColumn, Datum* pDatum, i8& iHash);
    static void vPrint(CacheLine xaCacheLine[], i8 i_Row, Column* pCol);
    static void vRecord(Record* pRec, CacheLine xaCacheLine[], i8 i_Row, ColumnVec* pColVec);
    static void vString(s48K& sString, ColumnVec* pColVec, CacheLine* aCacheLine);
  
  private:
    static void vAppend(Record* pRec, CacheLine xaCacheLine[], i8 i_Row, Column* pCol);
  
    static i8 nDataBytes(eMN eEncoding, eDM eDatum);
  
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
  
    static eMN eEncodingBcd(Datum* pDatum, i8& nBytesEncoded);
    static eMN eEncodingBol(Datum* pDatum, i8& nBytesEncoded);
    static eMN eEncodingDte(Datum* pDatum, i8& nBytesEncoded);
    static eMN eEncodingFlt(Datum* pDatum, i8& nBytesEncoded);
    static eMN eEncodingInt(Datum* pDatum, i8& nBytesEncoded);
    static eMN eEncodingIvl(Datum* pDatum, i8& nBytesEncoded);
    static eMN eEncodingTme(Datum* pDatum, i8& nBytesEncoded);
    static eMN eEncodingTsp(Datum* pDatum, i8& nBytesEncoded);
    static eMN eEncodingVch(Datum* pDatum, i8& nBytesEncoded);
  
    static eMN eGetEncoding(i8 i_Column, CacheLine* aCacheLine, i8& i_Offset);
  
    static void vInitCacheLine(CacheLine* aCacheLine);
  
    static void vRestoreDte(Datum* pDatum, eMN eEncoding);
    static void vRestoreTme(Datum* pDatum, eMN eEncoding);
    static void vRestoreTsp(Datum* pDatum, eMN eEncoding);
  
    static i8 iStripDte(WordView& wTspView, eMN eEncoding);
    static i8 iStripTme(WordView& wTspView, eMN eEncoding);
    static i8 iStripTsp(WordView& wTspView, eMN eEncoding);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
