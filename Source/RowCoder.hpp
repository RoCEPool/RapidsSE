/*============================================================================*/
/* RowCoder.hpp -- Row Coder                                                  */
/*============================================================================*/
#pragma once

namespace Data
{
  static const i8 C_TAIL_UNITS_OFFSET = 0;
  static const i8 C_TAIL_UNITS_BYTES  = 1;
  
  static const i8 C_SHOCO_COMPRESSED_OFFSET = C_TAIL_UNITS_OFFSET
                                              + C_TAIL_UNITS_BYTES;
  static const i8 C_SHOCO_COMPRESSED_BYTES  = 1;

  class RowCoder
  {
  public:
    static void vAnalyse(RowNub* pNub);
    static bool bCopy(RowNub* pIntoNub, RowNub* pFromNub);
    static void vDecode(Datum* pDatum, RowNub* pNub, i8 i_Row, Column* pCol);
    static void vDecode(Datum* pDatum, HeadLine* aHeadLine,
                        ColumnVec* pColVec, Column* pCol);
    static void vEncode(RowNub* pNub, Column* pCol, Datum* pDatum);
    static void vPrint(RowNub* pNub, i8 i_Row, Column* pCol);
    static void vRecord(Record* pRec, RowNub* pNub, i8 i_Row);
    static void vRecord(Record* pRec, HeadLine* aHeadLine, ColumnVec* pColVec);

  private:
    static void vAppend(Record* pRec, HeadLine* aHeadLine, ColumnVec* pColVec, Column* pCol);
  
    static i8 nDataBytes(HeadLine* aHeadLine, Column* pCol, eMN eEncoding, eDM eDatum);
  
    static void vDecodeBcd(Datum* pDatum, HeadLine* aHeadLine, ColumnVec* pColVec, Column* pCol);
    static void vDecodeBol(Datum* pDatum, HeadLine* aHeadLine, ColumnVec* pColVec, Column* pCol);
    static void vDecodeDte(Datum* pDatum, HeadLine* aHeadLine, ColumnVec* pColVec, Column* pCol);
    static void vDecodeFlt(Datum* pDatum, HeadLine* aHeadLine, ColumnVec* pColVec, Column* pCol);
    static void vDecodeInt(Datum* pDatum, HeadLine* aHeadLine, ColumnVec* pColVec, Column* pCol);
    static void vDecodeIvl(Datum* pDatum, HeadLine* aHeadLine, ColumnVec* pColVec, Column* pCol);
    static void vDecodeTme(Datum* pDatum, HeadLine* aHeadLine, ColumnVec* pColVec, Column* pCol);
    static void vDecodeTsp(Datum* pDatum, HeadLine* aHeadLine, ColumnVec* pColVec, Column* pCol);
    static void vDecodeVch(Datum* pDatum, HeadLine* aHeadLine, ColumnVec* pColVec, Column* pCol);

    static void vDecodeVchShoco(Datum* pDatum, HeadLine* aHeadLine,
                                Column* pCol, eMN eEncoding, i8 i_Offset);
    static void vDecodeVchShocoFit(Datum* pDatum, HeadLine* aHeadLine,
                                   Column* pCol, eMN eEncoding, i8 i_Offset);
    static void vDecodeVchSmall(Datum* pDatum, HeadLine* aHeadLine,
                                Column* pCol, eMN eEncoding, i8 i_Offset);

    static void vEncodeBcd(RowNub* pNub, Column* pCol, Datum* pDatum);
    static void vEncodeBol(RowNub* pNub, Column* pCol, Datum* pDatum);
    static void vEncodeDte(RowNub* pNub, Column* pCol, Datum* pDatum);
    static void vEncodeFlt(RowNub* pNub, Column* pCol, Datum* pDatum);
    static void vEncodeInt(RowNub* pNub, Column* pCol, Datum* pDatum);
    static void vEncodeIvl(RowNub* pNub, Column* pCol, Datum* pDatum);
    static void vEncodeTme(RowNub* pNub, Column* pCol, Datum* pDatum);
    static void vEncodeTsp(RowNub* pNub, Column* pCol, Datum* pDatum);
    static void vEncodeVch(RowNub* pNub, Column* pCol, Datum* pDatum);
  
    static void vEncodeVchShoco(RowNub* pNub, Column* pCol, Datum* pDatum);
    static void vEncodeVchSmall(RowNub* pNub, Column* pCol, Datum* pDatum);

    static eMN eEncodingBcd(Datum* pDatum, i8& nBytesEncoded);
    static eMN eEncodingBol(Datum* pDatum, i8& nBytesEncoded);
    static eMN eEncodingDte(Datum* pDatum, i8& nBytesEncoded);
    static eMN eEncodingFlt(Datum* pDatum, i8& nBytesEncoded);
    static eMN eEncodingInt(Datum* pDatum, i8& nBytesEncoded);
    static eMN eEncodingIvl(Datum* pDatum, i8& nBytesEncoded);
    static eMN eEncodingTme(Datum* pDatum, i8& nBytesEncoded);
    static eMN eEncodingTsp(Datum* pDatum, i8& nBytesEncoded);
    static eMN eEncodingVch(Datum* pDatum, i8& nBytesEncoded);
  
    //! move into Nub
    static i8 iFindTail(RowNub* pNub, TailLine* aFindLine);

    static i8 nFreeSpaceHead(RowNub* pNub, HeadLine* aHeadLine);
  
    static eMN eGetEncoding(HeadLine* aHeadLine, ColumnVec* pColVec,
                            i8 i_Col, i8& i_Offset);
  
    static void vInitHeadLine(HeadLine* aHeadLine);
  
    static i8 nLines(RowNub* pNub, i8 i_Row);
  
    static void vRestoreDte(Datum* pDatum, eMN eEncoding);
    static void vRestoreTme(Datum* pDatum, eMN eEncoding);
    static void vRestoreTsp(Datum* pDatum, eMN eEncoding);
  
    static void vFromBaseDays(Datum* pDatum, i8& iEra);
    static void vIntoBaseDays(Datum* pDatum, i8& iEra);
  
    static i8 iStripDte(WordView& wTspView, eMN eEncoding);
    static i8 iStripTme(WordView& wTspView, eMN eEncoding);
    static i8 iStripTsp(WordView& wTspView, eMN eEncoding);
 
    static i8 nShocoTailSize(RowNub* pNub, HeadLine* aHeadLine);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
