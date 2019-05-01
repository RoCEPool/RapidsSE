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

  class RowCoder : public Object
  {
  public:
  
    RowNub*    m_pEncodeNub;
  
    ColumnVec* m_pColVec;
    Coda*      m_pCoda;

    HeadLine*  m_aHeadLine;

    i8         m_i_IntoOffset; // encoding offset
    i8         m_i_FromOffset; // encoding offset


    //  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
  
    struct {
      i8   m_nEncoded;       // no of times the column has originally encoded
      i8   m_nEncodedBytes;  // bytes originally encoded
      i8   m_nRecoded;       // no of times able to succesfully recode
      i8   m_nRecodedBytes;  // bytes succesfully recoded
      i8   m_nOops;          // no of times unable to succesfully recode
      i8   m_nOopsBytes;     // oops bytes
      i8   m_nOopsMiss;      // oops miss

    } m_xaUsage[C_MAX_COLUMNS];
  
    i8 m_nBuffers;
    i8 m_nRows;
    i8 m_nHeadFree;

    i8 m_nTails;
    i8 m_nTailUnits;
  
    i8 m_nFreeHead_0_7;
    i8 m_nFreeHead_8_15;
    i8 m_nFreeHead_16_23;
    i8 m_nFreeHead_24_31;
    i8 m_nFreeHead_32_39;
    i8 m_nFreeHead_40_47;
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
  void extracted();
  
    RowCoder(void);
    ~RowCoder(void) noexcept(false);

    void vCopyBegin(RowNub* pIntoNub);
    void vCopyEnd(void);
    bool bCopyRow(RowNub* pFromNub);

    void vEncodeBegin(RowNub* pEncodeNub);
    void vEncodeDatum(Datum* pDatum, Column* pCol);
    void vEncodeEnd(void);

    void vReport(RowCoder* pRowCoder_Encode);
  
  private:
    void vEncodeBcd(Datum* pDatum, Column* pCol);
    void vEncodeBol(Datum* pDatum, Column* pCol);
    void vEncodeDte(Datum* pDatum, Column* pCol);
    void vEncodeFlt(Datum* pDatum, Column* pCol);
    void vEncodeInt(Datum* pDatum, Column* pCol);
    void vEncodeIvl(Datum* pDatum, Column* pCol);
    void vEncodeTme(Datum* pDatum, Column* pCol);
    void vEncodeTsp(Datum* pDatum, Column* pCol);
    void vEncodeVch(Datum* pDatum, Column* pCol);
    void vEncodeVchShoco(Datum* pDatum, Column* pCol);
    void vEncodeVchSmall(Datum* pDatum, Column* pCol);
    
    void vRecode(i8 i_IntoHead, RowNub* pFromNub, HeadLine* aFromHead);
    void vRecodeBcd(Column* pCol, i8& i_IntoOffset, i8& i_FromOffset);
    void vRecodeDte(Column* pCol, i8& i_IntoOffset, i8& i_FromOffset);
    void vRecodeInt(Column* pCol, i8& i_IntoOffset, i8& i_FromOffset);
    bool bRecodeIntDelta(Column* pCol, i8& i_IntoOffset, i8& i_FromOffset);
    bool bRecodeIntDigit(Column* pCol, i8& i_IntoOffset, i8& i_FromOffset);
    void vRecodeVch(i8 i_IntoHead,
                    RowNub* pFromNub, Column* pCol, eMN eEncoding,
                    i8& i_ToOffset, i8& i_FromOffset);
    void vRecodeVchAlpha(Column* pCol, i8& i_IntoOffset, i8& i_FromOffset);
    void vRecodeVchShoco(i8 i_IntoHead,
                         RowNub* pFromNub, Column* pCol,
                         i8 i_ToOffset, i8 i_FromOffset);



  public:
    static void vAnalyse(RowNub* pNub);
    static void vDecode(Datum* pDatum, RowNub* pNub, i8 i_Row, Column* pCol,
                        i8& i_LastColumn, i8& i_LastOffset);
//    static void vDecodeFast(Datum* pDatum, HeadLine* aHeadLine, ColumnVec* pColVec,
//                            Column* pCol, Coda* pCoda,
//                            WordView& xWord,i8& i_LastColumn, i8& i_LastOffset);
    static void vDecode(Datum* pDatum, HeadLine* aHeadLine,
                        ColumnVec* pColVec, Column* pCol, Coda* pCoda,
                        i8& i_LastColumn, i8& i_LastOffset);
    static void vHeadDump(HeadLine* aHeadLine, ColumnVec* pColVec);
    static void vHeadDumpColumn(HeadLine* aHeadLine, ColumnVec* pColVec, i8 iCol, Stz* aRecodeFunction);
    static void vHeadDumpNat(HeadLine* aHeadLine, ColumnVec* pColVec);
    static void vHeadDumpNatColumn(HeadLine* aHeadLine, ColumnVec* pColVec, i8 iCol);
    static void vPrint(RowNub* pNub, i8 i_Row, Column* pCol, i8& i_LastColumn, i8& i_LastOffset);
    static void vRecord(Record* pRec, RowNub* pNub, i8 i_Row);
    static void vRecord(Record* pRec, HeadLine* aHeadLine, ColumnVec* pColVec);
    static void vRetrieve(Datum& oDatum, RowBuffer* pRowBuffer, HeadLine* aHeadLine, Column* pCol);
  
  private:
    static void vAppend(Record* pRec, HeadLine* aHeadLine,
                        ColumnVec* pColVec, Column* pCol, Coda* pCoda,
                        i8& i_LastColumn, i8& i_LastOffset);
  
    static i8 iBaseOffset(ColumnVec* pColVec);
  
    static i8 nDataBytes(eDM eDatum, eMN eEncoding);
  
    static void vDecodeBcd(Datum* pDatum, HeadLine* aHeadLine,
                           ColumnVec* pColVec, Column* pCol, Coda* pCoda,
                           i8& i_LastColumn, i8& i_LastOffset);
    static void vDecodeBol(Datum* pDatum, HeadLine* aHeadLine,
                           ColumnVec* pColVec, Column* pCol, Coda* pCoda,
                           i8& i_LastColumn, i8& i_LastOffset);
    //static void vDecodeBolFast(Datum* pDatum, HeadLine* aHeadLine,
    //                       Column* pCol,WordView& xWord);
    static void vDecodeDte(Datum* pDatum, HeadLine* aHeadLine,
                           ColumnVec* pColVec, Column* pCol, Coda* pCoda,
                           i8& i_LastColumn, i8& i_LastOffset);
    static void vDecodeFlt(Datum* pDatum, HeadLine* aHeadLine,
                           ColumnVec* pColVec, Column* pCol, Coda* pCoda,
                           i8& i_LastColumn, i8& i_LastOffset);
    static void vDecodeInt(Datum* pDatum, HeadLine* aHeadLine,
                           ColumnVec* pColVec, Column* pCol, Coda* pCoda,
                           i8& i_LastColumn, i8& i_LastOffset);
    static void vDecodeIvl(Datum* pDatum, HeadLine* aHeadLine,
                           ColumnVec* pColVec, Column* pCol, Coda* pCoda,
                           i8& i_LastColumn, i8& i_LastOffset);
    static void vDecodeTme(Datum* pDatum, HeadLine* aHeadLine,
                           ColumnVec* pColVec, Column* pCol, Coda* pCoda,
                           i8& i_LastColumn, i8& i_LastOffset);
    static void vDecodeTsp(Datum* pDatum, HeadLine* aHeadLine,
                           ColumnVec* pColVec, Column* pCol, Coda* pCoda,
                           i8& i_LastColumn, i8& i_LastOffset);
    static void vDecodeVch(Datum* pDatum, HeadLine* aHeadLine,
                           ColumnVec* pColVec, Column* pCol, Coda* pCoda,
                           i8& i_LastColumn, i8& i_LastOffset);
  
    // extended datums
    static void vDecodeAlpha(Datum* pDatum, HeadLine* aHeadLine,
                             ColumnVec* pColVec, Column* pCol, Coda* pCoda,
                             i8& i_LastColumn, i8& i_LastOffset);
    static void vDecodeDays(Datum* pDatum, HeadLine* aHeadLine,
                            ColumnVec* pColVec, Column* pCol, Coda* pCoda,
                            i8& i_LastColumn, i8& i_LastOffset);
    static void vDecodeDeltaPlus(Datum* pDatum, HeadLine* aHeadLine,
                                 ColumnVec* pColVec, Column* pCol, Coda* pCoda,
                                 i8& i_LastColumn, i8& i_LastOffset);
    static void vDecodeDigit(Datum* pDatum, HeadLine* aHeadLine,
                             ColumnVec* pColVec, Column* pCol, Coda* pCoda,
                              i8& i_LastColumn, i8& i_LastOffset);
  
    static void vDecodeTenth(Datum* pDatum, HeadLine* aHeadLine,
                              ColumnVec* pColVec, Column* pCol, Coda* pCoda,
                              i8& i_LastColumn, i8& i_LastOffset);

    // shocos
    static void vDecodeVchShoco(Datum* pDatum, HeadLine* aHeadLine,
                                Column* pCol, eMN eEncoding, i8 i_Offset);
    static void vDecodeVchShocoFit(Datum* pDatum, HeadLine* aHeadLine,
                                   Column* pCol, eMN eEncoding, i8 i_Offset);
    static void vDecodeVchShocoSplit(Datum* pDatum, HeadLine* aHeadLine,
                                   Column* pCol, eMN eEncoding, i8 i_Offset);
    static void vDecodeVchSmall(Datum* pDatum, HeadLine* aHeadLine,
                                Column* pCol, eMN eEncoding, i8 i_Offset);

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
    static i8 nFreeSpaceHead(HeadLine* aHeadLine, ColumnVec* pColVec, Coda* pCoda);

    static eMN eGetEncodingExt(i8 i_Col, HeadLine* aHeadLine,
                               ColumnVec* pColVec, Coda* pCoda, i8& i_PriorColumn_,
                               i8& i_Offset_);
  
    static eMN eGetEncodingNat(i8 i_ForCol, HeadLine* aHeadLine,
                               ColumnVec* pColVec, i8& i_PriorColumn_,
                               i8& i_Offset_);
    static Stz* sEncoding(eMN eEncoding, eDM eDatumNat, eDM eDatumExt);
    static void vInitHeadLine(HeadLine* aHeadLine);
  
    static i8 nLines(RowNub* pNub, i8 i_Row);
  
    static void vRestoreDte(Datum* pDatum, eMN eEncoding);
    static void vRestoreTme(Datum* pDatum, eMN eEncoding);
    static void vRestoreTsp(Datum* pDatum, eMN eEncoding);
  
    static void vFromBaseDays(Datum* pDatum, i8& iEra);
    static i8   iIntoBaseDays(i8 nDaysWithinEra, i8 iEra);
    static void vIntoBaseDays(Datum* pDatum, i8& iEra);

    static i8 iStripDte(WordView& wTspView, eMN eEncoding);
    static i8 iStripTme(WordView& wTspView, eMN eEncoding);
    static i8 iStripTsp(WordView& wTspView, eMN eEncoding);
 
    static i8 nShocoTailSize(RowNub* pNub, HeadLine* aHeadLine);
  
    static void vSetAuxInt(Coda* pIntoCoda, HeadLine* aIntoHead, Column* pCol,
                           eMN eEncoding, i8 i_IntoOffset);
  
    void vUsageIncEncoded(i8 i_Col, i8 nBytes);
    void vUsageIncRecoded(i8 i_Col, i8 nBytes);
    void vUsageIncOops(i8 i_Col, i8 nBytes, i8 nMiss);
    void vUsageReset(void);

  private:
    Footer* aFooter(void);
    
    /*----------------------------------------------------------------------------*/
    /* decode boolean from given cache line                                       */
    /*----------------------------------------------------------------------------*/
    static inline void vDecodeBolFast(Datum* pDatum, HeadLine* aHeadLine,
                                  Column* pCol)
    {
      i8 i_Column  = pCol->iColumnNo();
      
      pDatum->m_eDatum = DM_BOL;
      
      //eMN eEncoding = eGetEncodingExt(i_Column,aHeadLine,pColVec,pCoda,i_LastColumn,i_Offset);
      //eMN eEncoding = (eMN)aHeadLine->w8_1NFast(xWord, i_Column);
      eMN eEncoding = (eMN)aHeadLine->w8_1NFast2(i_Column);

//      switch (eEncoding) {
//
//        case MN_NULL:  pDatum->m_iInt = C_DATUM_NULL; break;
//        case MN_FALSE: pDatum->m_iInt = (i8)0;        break;
//        case MN_TRUE:  pDatum->m_iInt = (i8)1;        break;
//
//        default:
//          NEVER_REACH;
//      }
//
      
      i8 x = 1;
      switch (x) {
          
        case 0:  pDatum->m_iInt = C_DATUM_NULL; break;
        case 1:  pDatum->m_iInt = (i8)0;        break;
        case 2:  pDatum->m_iInt = (i8)1;        break;
          
        default:
          NEVER_REACH;
      }
     
    }
    
    /*----------------------------------------------------------------------------*/
    /* get encoded datum from cache line                                          */
    /*----------------------------------------------------------------------------*/
    static inline void vDecodeFast(Datum* pDatum,
                                   RowBuffer* pRowBuffer, HeadLine* aHeadLine,
                                   Column* pCol)
    {
      
      if (pCol->eDatumType() == DM_BOL) {
        vDecodeBolFast(pDatum,aHeadLine,pCol);
        return;
      }
    
      Coda* pCoda = pRowBuffer->pRowTray()->pCoda();
      ColumnVec* pColVec = pRowBuffer->pRowTray()->pDesc()->pColumnVec();
      i8 i_LastColumn = 0;
      i8 i_LastOffset = iBaseOffset(pColVec);
    
      vDecode(pDatum,aHeadLine,pColVec,pCol,pCoda,i_LastColumn,i_LastOffset);
    }
  };
  
  RowCoder* _pRowCoder(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
