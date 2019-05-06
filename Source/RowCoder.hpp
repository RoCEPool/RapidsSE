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
    
    void vRecode(i8 i_IntoHead, RowNub* pFromNub, HeadLine* aFromHead);
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
    static void vHeadDumpColumn(HeadLine* aHeadLine, ColumnVec* pColVec, i8 iCol, Stz* aRecodeFunction);
    static void vHeadDumpNat(HeadLine* aHeadLine, ColumnVec* pColVec);
    static void vHeadDumpNatColumn(HeadLine* aHeadLine, ColumnVec* pColVec, i8 iCol);
    static void vInitEncoding(void);
    static void vPrint(RowNub* pNub, i8 i_Row, Column* pCol, i8& i_LastColumn, i8& i_LastOffset);
    static void vRecord(Record* pRec, RowNub* pNub, i8 i_Row);
    static void vRecord(Record* pRec, HeadLine* aHeadLine, ColumnVec* pColVec);
  
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

    static i8 iStripDte(Word& wTspView, eMN eEncoding);
    static i8 iStripTme(Word& wTspView, eMN eEncoding);
    static i8 iStripTsp(Word& wTspView, eMN eEncoding);
 
    static i8 nShocoTailSize(RowNub* pNub, HeadLine* aHeadLine);
  
    static void vSetAuxInt(Coda* pIntoCoda, HeadLine* aIntoHead, Column* pCol,
                           eMN eEncoding, i8 i_IntoOffset);
  
    void vUsageIncEncoded(Column* pCol, i8 nBytes);
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
    
      for (i8 i_Nibble = 0; i_Nibble < i_ForCol; i_Nibble++) {
        
        eMN eEncoding = (eMN)aHeadLine->iMetaNibbleFast(i_Nibble);
        
        eDM eDatumExt = pCoda->m_aeExtDatum[i_Nibble];
        i_Offset_ += nDataBytes(eDatumExt,eEncoding);
        
//! experiment
//        if (eEncoding > MN_8B) {
//          eDM eDatumExt = pCoda->m_aeExtDatum[i_Nibble];
//          i_Offset_ += nDataBytes(eDatumExt,eEncoding);
//        }
//        else {
//          i_Offset_ += eEncoding;
//        }

      }
      
      eMN eEncoding = (eMN)aHeadLine->iMetaNibbleFast(i_ForCol);
      
      return eEncoding;
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
      
      eMN eEncoding = (eMN)aHeadLine->iMetaNibbleFast(i_ForCol);
      if (eEncoding == MN_NULL ||
          eEncoding == MN_MINUS_ONE ||
          eEncoding == MN_ZERO ||
          eEncoding == MN_PLUS_ONE)
        return eEncoding;
      
      for (i8 i_Nibble = 0; i_Nibble < i_ForCol; i_Nibble++) {
        
        eEncoding = (eMN)aHeadLine->iMetaNibbleFast(i_Nibble);
        
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
      eEncoding = (eMN)aHeadLine->iMetaNibbleFast(i_ForCol);
      
      return eEncoding;
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
    
      i8 iOffset = (nColumns + 1) / 2;
    
      iOffset = 8; //! temp
    
      return iOffset;
    }
  

  
  };
  
  RowCoder* _pRowCoder(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
