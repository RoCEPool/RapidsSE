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
  static i8 m_iDatumEncoding[DM_TENTH][MN_OVERFLOW];
  
  /*----------------------------------------------------------------------------*/
  const static i8 nMonthDaySum[13] =
  //    J  F  M   A   M   J   J   A   S   O   N   D
  {0,31,60,91,121,152,182,213,244,274,305,335,366};
  
  class RowCoder : public Object
  {
  public:
  
    RowNub*    m_pEncodeNub;
  
    RowParts*  m_pWorkingParts;
    RowNub*    m_pWorkingNub;

    ColumnVec* m_pColVec;
    Coda*      m_pCoda;

    i8         m_i_IntoHead;
    i8         m_i_IntoOffset; // encoding offset
    HeadLine*  m_aHeadLine;
  
    i8         m_i_FromHead;
    i8         m_i_FromOffset; // encoding offset
    HeadLine*  m_aFromLine;

  
    Datum      m_oEncodedDatum; // the encoded datum

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
    i8 m_nFreeHead_48_55;
    i8 m_nFreeHead_56_63;
    i8 m_nFreeHead_64_;

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
    void vAnalyseFree(void);
    void vCopy_Large(RowNub* pFromNub);
    void vCopy_Short(RowNub* pFromNub);
    void vDataPut(Datum* pDatum);
    void vDataPut(w8 wDatum, i8 nBytes);
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
  
    void vHashColumn(Datum* pDatum, Column* pCol);
  
    void vMetaNibble(Column* pCol, eMN eMetaNibble);
    void vMetaPut(Datum* pDatum, Column* pCol);

    void vRecode_Large(RowNub* pFromNub);
    void vRecode_Short(RowNub* pFromNub);
    void vRecodeBcd(Column* pCol);
    void vRecodeDte(Column* pCol);
    void vRecodeInt(Column* pCol);
    bool bRecodeIntDelta(Column* pCol);
    bool bRecodeIntDigit(Column* pCol);
    void vRecodeVch(i8 i_IntoHead, RowNub* pFromNub, Column* pCol, eMN eEncoding);
    void vRecodeVchAlpha(Column* pCol);
    void vRecodeVchShoco(i8 i_IntoHead, RowNub* pFromNub, Column* pCol);



  public:
    static void vAnalyse(RowNub* pNub);
    static void vAppend(Datum& oDatum, Column* pCol, Record* pRec);
    static void vDecode(Datum* pDatum, RowNub* pNub, i8 i_Row, Column* pCol,
                        i8& i_LastColumn, i8& i_LastOffset);
//    static void vDecodeFast(Datum* pDatum, HeadLine* aHeadLine, ColumnVec* pColVec,
//                            Column* pCol, Coda* pCoda,
//                            Word& xWord,i8& i_LastColumn, i8& i_LastOffset);
    static void vDecode(Datum* pDatum, HeadLine* aHeadLine,
                        ColumnVec* pColVec, Column* pCol, Coda* pCoda,
                        i8& i_LastColumn, i8& i_LastOffset);
    static void vDecodeFast_Fallback(Datum* pDatum, HeadLine* aHeadLine, Column* pCol,
                                     Coda* pCoda);
    static void vHeadDump(HeadLine* aHeadLine, ColumnVec* pColVec);
    static void vHeadDump(HeadLine* aHeadLine, ColumnVec* pColVec, Coda* pCoda);
    static void vHeadDumpColumn(HeadLine* aHeadLine, ColumnVec* pColVec, i8 iCol, Stz* aRecodeFunction);
    static void vHeadDumpNat(HeadLine* aHeadLine, ColumnVec* pColVec);
    static void vHeadDumpNatColumn(HeadLine* aHeadLine, ColumnVec* pColVec, i8 iCol);
    static void vInitEncoding(void);
    static void vPrint(RowNub* pNub, i8 i_Row, Column* pCol, i8& i_LastColumn, i8& i_LastOffset);
    static void vRecord(Record* pRec, RowNub* pNub, i8 i_Row);
    static void vRecord(Record* pRec, HeadLine* aHeadLine, ColumnVec* pColVec);
    static void vRecord(Record* pRec, HeadLine* aHeadLine,
                        ColumnVec* pColVec, Coda* pCoda);

  private:
    static void vAppend(Record* pRec, HeadLine* aHeadLine,
                        ColumnVec* pColVec, Column* pCol, Coda* pCoda,
                        i8& i_LastColumn, i8& i_LastOffset);
  
    //static i8 iBaseOffset(ColumnVec* pColVec);
  
    static i8 nDataBytes(eDM eDatum, eMN eEncoding);
    //static i8 nDataBytes2(eDM eDatum, eMN eEncoding);
    
    static void vDecodeBcd(Datum* pDatum, HeadLine* aHeadLine,
                           ColumnVec* pColVec, Column* pCol, Coda* pCoda,
                           i8& i_LastColumn, i8& i_LastOffset);
    static void vDecodeBol(Datum* pDatum, HeadLine* aHeadLine,
                           ColumnVec* pColVec, Column* pCol, Coda* pCoda,
                           i8& i_LastColumn, i8& i_LastOffset);
    //static void vDecodeBolFast(Datum* pDatum, HeadLine* aHeadLine,
    //                       Column* pCol,Word& xWord);
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

    void vCodeBcd(Datum* pTgtDatum, Datum* pSrcDatum);
    void vCodeBol(Datum* pTgtDatum, Datum* pSrcDatum);
    void vCodeDte(Datum* pTgtDatum, Datum* pSrcDatum);
    void vCodeFlt(Datum* pTgtDatum, Datum* pSrcDatum);
    void vCodeInt(Datum* pTgtDatum, Datum* pSrcDatum);
    void vCodeIvl(Datum* pTgtDatum, Datum* pSrcDatum);
    void vCodeTme(Datum* pTgtDatum, Datum* pSrcDatum);
    void vCodeTsp(Datum* pTgtDatum, Datum* pSrcDatum);
    void vCodeVch(Datum* pTgtDatum, Datum* pSrcDatum);
  
    void vXchkBcd(Datum* pDatum, Column* pCol);
    void vXchkBol(Datum* pDatum, Column* pCol);
    void vXchkDte(Datum* pDatum, Column* pCol);
    void vXchkFlt(Datum* pDatum, Column* pCol);
    void vXchkInt(Datum* pDatum, Column* pCol);
    void vXchkIvl(Datum* pDatum, Column* pCol);
    void vXchkTme(Datum* pDatum, Column* pCol);
    void vXchkTsp(Datum* pDatum, Column* pCol);
    void vXchkVch(Datum* pDatum, Column* pCol);

  
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

    static i8 iStripDte(Word& wTspView, eMN eEncoding);
    static i8 iStripTme(Word& wTspView, eMN eEncoding);
    static i8 iStripTsp(Word& wTspView, eMN eEncoding);
 
    static i8 nShocoTailSize(RowNub* pNub, HeadLine* aHeadLine);
  
    static void vSetAuxInt(Coda* pIntoCoda, HeadLine* aIntoHead, Column* pCol,
                           eMN eEncoding, i8 i_IntoOffset);
  
    void vUsageIncEncoded(Column* pCol, i8 nBytes);
    void vUsageIncEncoded(Column* pCol, Datum* pDatum);
    void vUsageIncRecoded(Column* pCol, i8 nBytes);
    void vUsageIncOops(Column* pCol, i8 nBytes, i8 nMiss);
    void vUsageReset(void);

  private:
    Footer* aFooter(void);

    /*----------------------------------------------------------------------------*/
    /* decode decimal from given cache line                                       */
    /*----------------------------------------------------------------------------*/
    static inline void vDecodeBcdFast(Datum* pDatum, HeadLine* aHeadLine,
                                      eMN eEncoding, i8 i_Offset)
    {
      
      pDatum->m_eDatum = DM_BCD;
      
      switch (eEncoding) {
          
        case MN_NULL: pDatum->m_iInt = C_DATUM_NULL;               break;
        case MN_1B:   aHeadLine->w8_1BFast(pDatum, i_Offset); break;
        case MN_2B:   aHeadLine->w8_2BFast(pDatum, i_Offset); break;
        case MN_3B:   aHeadLine->w8_3BFast(pDatum, i_Offset); break;
        case MN_4B:   aHeadLine->w8_4BFast(pDatum, i_Offset); break;
        case MN_5B:   aHeadLine->w8_5BFast(pDatum, i_Offset); break;
        case MN_6B:   aHeadLine->w8_6BFast(pDatum, i_Offset); break;
        case MN_7B:   aHeadLine->w8_7BFast(pDatum, i_Offset); break;
        case MN_8B:   aHeadLine->w8_8BFast(pDatum, i_Offset); break;
          
        case MN_MINUS_ONE: pDatum->m_iInt = (i8)-1; break;
        case MN_ZERO:      pDatum->m_iInt = (i8)0;  break;
        case MN_PLUS_ONE:  pDatum->m_iInt = (i8)1;  break;
          
        default:
          NEVER_REACH;
      }
    }


    /*----------------------------------------------------------------------------*/
    /* decode boolean from given cache line                                       */
    /*----------------------------------------------------------------------------*/
    static inline void vDecodeBolFast(Datum* pDatum, HeadLine* aHeadLine, Column* pCol)
    {
      i8 i_Column = pCol->iColumnNo();
      
      pDatum->m_eDatum = DM_BOL;
      
      eMN eEncoding = (eMN)aHeadLine->w8_1NFast((byte)i_Column);

      switch (eEncoding) {

        case MN_NULL:  pDatum->m_iInt = C_DATUM_NULL; return;
        case MN_FALSE: pDatum->m_iInt = (i8)0;        return;
        case MN_TRUE:  pDatum->m_iInt = (i8)1;        return;

        default:
          NEVER_REACH;
      }
    }
    
    /*----------------------------------------------------------------------------*/
    /* decode date from given cache line                                          */
    /*----------------------------------------------------------------------------*/
    static inline void vDecodeDteFast(Datum* pDatum, HeadLine* aHeadLine,
                                  eMN eEncoding, i8& i_Offset)
    {
      pDatum->m_eDatum = DM_DTE;
      
      switch (eEncoding) {
          
        case MN_NULL: pDatum->m_iInt = C_DATUM_NULL; break;
          
        case MN_1B:
        case MN_2B:
        case MN_3B:
        case MN_4B:
        case MN_5B:
        case MN_6B:
        case MN_7B:
          NEVER_REACH;
          
        case MN_8B: aHeadLine->w8_8BFast(pDatum, i_Offset); break;
          
        case MN_CLASSIC_1B: aHeadLine->w8_1BFast(pDatum,i_Offset); break;
        case MN_CLASSIC_2B: aHeadLine->w8_2BFast(pDatum,i_Offset); break;
        case MN_SILVER_1B:  aHeadLine->w8_1BFast(pDatum,i_Offset); break;
        case MN_SILVER_2B:  aHeadLine->w8_2BFast(pDatum,i_Offset); break;
        case MN_MODERN_1B:  aHeadLine->w8_1BFast(pDatum,i_Offset); break;
        case MN_MODERN_2B:  aHeadLine->w8_2BFast(pDatum,i_Offset); break;
          
        default:
          NEVER_REACH;
      }
      
      vRestoreDteFast(pDatum,eEncoding);
    }

    /*----------------------------------------------------------------------------*/
    /* decode integer from given cache line                                       */
    /*----------------------------------------------------------------------------*/
    static inline void vDecodeIntFast(Datum* pDatum, HeadLine* aHeadLine,
                                      eMN eEncoding, i8 i_Offset)
    {
      
      pDatum->m_eDatum = DM_INT;
      
      //vTrace("Decoding %lld\n"_Stz, eEncoding);
      
      switch (eEncoding) {
          
        case MN_NULL: pDatum->m_iInt = C_DATUM_NULL;          break;
        case MN_1B:   aHeadLine->w8_1BFast(pDatum, i_Offset); break;
        case MN_2B:   aHeadLine->w8_2BFast(pDatum, i_Offset); break;
        case MN_3B:   aHeadLine->w8_3BFast(pDatum, i_Offset); break;
        case MN_4B:   aHeadLine->w8_4BFast(pDatum, i_Offset); break;
        case MN_5B:   aHeadLine->w8_5BFast(pDatum, i_Offset); break;
        case MN_6B:   aHeadLine->w8_6BFast(pDatum, i_Offset); break;
        case MN_7B:   aHeadLine->w8_7BFast(pDatum, i_Offset); break;
        case MN_8B:   aHeadLine->w8_8BFast(pDatum, i_Offset); break;
          
        case MN_MINUS_ONE: pDatum->m_iInt = (i8)-1; break;
        case MN_ZERO:      pDatum->m_iInt = (i8)0;  break;
        case MN_PLUS_ONE:  pDatum->m_iInt = (i8)1;  break;
          
//        case MN_SAME:
//          pDatum->m_iInt = pCoda->m_aiAux[i_Column].m_iInt;
//          break;
          
          //    case MN_PLUS_1B:
          //      pDatum->m_iInt = pCoda->m_aiAux[i_Column].m_iInt
          //                     + aHeadLine->w8_1B(i_Offset);
          //      break;
          //
          //    case MN_MINUS_1B:
          //      pDatum->m_iInt = pCoda->m_aiAux[i_Column].m_iInt
          //                     - aHeadLine->w8_1B(i_Offset);
          break;
          
        default:
          NEVER_REACH;
      }
    }
    
    /*----------------------------------------------------------------------------*/
    /* decode varchar from given cache line                                       */
    /*----------------------------------------------------------------------------*/
    static inline void vDecodeVchFast(Datum* pDatum, HeadLine* aHeadLine,
                                      eMN eEncoding, i8 i_Offset)
    {
      
      pDatum->m_eDatum = DM_VCH;
      
      switch (eEncoding) {
        case MN_NULL: pDatum->m_iInt = C_DATUM_NULL; break;
        case MN_1B:
        case MN_2B:
        case MN_3B:
        case MN_4B:
        case MN_5B:
        case MN_6B:
        case MN_7B:
        case MN_8B:
          vDecodeVchSmallFast(pDatum,aHeadLine,eEncoding,i_Offset);
          break;
          
        case MN_EMPTY:
          pDatum->m_nChars = 0;
          *pDatum->m_sVch.aChar() = '\0';
          pDatum->m_iInt = 0;
          break;
          
        case MN_SHOCO_FIT:
          vDecodeVchShocoFitFast(pDatum,aHeadLine,i_Offset);
          break;
          
        case MN_SHOCO_SPLIT:
          vDecodeVchShocoSplitFast(pDatum,aHeadLine,i_Offset);
          break;
          
        case MN_SHOCO_2B:
          vDecodeVchShocoFast(pDatum,aHeadLine,i_Offset);
          break;
          
        default:
          NEVER_REACH;
      }
      
      /*
      if (MN_NULL == eEncoding)
        pDatum->m_iInt = C_DATUM_NULL;
      
      else if (eEncoding <= MN_8B)
        vDecodeVchSmallFast(pDatum,aHeadLine,eEncoding,i_Offset);
      
      else if (MN_EMPTY == eEncoding) {
        pDatum->m_nChars = 0;
        *pDatum->m_xWsp.aArea() = '\0';
        pDatum->m_iInt = 0;
      }
      
      else if (MN_SHOCO_2B == eEncoding)
        vDecodeVchShocoFast(pDatum,aHeadLine,eEncoding,i_Offset);
      
      else if (MN_SHOCO_FIT == eEncoding)
        vDecodeVchShocoFitFast(pDatum,aHeadLine,eEncoding,i_Offset);
      
      else if (MN_SHOCO_SPLIT == eEncoding)
        vDecodeVchShocoSplitFast(pDatum,aHeadLine,eEncoding,i_Offset);
      
      else
        NEVER_REACH;
       */
      
    }
    
    /*----------------------------------------------------------------------------*/
    /* decode varchar from given cache line                                       */
    /*----------------------------------------------------------------------------*/
    static inline void vDecodeVchShocoFast(Datum* pDatum, HeadLine* aHeadLine, i8 i_Offset)
    {
      //i8 i_TailDistance = aHeadLine->w8_2B(i_Offset);
      aHeadLine->w8_2BFast(pDatum, i_Offset);
      i8 i_TailDistance = pDatum->m_wWord;
      
      TailLine* aShocoTail = RowNub::aToTail(aHeadLine,i_TailDistance);
      
      //MemLib::vWordDump(aShocoTail,sizeof(TailLine));
      
      /*
       +=====================================+
       |            Shoco Tail Line          +
       +=====================================+
       | U | C | <compressed bytes> <unused> +
       +-------------------------------------+
       | 0 | 1 | 2                        63 |
       +-------------------------------------+
       
       where: U is one byte being the unit length of the tail line
       C is one byte being the number of compressed bytes
       */
      
      //i8 nCompressed = aShocoTail->w8_1B(C_SHOCO_COMPRESSED_OFFSET);
      aShocoTail->w8_1BFast(pDatum, C_SHOCO_COMPRESSED_OFFSET);
      i8 nCompressed = pDatum->m_wWord;
      //RASSERT(nCompressed <= 4);
      
      i8 nDecompressed =
      Compress::iFromShoco(pDatum->m_sVch.aChar(),   // decompressed destination
                           255,                      // max destination
                           ((byte*)aShocoTail)       // from compressed source
                           + C_SHOCO_COMPRESSED_OFFSET
                           + C_SHOCO_COMPRESSED_BYTES,
                           nCompressed);             // bytes compressed
      ASSERT(nDecompressed);
      pDatum->m_nChars = nDecompressed;
    }
    
    /*----------------------------------------------------------------------------*/
    /* decode varchar from given cache line                                       */
    /*----------------------------------------------------------------------------*/
    static inline void vDecodeVchShocoFitFast(Datum* pDatum, HeadLine* aHeadLine,
                                             i8 i_Offset)
    {
      //i8 nCompressed = aHeadLine->w8_1B(i_Offset);
      aHeadLine->w8_1BFast(pDatum, i_Offset);
      i8 nCompressed = pDatum->m_wWord;
      
      RASSERT(nCompressed <= 56);
      
      i8 nDecompressed =
      Compress::iFromShoco((char*)pDatum->m_sVch.aChar(),
                           255,
                           ((byte*)aHeadLine)+i_Offset+1,
                           nCompressed);
      ASSERT(nDecompressed);
      pDatum->m_nChars = nDecompressed;
    }
    
    /*----------------------------------------------------------------------------*/
    /* decode varchar from given cache line                                       */
    /*----------------------------------------------------------------------------*/
    static inline void vDecodeVchShocoSplitFast(Datum* pDatum, HeadLine* aHeadLine,
                                                i8 i_Offset)
    {
      byte caShoco[C_4KB];
      
      //i8 i_TailDistance = aHeadLine->w8_2B(i_Offset);
      aHeadLine->w8_2BFast(pDatum, i_Offset);
      i8 i_TailDistance = pDatum->m_wWord;
      
      TailLine* aShocoTail = RowNub::aToTail(aHeadLine,i_TailDistance);
      
      //i8 nCompressed = aHeadLine->w8_1B(i_Offset+2);
      aHeadLine->w8_2BFast(pDatum, i_Offset+2);
      i8 nCompressed = pDatum->m_wWord;
      
      RASSERT(nCompressed <= 56);
      
      i8 nBytesPartA = sizeof(HeadLine) - i_Offset - 3;
      i8 nBytesPartB = nCompressed - nBytesPartA;
      
      MemLib::vCopy((byte*)caShoco+2,
                    (byte*)aHeadLine + i_Offset + 3,
                    nBytesPartA,
                    CN_ROWTRAY);
      MemLib::vCopy((byte*)caShoco+2+nBytesPartA,
                    (byte*)aShocoTail+1,
                    nBytesPartB,
                    CN_ROWTRAY);
      
      //MemLib::vWordDump(caShoco+2,3 * sizeof(SpaceUnit));
      
      i8 nDecompressed =
      Compress::iFromShoco((char*)pDatum->m_sVch.aChar(),
                           255,
                           (byte*)caShoco+2,
                           nCompressed);
      ASSERT(nDecompressed);
      pDatum->m_nChars = nDecompressed;
    }

    /*----------------------------------------------------------------------------*/
    /* decode varchar from given cache line                                       */
    /*----------------------------------------------------------------------------*/
    static inline void vDecodeVchSmallFast(Datum* pDatum, HeadLine* aHeadLine,
                                           eMN eEncoding, i8 i_Offset)
    {
      switch (eEncoding) {
          
        case MN_1B:
          aHeadLine->w8_1BFast(pDatum,i_Offset);
          *(i8*)pDatum->m_sVch.aChar() = pDatum->m_iInt;
          ((char*)pDatum->m_sVch.aChar())[1] = '\0';
          pDatum->m_nChars = 1;
          break;
        case MN_2B:
          aHeadLine->w8_2BFast(pDatum,i_Offset);
          *(i8*)pDatum->m_sVch.aChar() = pDatum->m_iInt;
          ((char*)pDatum->m_sVch.aChar())[2] = '\0';
          pDatum->m_nChars = 2;
          break;
        case MN_3B:
          aHeadLine->w8_3BFast(pDatum,i_Offset);
          *(i8*)pDatum->m_sVch.aChar() = pDatum->m_iInt;
          ((char*)pDatum->m_sVch.aChar())[3] = '\0';
          pDatum->m_nChars = 3;
          break;
        case MN_4B:
          aHeadLine->w8_4BFast(pDatum,i_Offset);
          *(i8*)pDatum->m_sVch.aChar() = pDatum->m_iInt;
          ((char*)pDatum->m_sVch.aChar())[4] = '\0';
          pDatum->m_nChars = 4;
          break;
        case MN_5B:
          aHeadLine->w8_5BFast(pDatum,i_Offset);
          *(i8*)pDatum->m_sVch.aChar() = pDatum->m_iInt;
          ((char*)pDatum->m_sVch.aChar())[5] = '\0';
          pDatum->m_nChars = 5;
          break;
        case MN_6B:
          aHeadLine->w8_6BFast(pDatum,i_Offset);
          *(i8*)pDatum->m_sVch.aChar() = pDatum->m_iInt;
          ((char*)pDatum->m_sVch.aChar())[6] = '\0';
          pDatum->m_nChars = 6;
          break;
        case MN_7B:
          aHeadLine->w8_7BFast(pDatum,i_Offset);
          *(i8*)pDatum->m_sVch.aChar() = pDatum->m_iInt;
          ((char*)pDatum->m_sVch.aChar())[7] = '\0';
          pDatum->m_nChars = 7;
          break;
        case MN_8B:
          aHeadLine->w8_8BFast(pDatum,i_Offset);
          *(i8*)pDatum->m_sVch.aChar() = pDatum->m_iInt;
          ((char*)pDatum->m_sVch.aChar())[8] = '\0';
          pDatum->m_nChars = 8;
          break;
        default:
          NEVER_REACH;
      }
      
    }

    /*----------------------------------------------------------------------------*/
    /* get encoded datum from cache line                                          */
    /*----------------------------------------------------------------------------*/
    static inline void vDecodeFast(Datum* pDatum, HeadLine* aHeadLine, Column* pCol,
                                   Coda* pCoda)
    {
      if (pCol->eDatumType() == DM_BOL) {
        vDecodeBolFast(pDatum,aHeadLine,pCol);
        return;
      }

      vDecodeFast_Fallback(pDatum,aHeadLine,pCol,pCoda);
    }
  
    /*----------------------------------------------------------------------------*/
    /* get the extended encoding (and offset) of a column from given head line    */
    /*----------------------------------------------------------------------------*/
    //! consider passing a eDatum array instead of colum vector
    
    static inline eMN eGetEncodingExtFast(HeadLine* aHeadLine, i8 i_ForCol,
                                          Coda* pCoda, i8& i_Offset_)
    {
    
//! experiment on loop unrolling
//! refer: https://en.wikipedia.org/wiki/Duff%27s_device
//
//      if (1 == i_ForCol) {
//        eMN eEncoding = (eMN)aHeadLine->iMetaNibbleFast(0);
//        eDM eDatumExt = pCoda->m_aeExtDatum[0];
//        i_Offset_ += nDataBytes(eDatumExt,eEncoding);
//        eEncoding = (eMN)aHeadLine->iMetaNibbleFast(1);
//        return eEncoding;
//      }
    
      eMN eEncodingDatum = (eMN)aHeadLine->iMetaNibbleFast(i_ForCol);
      i8 i_Offset1_ = i_Offset_;
      
      if (eEncodingDatum == MN_NULL)
        return eEncodingDatum;
      
      for (i8 i_Nibble = 0; i_Nibble < i_ForCol; i_Nibble++) {
        
        eMN eEncoding = (eMN)aHeadLine->iMetaNibbleFast(i_Nibble);
        
        eDM eDatumExt = pCoda->m_aeExtDatum[i_Nibble];
//        i_Offset1_ += nDataBytes(eDatumExt,eEncoding);
        i_Offset_ += m_iDatumEncoding[eDatumExt][eEncoding];
//        if (i_Offset1_ != i_Offset_) {
//          i8 x = nDataBytes(eDatumExt,eEncoding);
//          i8 y = m_iDatumEncoding[eDatumExt][eEncoding];
//          i8 z = 1;
//        }
//        RASSERT(i_Offset1_ == i_Offset_);
        
//! experiment
//        if (eEncoding > MN_8B) {
//          eDM eDatumExt = pCoda->m_aeExtDatum[i_Nibble];
//          i_Offset_ += nDataBytes(eDatumExt,eEncoding);
//        }
//        else {
//          i_Offset_ += eEncoding;
//        }

      }
      
      
      
      return eEncodingDatum;
    }
    
    /*----------------------------------------------------------------------------*/
    /* get the extended encoding (and offset) of a column from given head line    */
    /*----------------------------------------------------------------------------*/
    //! consider passing a eDatum array instead of colum vector
    
    static inline eMN eGetEncodingExtVchFast(HeadLine* aHeadLine, i8 i_ForCol,
                                          Coda* pCoda, i8& i_Offset_)
    {
      
      //! experiment on loop unrolling
      //! refer: https://en.wikipedia.org/wiki/Duff%27s_device
      //
      //      if (1 == i_ForCol) {
      //        eMN eEncoding = (eMN)aHeadLine->iMetaNibbleFast(0);
      //        eDM eDatumExt = pCoda->m_aeExtDatum[0];
      //        i_Offset_ += nDataBytes(eDatumExt,eEncoding);
      //        eEncoding = (eMN)aHeadLine->iMetaNibbleFast(1);
      //        return eEncoding;
      //      }
      
      eMN eEncodingDatum = (eMN)aHeadLine->iMetaNibbleFast(i_ForCol);
      
      if (eEncodingDatum == MN_NULL || eEncodingDatum == MN_EMPTY)
        return eEncodingDatum;
      
      for (i8 i_Nibble = 0; i_Nibble < i_ForCol; i_Nibble++) {
        
        eMN eEncoding = (eMN)aHeadLine->iMetaNibbleFast(i_Nibble);
        
        eDM eDatumExt = pCoda->m_aeExtDatum[i_Nibble];
        //        i_Offset1_ += nDataBytes(eDatumExt,eEncoding);
        i_Offset_ += m_iDatumEncoding[eDatumExt][eEncoding];
        //        if (i_Offset1_ != i_Offset_) {
        //          i8 x = nDataBytes(eDatumExt,eEncoding);
        //          i8 y = m_iDatumEncoding[eDatumExt][eEncoding];
        //          i8 z = 1;
        //        }
        //        RASSERT(i_Offset1_ == i_Offset_);
        
        //! experiment
        //        if (eEncoding > MN_8B) {
        //          eDM eDatumExt = pCoda->m_aeExtDatum[i_Nibble];
        //          i_Offset_ += nDataBytes(eDatumExt,eEncoding);
        //        }
        //        else {
        //          i_Offset_ += eEncoding;
        //        }
        
      }
      
      
      
      return eEncodingDatum;
    }
    
    /*----------------------------------------------------------------------------*/
    /* get the extended encoding (and offset) of a column from given head line    */
    /*----------------------------------------------------------------------------*/
    //! consider passing a eDatum array instead of colum vector
    
    static inline eMN eGetEncodingExtGenericFast(HeadLine* aHeadLine, i8 i_ForCol,
                                          Coda* pCoda, i8& i_Offset_)
    {
      
      //! experiment on loop unrolling
      //! refer: https://en.wikipedia.org/wiki/Duff%27s_device
      //
      //      if (1 == i_ForCol) {
      //        eMN eEncoding = (eMN)aHeadLine->iMetaNibbleFast(0);
      //        eDM eDatumExt = pCoda->m_aeExtDatum[0];
      //        i_Offset_ += nDataBytes(eDatumExt,eEncoding);
      //        eEncoding = (eMN)aHeadLine->iMetaNibbleFast(1);
      //        return eEncoding;
      //      }
      
      eMN eEncodingDatum = (eMN)aHeadLine->iMetaNibbleFast(i_ForCol);
      if (eEncodingDatum == MN_NULL ||
          eEncodingDatum == MN_MINUS_ONE ||
          eEncodingDatum == MN_ZERO ||
          eEncodingDatum == MN_PLUS_ONE)
        return eEncodingDatum;
      
      for (i8 i_Nibble = 0; i_Nibble < i_ForCol; i_Nibble++) {
        
        eMN eEncoding = (eMN)aHeadLine->iMetaNibbleFast(i_Nibble);
        
        eDM eDatumExt = pCoda->m_aeExtDatum[i_Nibble];
        //i_Offset_ += nDataBytes(eDatumExt,eEncoding);
        i_Offset_ += m_iDatumEncoding[eDatumExt][eEncoding];
        //! experiment
        //        if (eEncoding > MN_8B) {
        //          eDM eDatumExt = pCoda->m_aeExtDatum[i_Nibble];
        //          i_Offset_ += nDataBytes(eDatumExt,eEncoding);
        //        }
        //        else {
        //          i_Offset_ += eEncoding;
        //        }
        
      }
      //eEncoding = (eMN)aHeadLine->iMetaNibbleFast(i_ForCol);
      
      return eEncodingDatum;
    }
    
    /*----------------------------------------------------------------------------*/
    /* get the extended encoding (and offset) of a column from given head line    */
    /*----------------------------------------------------------------------------*/
    //! consider passing a eDatum array instead of colum vector
    
    static inline eMN eGetEncodingExtFastCol0(HeadLine* aHeadLine)
    {
      eMN eEncoding = (eMN)aHeadLine->iMetaNibbleZeroFast();
      
      return eEncoding;
    }
    
    /*----------------------------------------------------------------------------*/
    /* get the extended encoding (and offset) of a column from given head line    */
    /*----------------------------------------------------------------------------*/
    //! consider passing a eDatum array instead of colum vector
    
    static inline eMN eGetEncodingExtFast(i8 i_ForCol, HeadLine* aHeadLine,
                                          Coda* pCoda, i8& i_Offset_)
    {
      eMN eEncoding;
      
      for (i8 i_Nibble = 0; i_Nibble < i_ForCol; i_Nibble++) {
        
        eEncoding = (eMN)aHeadLine->iMetaNibbleFast(i_Nibble);
        
        eDM eDatumExt = pCoda->m_aeExtDatum[i_Nibble];
        //i_Offset_ += nDataBytes(eDatumExt,eEncoding);
        i_Offset_ += m_iDatumEncoding[eDatumExt][eEncoding];
        
      }
      eEncoding = (eMN)aHeadLine->iMetaNibbleFast(i_ForCol);
      
      return eEncoding;
    }
    
  public:
    /*----------------------------------------------------------------------------*/
    /* retrieve and decode row from cache line into a datu                        */
    /*----------------------------------------------------------------------------*/
    static inline void vRetrieve(Datum& oDatum, HeadLine* aHeadLine, Column* pCol,
                                 Coda* pCoda)
    {
      //vHeadDump(aHeadLine, pColVec);
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
//      i8 iRem = (i8)aHeadLine % 64;
//      RASSERT(! iRem);
    
      // get the encoded value into datum
      vDecodeFast(&oDatum,aHeadLine,pCol,pCoda);
    
//      i8 iSum = 0;
//      byte* ayByte = (byte*)aHeadLine;
//      for (i8 _i = 0; _i < 64; _i++)
//        iSum+=*ayByte++;
//      oDatum.m_iInt = iSum;
    }
  

    /*----------------------------------------------------------------------------*/
    /* return base offset for column vector                                       */
    /*----------------------------------------------------------------------------*/
    static inline i8 iBaseOffset(ColumnVec* pColVec)
    {
      i8 nColumns = pColVec->nItems();

      i8 iOffset  = (nColumns + 1) / 2;
    
      //! temporary                                                          FIX
      if (iOffset < 8)
        iOffset = 8;
    
      return iOffset;
    }
  
    /*----------------------------------------------------------------------------*/
    /* compute date from number of days since base date                           */
    /*----------------------------------------------------------------------------*/
    static inline void vFromBaseDaysClassicFast(Datum* pDatum)
    {
      i8 nYearsMonthsDays = pDatum->m_iDte;
      i8 nYears = (nYearsMonthsDays - 1) / 366;
      i8 nMonthsDays = nYearsMonthsDays - (nYears * 366);
      
      i8 i_Month;
      for (i_Month = 1; i_Month <= 11; i_Month++)
        if (nMonthDaySum[i_Month] >= nMonthsDays)
          break;
      
      pDatum->xDte.m_uYearDelta = nYears;
      pDatum->xDte.m_uMonth     = i_Month;
      pDatum->xDte.m_uDay       = nMonthsDays - nMonthDaySum[i_Month - 1];
      pDatum->xDte.m_uSpare     = 0;
    }
    
    /*----------------------------------------------------------------------------*/
    /* compute date from number of days since base date                           */
    /*----------------------------------------------------------------------------*/
    static inline void vFromBaseDaysModernFast(Datum* pDatum)
    {
      i8 nYearsMonthsDays = pDatum->m_iDte + C_MODERN_DAYS;
      
      i8 nYears = (nYearsMonthsDays - 1) / 366;
      i8 nMonthsDays = nYearsMonthsDays - (nYears * 366);
      
      i8 i_Month;
      for (i_Month = 1; i_Month <= 11; i_Month++)
        if (nMonthDaySum[i_Month] >= nMonthsDays)
          break;
      
      pDatum->xDte.m_uYearDelta = nYears;
      pDatum->xDte.m_uMonth     = i_Month;
      pDatum->xDte.m_uDay       = nMonthsDays - nMonthDaySum[i_Month - 1];
      pDatum->xDte.m_uSpare     = 0;
    }
    
    /*----------------------------------------------------------------------------*/
    /* compute date from number of days since base date                           */
    /*----------------------------------------------------------------------------*/
    static inline void vFromBaseDaysSilverFast(Datum* pDatum)
    {
      i8 nYearsMonthsDays = pDatum->m_iDte + C_SILVER_DAYS;
      
      i8 nYears = (nYearsMonthsDays - 1) / 366;
      i8 nMonthsDays = nYearsMonthsDays - (nYears * 366);
      
      i8 i_Month;
      for (i_Month = 1; i_Month <= 11; i_Month++)
        if (nMonthDaySum[i_Month] >= nMonthsDays)
          break;
      
      pDatum->xDte.m_uYearDelta = nYears;
      pDatum->xDte.m_uMonth     = i_Month;
      pDatum->xDte.m_uDay       = nMonthsDays - nMonthDaySum[i_Month - 1];
      pDatum->xDte.m_uSpare     = 0;
    }
    
    /*----------------------------------------------------------------------------*/
    /* restore zero stripped dte                                                  */
    /*----------------------------------------------------------------------------*/
    static inline void vRestoreDteFast(Datum* pDatum, eMN eEncoding)
    {
       switch (eEncoding) {
           
         case MN_NULL:
           break;
           
         case MN_CLASSIC_1B:
         case MN_CLASSIC_2B:
           vFromBaseDaysClassicFast(pDatum);
           break;
         
         case MN_SILVER_1B:
         case MN_SILVER_2B:
           vFromBaseDaysSilverFast(pDatum);
           break;
         
         case MN_MODERN_1B:
         case MN_MODERN_2B:
           vFromBaseDaysModernFast(pDatum);
           break;
         
         default:
           NEVER_REACH;
       }
      
      /*
       if (eEncoding <= MN_CLASSIC_2B) {
       vFromBaseDaysClassicFast(pDatum);
       return;
       }
       
       if (eEncoding <= MN_SILVER_2B) {
       vFromBaseDaysSilverFast(pDatum);
       return;
       }
       
       vFromBaseDaysModernFast(pDatum);
       //pDatum->m_iInt <<= 47;
       */
      
    }
  
  };

  
  RowCoder* _pRowCoder(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
