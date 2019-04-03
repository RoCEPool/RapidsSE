/*============================================================================*/
/* RowCoder.hpp -- Row Coder                                                  */
/*============================================================================*/
#pragma once

namespace Data
{  
  class RowCoder
  {
  public:
    static void vCopy(CacheLine* pIntoHead, CacheLine* pIntoTails,
                      CacheLine xaFromLine[],
                      ColumnVec* pColVec);
  
    static void vDecode(CacheLine xaCacheLine[], i8 i_Row,
                        Column* pCol, Datum* pDatum);
  
    static i8   nLines(CacheLine xaCacheLine[], i8 i_Row, ColumnVec* pColVec);
  
    static void vEncode(RowNub* pRowNub,
                        i8& i_Tail, i8& i_Offset,
                        Column* pColumn, Datum* pDatum, i8& iHash);
  
    static void vPrint(CacheLine xaCacheLine[], i8 i_Row, Column* pCol);
  
    static void vRecord(Record* pRec, CacheLine xaCacheLine[], i8 i_Row,
                        ColumnVec* pColVec);
  
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
  
    static void vEncodeBcd(RowNub* pRowNub,
                           i8& i_Tail, i8& i_Offset,
                           Column* pColumn, Datum* pDatum, i8& iHash);
    static void vEncodeBol(RowNub* pRowNub,
                           i8& i_Tail, i8& i_Offset,
                           Column* pColumn, Datum* pDatum, i8& iHash);
    static void vEncodeDte(RowNub* pRowNub,
                           i8& i_Tail, i8& i_Offset,
                           Column* pColumn, Datum* pDatum, i8& iHash);
    static void vEncodeFlt(RowNub* pRowNub,
                           i8& i_Tail, i8& i_Offset,
                           Column* pColumn, Datum* pDatum, i8& iHash);
    static void vEncodeInt(RowNub* pRowNub,
                           i8& i_Tail, i8& i_Offset,
                           Column* pColumn, Datum* pDatum, i8& iHash);
    static void vEncodeIvl(RowNub* pRowNub,
                           i8& i_Tail, i8& i_Offset,
                           Column* pColumn, Datum* pDatum, i8& iHash);
    static void vEncodeTme(RowNub* pRowNub,
                           i8& i_Tail, i8& i_Offset,
                           Column* pColumn, Datum* pDatum, i8& iHash);
    static void vEncodeTsp(RowNub* pRowNub,
                           i8& i_Tail, i8& i_Offset,
                           Column* pColumn, Datum* pDatum, i8& iHash);
    static void vEncodeVch(RowNub* pRowNub,
                           i8& i_Tail, i8& i_Offset,
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
