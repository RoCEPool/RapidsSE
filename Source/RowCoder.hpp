/*============================================================================*/
/* RowCoder.hpp -- Row Coder                                                  */
/*============================================================================*/
#pragma once

namespace Data
{  
  class RowCoder
  {
  public:
    static void vCopy(RowNub* pIntoRowNub, RowNub* pFromRowNub);
  
    static void vDecode(Datum* pDatum, RowNub* pRowNub, i8 i_Row, Column* pCol);
  
    //! make this private
    static i8   nLines(RowNub* pRowNub, i8 i_Row);
  
    static void vEncode(RowNub* pRowNub, Column* pColumn, Datum* pDatum);
  
    static void vPrint(RowNub* pRowNub, i8 i_Row, Column* pCol);
  
    static void vRecord(Record* pRec, RowNub* pRowNub, i8 i_Row);
  
  private:
    static void vAppend(Record* pRec, RowNub* pRowNub, i8 i_Row, Column* pCol);
  
    static i8 nDataBytes(eMN eEncoding, eDM eDatum);
  
    static void vDecodeBcd(Datum* pDatum, RowNub* pRowNub, i8 i_Row, Column* pCol);
    static void vDecodeBol(Datum* pDatum, RowNub* pRowNub, i8 i_Row, Column* pCol);
    static void vDecodeDte(Datum* pDatum, RowNub* pRowNub, i8 i_Row, Column* pCol);
    static void vDecodeFlt(Datum* pDatum, RowNub* pRowNub, i8 i_Row, Column* pCol);
    static void vDecodeInt(Datum* pDatum, RowNub* pRowNub, i8 i_Row, Column* pCol);
    static void vDecodeIvl(Datum* pDatum, RowNub* pRowNub, i8 i_Row, Column* pCol);
    static void vDecodeTme(Datum* pDatum, RowNub* pRowNub, i8 i_Row, Column* pCol);
    static void vDecodeTsp(Datum* pDatum, RowNub* pRowNub, i8 i_Row, Column* pCol);
    static void vDecodeVch(Datum* pDatum, RowNub* pRowNub, i8 i_Row, Column* pCol);
  
    static void vEncodeBcd(RowNub* pRowNub, Column* pColumn, Datum* pDatum);
    static void vEncodeBol(RowNub* pRowNub, Column* pColumn, Datum* pDatum);
    static void vEncodeDte(RowNub* pRowNub, Column* pColumn, Datum* pDatum);
    static void vEncodeFlt(RowNub* pRowNub, Column* pColumn, Datum* pDatum);
    static void vEncodeInt(RowNub* pRowNub, Column* pColumn, Datum* pDatum);
    static void vEncodeIvl(RowNub* pRowNub, Column* pColumn, Datum* pDatum);
    static void vEncodeTme(RowNub* pRowNub, Column* pColumn, Datum* pDatum);
    static void vEncodeTsp(RowNub* pRowNub, Column* pColumn, Datum* pDatum);
    static void vEncodeVch(RowNub* pRowNub, Column* pColumn, Datum* pDatum);
  
    static eMN eEncodingBcd(Datum* pDatum, i8& nBytesEncoded);
    static eMN eEncodingBol(Datum* pDatum, i8& nBytesEncoded);
    static eMN eEncodingDte(Datum* pDatum, i8& nBytesEncoded);
    static eMN eEncodingFlt(Datum* pDatum, i8& nBytesEncoded);
    static eMN eEncodingInt(Datum* pDatum, i8& nBytesEncoded);
    static eMN eEncodingIvl(Datum* pDatum, i8& nBytesEncoded);
    static eMN eEncodingTme(Datum* pDatum, i8& nBytesEncoded);
    static eMN eEncodingTsp(Datum* pDatum, i8& nBytesEncoded);
    static eMN eEncodingVch(Datum* pDatum, i8& nBytesEncoded);
  
    static eMN eGetEncoding(CacheLine* aCacheLine, i8 i_Column, i8& i_Offset);
  
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
