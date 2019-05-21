/*============================================================================*/
/* RowCoder.hpp -- Row Coder Mark III                                         */
/*============================================================================*/
#pragma once

namespace Data
{
  static const i8 C_METABITS = 8;
  
  static const i8 C_TAIL_UNITS_OFFSET = 0;
  static const i8 C_TAIL_UNITS_BYTES  = 1;
  
  static const i8 C_SHOCO_COMPRESSED_OFFSET = C_TAIL_UNITS_OFFSET
                                              + C_TAIL_UNITS_BYTES;
  static const i8 C_SHOCO_COMPRESSED_BYTES  = 1;
  
  static i1 g_iDatumEncoding[DM_ENUMS][MN_OVERFLOW];
  
  /*----------------------------------------------------------------------------*/
  const static i8 nMonthDaySum[13] =
  //    J  F  M   A   M   J   J   A   S   O   N   D
  {0,31,60,91,121,152,182,213,244,274,305,335,366};
  
  class RowCoder : public Object
  {
  public:
  
    ColumnVec* m_pColVec;
    i8         m_nHeadBytes;   // size of head line for this colvec/table
    i8         m_nMetaData;    // size of meta data for this colvec/table
    i8         m_i_BaseOffset; // base offset for this colvec/table
  
    RowNub*    m_pIntoNub;
    Coda*      m_pIntoCoda;
    i8         m_i_IntoHead;
    HeadLine*  m_aIntoHead;
    i8         m_i_IntoOffset; // encoding offset
  
    RowNub*    m_pFromNub;
    i8         m_i_FromHead;
    HeadLine*  m_aFromHead;
    i8         m_i_FromOffset; // encoding offset
    eMN        m_eFromEncoding;
    eMC        m_eFromEncoding_m3;
    i8         m_nFromDataBytes;
  
    RowParts*  m_pWorkParts;
    RowNub*    m_pWorkNub;
    HeadLine*  m_aWorkHead;

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
  
    i8 m_nTotalHeads;

    i8 m_nHeadFreeBytes;
    i8 m_nHeadFreeBytes_0_7;
    i8 m_nHeadFreeBytes_8_15;
    i8 m_nHeadFreeBytes_16_23;
    i8 m_nHeadFreeBytes_24_31;
    i8 m_nHeadFreeBytes_32_39;
    i8 m_nHeadFreeBytes_40_47;
    i8 m_nHeadFreeBytes_48_55;
    i8 m_nHeadFreeBytes_56_63;
    i8 m_nHeadFreeBytes_64_;

    i8 m_nTails;
    i8 m_nTailUnits;

    i8 m_nTotalBuffers;

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

    void vEncodeBegin(RowNub* pIntoNub);
    void vEncodeDatum(Datum* pSrcDatum, Column* pCol);
    void vEncodeDatum_mark3(Datum* pSrcDatum, Column* pCol);
    void vEncodeEnd(void);

    void vReport(RowCoder* pRowCoder_Encode);
  
  private:
    void vAnalyseFree(void);
    void vAnalyseFree_mark3(void);
    
    void vCodeBcd(Datum* pTgtDatum, Datum* pSrcDatum);
    void vCodeBol(Datum* pTgtDatum, Datum* pSrcDatum);
    void vCodeDte(Datum* pTgtDatum, Datum* pSrcDatum);
    void vCodeFlt(Datum* pTgtDatum, Datum* pSrcDatum);
    void vCodeInt(Datum* pTgtDatum, Datum* pSrcDatum);
    void vCodeIvl(Datum* pTgtDatum, Datum* pSrcDatum);
    void vCodeTme(Datum* pTgtDatum, Datum* pSrcDatum);
    void vCodeTsp(Datum* pTgtDatum, Datum* pSrcDatum);
    void vCodeVch(Datum* pTgtDatum, Datum* pSrcDatum);
  
    // MARK III
    void vCodeBol_mark3(Datum* pTgtDatum, Datum* pSrcDatum);
    void vCodeBcd_mark3(Datum* pTgtDatum, Datum* pSrcDatum);
    void vCodeDte_mark3(Datum* pTgtDatum, Datum* pSrcDatum);
    void vCodeFlt_mark3(Datum* pTgtDatum, Datum* pSrcDatum);
    void vCodeInt_mark3(Datum* pTgtDatum, Datum* pSrcDatum);
    void vCodeTme_mark3(Datum* pTgtDatum, Datum* pSrcDatum);
    void vCodeTsp_mark3(Datum* pTgtDatum, Datum* pSrcDatum);
    void vCodeVch_mark3(Datum* pTgtDatum, Datum* pSrcDatum);
    
    void vCopyHead_Large(void);
    void vCopyHead_Short(void);
  
    void vDataPut(Datum* pDatum);
    void vDataPut(w8 wDatum, i8 nBytes);
    void vEraDatePut(Datum* pDatum);
  
    void vEncodeBcd(Datum* pSrcDatum, Column* pCol);
    void vEncodeBol(Datum* pSrcDatum, Column* pCol);
    void vEncodeDte(Datum* pSrcDatum, Column* pCol);
    void vEncodeFlt(Datum* pSrcDatum, Column* pCol);
    void vEncodeInt(Datum* pSrcDatum, Column* pCol);
    void vEncodeIvl(Datum* pSrcDatum, Column* pCol);
    void vEncodeTme(Datum* pSrcDatum, Column* pCol);
    void vEncodeTsp(Datum* pSrcDatum, Column* pCol);
    void vEncodeVch(Datum* pSrcDatum, Column* pCol);
    void vEncodeVchShoco(Datum* pSrcDatum, Column* pCol);
    void vEncodeVchSmall(Datum* pSrcDatum, Column* pCol);
    
    // MARK III
    void vEncodeBol_mark3(Datum* pSrcDatum, Column* pCol);
    void vEncodeBcd_mark3(Datum* pSrcDatum, Column* pCol);
    void vEncodeDte_mark3(Datum* pSrcDatum, Column* pCol);
    void vEncodeFlt_mark3(Datum* pSrcDatum, Column* pCol);
    void vEncodeInt_mark3(Datum* pSrcDatum, Column* pCol);
    void vEncodeTme_mark3(Datum* pSrcDatum, Column* pCol);
    void vEncodeTsp_mark3(Datum* pSrcDatum, Column* pCol);
    void vEncodeVch_mark3(Datum* pSrcDatum, Column* pCol);
    void vEncodeVchShoco_mark3(Datum* pSrcDatum, Column* pCol);
    void vEncodeVchSmall_mark3(Datum* pSrcDatum, Column* pCol);
    
    eMN  eEncodingInitial(Column* pCol,
                          i8& i_PriorColumn_, i8& i_Offset_);
    eMC  eEncodingInitial_mark3(Column* pCol,
                          i8& i_PriorColumn_, i8& i_Offset_);
    void vHashColumn(Datum* pSrcDatum, Column* pCol);
  
    void vMetaCode(Column* pCol, eMN eMetaCode);
    void vMetaCode_mark3(Column* pCol, eMC eMetaCode);
    void vMetaPut(Datum* pDatum, Column* pCol);
    void vMetaPut_mark3(Datum* pDatum, Column* pCol);

    void vRecode_Large(void);
    void vRecode_Short(void);
    void vRecodeBcd(Column* pCol);
    void vRecodeBol(Column* pCol);
    void vRecodeDte(Column* pCol);
    void vRecodeFlt(Column* pCol);
    void vRecodeInt(Column* pCol);
    bool bRecodeIntDigit(Column* pCol);
    void vRecodeIvl(Column* pCol);
    void vRecodeTme(Column* pCol);
    void vRecodeTsp(Column* pCol);
    void vRecodeVch(Column* pCol);
    void vRecodeVchShoco(Column* pCol);
    bool bRecodeVchShocoDupe(Column* pCol);
    bool bRecodeVchShocoFold(Column* pCol);
    bool bRecodeVchShocoSplit(Column* pCol);
    void vRecodeVchShocoTail(Column* pCol);

    // MARK III
    void vRecode_Short_mark3(void);

    // MARK III
    void vRecodeBol_mark3(Column* pCol);
    void vRecodeBcd_mark3(Column* pCol);
    void vRecodeDte_mark3(Column* pCol);
    void vRecodeFlt_mark3(Column* pCol);
    void vRecodeInt_mark3(Column* pCol);
    bool bRecodeIntDigit_mark3(Column* pCol);
    void vRecodeTme_mark3(Column* pCol);
    void vRecodeTsp_mark3(Column* pCol);
    void vRecodeVch_mark3(Column* pCol);
    
    void vUsageIncEncoded(Column* pCol, i8 nBytes);
    void vUsageIncEncoded(Column* pCol, Datum* pDatum);
    void vUsageIncRecoded(Column* pCol, i8 nBytes);
    void vUsageIncOops(Column* pCol, i8 nBytes, i8 nMiss);
    void vUsageReset(void);
  
    void vXchkBcd(Datum* pDatum, Column* pCol, i8 iRow);
    void vXchkBol(Datum* pDatum, Column* pCol, i8 iRow);
    void vXchkDte(Datum* pDatum, Column* pCol, i8 iRow);
    void vXchkFlt(Datum* pDatum, Column* pCol, i8 iRow);
    void vXchkInt(Datum* pDatum, Column* pCol, i8 iRow);
    void vXchkIvl(Datum* pDatum, Column* pCol, i8 iRow);
    void vXchkTme(Datum* pDatum, Column* pCol, i8 iRow);
    void vXchkTsp(Datum* pDatum, Column* pCol, i8 iRow);
    void vXchkVch(Datum* pDatum, Column* pCol, i8 iRow);
    
    void vXchkBcdRecode(Column* pCol);
    void vXchkBolRecode(Column* pCol);
    void vXchkDteRecode(Column* pCol);
    void vXchkFltRecode(Column* pCol);
    void vXchkIntRecode(Column* pCol);
    void vXchkIvlRecode(Column* pCol);
    void vXchkTmeRecode(Column* pCol);
    void vXchkTspRecode(Column* pCol);
    void vXchkVchRecode(Column* pCol);
    
  public:
    static void vAnalyse(RowNub* pNub);
    static void vAnalyse_mark3(RowNub* pNub);
    static void vAppend(Record* pRec, Datum* pDatum, Column* pCol);
    static void vDecode(Datum* pDatum, RowNub* pNub, i8 i_Row, Column* pCol,
                        i8& i_LastColumn, i8& i_LastOffset);
    static void vDecode(Datum* pDatum, HeadLine* aHeadLine,
                        ColumnVec* pColVec, Column* pCol, Coda* pCoda,
                        i8& i_LastColumn, i8& i_LastOffset);
                        
    static void vDecode_mark3(Datum* pDatum, HeadLine* aHeadLine,
                        ColumnVec* pColVec, Column* pCol,
                        i8& i_LastColumn, i8& i_LastOffset);
    static void vDecodeFast_Fallback(Datum* pDatum, HeadLine* aHeadLine,
                                     ColumnVec* pColVec, Column* pCol,
                                     Coda* pCoda, i8& i_LastColumn, i8& i_LastOffset);
    static void vHeadDump(HeadLine* aHeadLine, ColumnVec* pColVec);
    static void vHeadDump(HeadLine* aHeadLine, ColumnVec* pColVec, Column* pCol);
    static void vHeadDump(HeadLine* aHeadLine, ColumnVec* pColVec, Coda* pCoda);
    static void vHeadDump(HeadLine* aHeadLine, ColumnVec* pColVec, Column* pCol, Coda* pCoda);
    static void vHeadDumpNat(HeadLine* aHeadLine, ColumnVec* pColVec);
    
    static void vHeadDump_mark3(HeadLine* aHeadLine, ColumnVec* pColVec);
    static void vHeadDump_mark3(HeadLine* aHeadLine, ColumnVec* pColVec, Column* pCol);
    static void vHeadDumpNat_mark3(HeadLine* aHeadLine, ColumnVec* pColVec);
    
    static void vInitEncoding(void);
    static void vRecord(Record* pRec, RowNub* pNub, i8 i_Row);
    static void vRecord(Record* pRec, HeadLine* aHeadLine, ColumnVec* pColVec);

  private:
    static void vAppend(Record* pRec, HeadLine* aHeadLine,
                        ColumnVec* pColVec, Column* pCol, Coda* pCoda,
                        i8& i_LastColumn, i8& i_LastOffset);
  
    static void vAppend_mark3(Record* pRec, HeadLine* aHeadLine,
                        ColumnVec* pColVec, Column* pCol,
                        i8& i_LastColumn, i8& i_LastOffset);
  
    static i8   nDataBytes(eDM eDatum, eMN eEncoding);
    static i8   nDataBytes_mark3(eDM eDatum, eMC eMetaCode);
  
    static void vDaysFromBase(Datum* pDatum, i8& iEra);
    static i8   iDaysIntoBase(i8 nDaysWithinEra, i8 iEra);
    static void vDaysIntoBase(Datum* pDatum, i8& iEra);

    static void vDateIntoDelta_mark3(Datum* pDatum, i8& iEra);
    static void vDateFromDelta_mark3(Datum* pDatum, i8& iEra);
    
    static void vDecodeBcd(Datum* pDatum, HeadLine* aHeadLine,
                           ColumnVec* pColVec, Column* pCol, Coda* pCoda,
                           i8& i_LastColumn, i8& i_LastOffset);
    static void vDecodeBol(Datum* pDatum, HeadLine* aHeadLine,
                           ColumnVec* pColVec, Column* pCol, Coda* pCoda,
                           i8& i_LastColumn, i8& i_LastOffset);
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
    static void vDecodeVchShoco(Datum* pDatum, HeadLine* aHeadLine,
                                Column* pCol, i8 i_Offset);
    static void vDecodeVchShocoFit(Datum* pDatum, HeadLine* aHeadLine,
                                   Column* pCol, i8 i_Offset);
    static void vDecodeVchShocoSplit(Datum* pDatum, HeadLine* aHeadLine,
                                     Column* pCol, i8 i_Offset);
    static void vDecodeVchSmall(Datum* pDatum, HeadLine* aHeadLine,
                                Column* pCol, eMN eEncoding, i8 i_Offset);


    // MARK III
    static void vDecodeBol_mark3(Datum* pDatum, HeadLine* aHeadLine,
                                 ColumnVec* pColVec, Column* pCol,
                                 i8& i_LastColumn, i8& i_LastOffset);
    static void vDecodeBcd_mark3(Datum* pDatum, HeadLine* aHeadLine,
                                 ColumnVec* pColVec, Column* pCol, 
                                 i8& i_LastColumn, i8& i_LastOffset);
   static void vDecodeDte_mark3(Datum* pDatum, HeadLine* aHeadLine,
                                 ColumnVec* pColVec, Column* pCol,
                                 i8& i_LastColumn, i8& i_LastOffset);
    static void vDecodeFlt_mark3(Datum* pDatum, HeadLine* aHeadLine,
                                 ColumnVec* pColVec, Column* pCol,
                                 i8& i_LastColumn, i8& i_LastOffset);
    static void vDecodeInt_mark3(Datum* pDatum, HeadLine* aHeadLine,
                                 ColumnVec* pColVec, Column* pCol,
                                 i8& i_LastColumn, i8& i_LastOffset);
    static void vDecodeTme_mark3(Datum* pDatum, HeadLine* aHeadLine,
                                 ColumnVec* pColVec, Column* pCol,
                                 i8& i_LastColumn, i8& i_LastOffset);
    static void vDecodeTsp_mark3(Datum* pDatum, HeadLine* aHeadLine,
                                 ColumnVec* pColVec, Column* pCol, 
                                 i8& i_LastColumn, i8& i_LastOffset);
    static void vDecodeVch_mark3(Datum* pDatum, HeadLine* aHeadLine,
                                 ColumnVec* pColVec, Column* pCol,
                                 i8& i_LastColumn, i8& i_LastOffset);
    static void vDecodeVchSmall_mark3(Datum* pDatum, HeadLine* aHeadLine,
                                      Column* pCol, eMC eEncoding, i8 i_Offset);
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

    static void vDecodeDigit_mark3(Datum* pDatum, HeadLine* aHeadLine,
                                   ColumnVec* pColVec, Column* pCol, Coda* pCoda,
                                   i8& i_LastColumn, i8& i_LastOffset);
    static void vDecodeTenth_mark3(Datum* pDatum, HeadLine* aHeadLine,
                                   ColumnVec* pColVec, Column* pCol, Coda* pCoda,
                                   i8& i_LastColumn, i8& i_LastOffset);
                                   
    static eMN eEncodingInitial(HeadLine* aHeadLine, Column* pCol,
                                ColumnVec* pColVec,
                                i8& i_PriorColumn_, i8& i_Offset_);
    static eMC eEncodingInitial_mark3(HeadLine* aHeadLine, Column* pCol,
                                ColumnVec* pColVec,
                                i8& i_PriorColumn_, i8& i_Offset_);
    static eMN eEncodingRecoded(HeadLine* aHeadLine, Column* pCol,
                                ColumnVec* pColVec, Coda* pCoda,
                                i8& i_PriorColumn_, i8& i_Offset_);
    static eMC eEncodingRecoded_mark3(HeadLine* aHeadLine, Column* pCol,
                                ColumnVec* pColVec,
                                i8& i_PriorColumn_, i8& i_Offset_);
    static Stz* sEncoding(eMN eEncoding, eDM eDatumNat, eDM eDatumExt);
  
    static i8 iFindTail(RowNub* pNub, TailLine* aFindLine);
  
    static i8 nFreeSpaceHead(HeadLine* aHeadLine, ColumnVec* pColVec, Coda* pCoda);
    static i8 nFreeSpaceHead_mark3(HeadLine* aHeadLine, ColumnVec* pColVec);
    
    static void vRestoreDte(Datum* pDatum, eMN eEncoding);
    static void vRestoreTme(Datum* pDatum);
    static void vRestoreTsp(Datum* pDatum);
    
    static i8 nShocoTailSize(RowNub* pNub, HeadLine* aHeadLine);

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
    /* decode varchar from given cache line                                       */
    /*----------------------------------------------------------------------------*/
    static inline void vDecodeAlphaFast(Datum* pDatum, HeadLine* aHeadLine,
                                        eMN eEncoding, Column* pCol, Coda* pCoda, i8 i_Offset)
    {
      i8 i_Col  = pCol->iColumnNo();
      
      pDatum->m_eDatum = DM_VCH;
      
      //RASSERT(eEncoding >= MN_ALPHA_0 && eEncoding <= MN_ALPHA_5);
      
      if (eEncoding < MN_ALPHA_0 || eEncoding > MN_ALPHA_5) {
        vDecodeVchFast(pDatum,aHeadLine,eEncoding,i_Offset);
        return;
      }
      
      //vDecodeVchSmall(pDatum,aHeadLine,pCol,eEncoding,i_Offset);
      
      pDatum->m_sVch.aChar()[0] = pCoda->m_aiAux[i_Col].m_caChr[eEncoding - MN_ALPHA_0];
      pDatum->m_sVch.aChar()[1] = '\0';
      
      //char cChr = aHeadLine->w8_1B(i_Offset);
      //RASSERT(cChr == pDatum->m_sVch.aChar()[0]);
      
      //RASSERT(pDatum->m_sVch.aChar()[0] == (char)pCoda->m_aiAux[i_Col]);
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
    /* decode days from given cache line                                          */
    /*----------------------------------------------------------------------------*/
    static inline void vDecodeDaysFast(Datum* pDatum, HeadLine* aHeadLine, eMN eEncoding,
                                       ColumnVec* pColVec, Column* pCol, Coda* pCoda, i8 i_Offset)
    {
      i8 i_FirstColumn  = pCol->iFirstDate();
      
      pDatum->m_eDatum = DM_DTE;
      i8 iEra;
      
      if (eEncoding == MN_DAYS_PLUS_1B || eEncoding == MN_DAYS_MINUS_1B) {
        i8 i_SearchColumn = 0;
        i8 i_SearchOffset = iBaseOffset(pColVec);
        Column* pColFirstDate  = pColVec->pGet(pCol->iFirstDate());
        eMN eFirstEncoding = eEncodingRecoded(aHeadLine,pColFirstDate,pColVec,
                                              pCoda,i_SearchColumn,i_SearchOffset);
        
        switch (eFirstEncoding) {
          case MN_SILVER_1B:    pDatum->m_iInt = aHeadLine->w8_1B(i_SearchOffset); iEra = C_SILVER_ERA; break;
          case MN_SILVER_2B:    pDatum->m_iInt = aHeadLine->w8_2B(i_SearchOffset); iEra = C_SILVER_ERA; break;
          case MN_CLASSIC_1B:   pDatum->m_iInt = aHeadLine->w8_1B(i_SearchOffset); iEra = C_CLASSIC_ERA; break;
          case MN_CLASSIC_2B:   pDatum->m_iInt = aHeadLine->w8_2B(i_SearchOffset); iEra = C_CLASSIC_ERA; break;
          default:
            NEVER_REACH;
        }
      }
      
      i8 iDays = 0;
      switch (eEncoding) {
          
        case MN_NULL: pDatum->m_iInt = C_DATUM_NULL; return;
          
        case MN_DAYS_PLUS_1B:     iDays = aHeadLine->w8_1B(i_Offset);   break;
        case MN_DAYS_MINUS_1B:    iDays = - aHeadLine->w8_1B(i_Offset); break;
        case MN_DAYS_CLASSIC_2B:  iDays = aHeadLine->w8_2B(i_Offset);  iEra = C_CLASSIC_ERA; break;
        case MN_DAYS_SILVER_2B:   iDays = aHeadLine->w8_2B(i_Offset);  iEra = C_SILVER_ERA; break;
        case MN_DAYS_MODERN_2B:   iDays = aHeadLine->w8_2B(i_Offset);  iEra = C_MODERN_ERA; break;
        default:
          NEVER_REACH;
      }
      
      if (eEncoding == MN_DAYS_PLUS_1B || eEncoding == MN_DAYS_MINUS_1B) {
        pDatum->m_iInt += iDays;
      }
      else {
        pDatum->m_iInt = iDays;
      }
      vDaysFromBase(pDatum,iEra);
    }

    /*----------------------------------------------------------------------------*/
    /* decode integer from given cache line                                       */
    /*----------------------------------------------------------------------------*/
    static inline void vDecodeDeltaPlusFast(Datum* pDatum, HeadLine* aHeadLine,eMN eEncoding,
                                        Column* pCol, Coda* pCoda, i8 i_Offset)
    {
      i8 i_Col  = pCol->iColumnNo();
      
      pDatum->m_eDatum = DM_INT;
      
      switch (eEncoding) {
          
        case MN_NULL: pDatum->m_iInt = C_DATUM_NULL;               break;
        case MN_1B:   pDatum->m_iInt = aHeadLine->w8_1B(i_Offset); break;
        case MN_2B:   pDatum->m_iInt = aHeadLine->w8_2B(i_Offset); break;
        case MN_3B:   pDatum->m_iInt = aHeadLine->w8_3B(i_Offset); break;
        case MN_4B:   pDatum->m_iInt = aHeadLine->w8_4B(i_Offset); break;
        case MN_5B:   pDatum->m_iInt = aHeadLine->w8_5B(i_Offset); break;
        case MN_6B:   pDatum->m_iInt = aHeadLine->w8_6B(i_Offset); break;
        case MN_7B:   pDatum->m_iInt = aHeadLine->w8_7B(i_Offset); break;
        case MN_8B:   pDatum->m_iInt = aHeadLine->w8_8B(i_Offset); break;
          
        case MN_MINUS_ONE: pDatum->m_iInt = (i8)-1; break;
        case MN_ZERO:      pDatum->m_iInt = (i8)0;  break;
        case MN_PLUS_ONE:  pDatum->m_iInt = (i8)1;  break;
          
        case MN_SAME:
          pDatum->m_iInt = pCoda->m_aiAux[i_Col].m_iInt;
          break;
          
        case MN_PLUS_1B:
          pDatum->m_iInt = pCoda->m_aiAux[i_Col].m_iInt
          + aHeadLine->w8_1B(i_Offset);
          break;
          
        case MN_PLUS_2B:
          pDatum->m_iInt = pCoda->m_aiAux[i_Col].m_iInt
          + aHeadLine->w8_2B(i_Offset);
          break;
          
          //
          //    case MN_MINUS_1B:
          //      pDatum->m_iInt = pCoda->m_aiAux[i_Col].m_iInt
          //                     - aHeadLine->w8_1B(i_Offset);
          break;
          
        default:
          NEVER_REACH;
      }
    }
    
    /*----------------------------------------------------------------------------*/
    /* decode digit from given cache line                                         */
    /*----------------------------------------------------------------------------*/
    static inline void vDecodeDigitFast(Datum* pDatum, HeadLine* aHeadLine,eMN eEncoding,
                                        Column* pCol, Coda* pCoda, i8 i_Offset)
    {
      i8 i_Col  = pCol->iColumnNo();
      
      pDatum->m_eDatum = DM_INT;
      
      if (eEncoding >= MN_DIGIT_0 && eEncoding <= MN_DIGIT_7) {
        pDatum->m_iInt = (i8)(eEncoding - MN_DIGIT_0);
      }
      else {
        switch (eEncoding) {
          case MN_NULL:       pDatum->m_iInt = C_DATUM_NULL;                break;
          case MN_DIGIT_1B:   pDatum->m_iInt = aHeadLine->w8_1B(i_Offset); break;
          case MN_DIGIT_2B:   pDatum->m_iInt = aHeadLine->w8_2B(i_Offset); break;
          case MN_DIGIT_3B:   pDatum->m_iInt = aHeadLine->w8_3B(i_Offset); break;
          case MN_DIGIT_4B:   pDatum->m_iInt = aHeadLine->w8_4B(i_Offset); break;
          case MN_DIGIT_6B:   pDatum->m_iInt = aHeadLine->w8_6B(i_Offset); break;
          case MN_DIGIT_8B:   pDatum->m_iInt = aHeadLine->w8_8B(i_Offset); break;
          default:
            NEVER_REACH;
        }
      }
      
    }

    /*----------------------------------------------------------------------------*/
    /* decode date from given cache line                                          */
    /*----------------------------------------------------------------------------*/
    static inline void vDecodeDteFast(Datum* pDatum, HeadLine* aHeadLine,
                                      eMN eEncoding, i8 i_Offset)
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
    /* decode tenth from given cache line                                         */
    /*----------------------------------------------------------------------------*/
    static inline void vDecodeTenthFast(Datum* pDatum, HeadLine* aHeadLine, eMN eEncoding,
                                    Column* pCol, i8 i_Offset)
    {
      i8 i_Col  = pCol->iColumnNo();
      
      pDatum->m_eDatum = DM_BCD;
      
      if (eEncoding >= MN_TENTH_0 && eEncoding <= MN_TENTH_10) {
        pDatum->m_iBcd = (i8)(eEncoding - MN_TENTH_0);
      }
      else {
        switch (eEncoding) {
          case MN_NULL:       pDatum->m_iInt = C_DATUM_NULL;                break;
          case MN_TENTH_2B:   pDatum->m_iInt = aHeadLine->w8_2B(i_Offset); break;
          case MN_TENTH_4B:   pDatum->m_iInt = aHeadLine->w8_4B(i_Offset); break;
          case MN_TENTH_8B:   pDatum->m_iInt = aHeadLine->w8_8B(i_Offset); break;
          default:
            NEVER_REACH;
        }
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
      
      //RASSERT(nCompressed <= 56);
      
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
      aHeadLine->w8_1BFast(pDatum, i_Offset+2);
      i8 nCompressed = pDatum->m_wWord;
      
      //RASSERT(nCompressed <= 56);
      
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
    static inline void vDecodeFast(Datum* pDatum, HeadLine* aHeadLine, ColumnVec* pColumnVec,
                                   Column* pCol, Coda* pCoda, i8& i_LastColumn, i8& i_LastOffset)
    {
      if (pCol->eDatumType() == DM_BOL) {
        vDecodeBolFast(pDatum,aHeadLine,pCol);
        return;
      }

      vDecodeFast_Fallback(pDatum,aHeadLine,pColumnVec,pCol,pCoda,i_LastColumn,i_LastOffset);
    }
  
    /*----------------------------------------------------------------------------*/
    /* get the extended encoding (and offset) of a column from given head line    */
    /*----------------------------------------------------------------------------*/
    //! consider passing a eDatum array instead of colum vector
    
    static inline eMN eGetEncodingExtFast(HeadLine* aHeadLine, i8 i_ForCol,
                                          Coda* pCoda, i8& i_LastColumn, i8& i_LastOffset_)
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
      //i8 i_Offset1_ = i_LastOffset_;
      
      if (eEncodingDatum == MN_NULL)
        return eEncodingDatum;
      
      for (i8 i_Nibble = i_LastColumn; i_Nibble < i_ForCol; i_Nibble++) {
        
        eMN eEncoding = (eMN)aHeadLine->iMetaNibbleFast(i_Nibble);
        
        eDM eDatumExt = pCoda->m_aeExtDatum[i_Nibble];
//        i_Offset1_ += nDataBytes(eDatumExt,eEncoding);
        i_LastOffset_ += g_iDatumEncoding[eDatumExt][eEncoding];
//        if (i_Offset1_ != i_LastOffset_) {
//          i8 x = nDataBytes(eDatumExt,eEncoding);
//          i8 y = g_iDatumEncoding[eDatumExt][eEncoding];
//          i8 z = 1;
//        }
//        RASSERT(i_Offset1_ == i_LastOffset_);
        
//! experiment
//        if (eEncoding > MN_8B) {
//          eDM eDatumExt = pCoda->m_aeExtDatum[i_Nibble];
//          i_Offset_ += nDataBytes(eDatumExt,eEncoding);
//        }
//        else {
//          i_Offset_ += eEncoding;
//        }

      }
      i_LastColumn = i_ForCol;
      
      
      return eEncodingDatum;
    }
    
    /*----------------------------------------------------------------------------*/
    /* get the extended encoding (and offset) of a column from given head line    */
    /*----------------------------------------------------------------------------*/
    //! consider passing a eDatum array instead of colum vector
    
    static inline eMN eGetEncodingExtVchFast(HeadLine* aHeadLine, i8 i_ForCol,
                                          Coda* pCoda, i8& i_LastColumn, i8& i_LastOffset_)
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
      //i8 i_Offset1_ = i_LastOffset_;
      
      if (eEncodingDatum == MN_NULL || eEncodingDatum == MN_EMPTY)
        return eEncodingDatum;
      
      for (i8 i_Nibble = i_LastColumn; i_Nibble < i_ForCol; i_Nibble++) {
        
        eMN eEncoding = (eMN)aHeadLine->iMetaNibbleFast(i_Nibble);
        
        eDM eDatumExt = pCoda->m_aeExtDatum[i_Nibble];
        //i_Offset1_ += nDataBytes(eDatumExt,eEncoding);
        i_LastOffset_ += g_iDatumEncoding[eDatumExt][eEncoding];
//        if (i_Offset1_ != i_LastOffset_) {
//          i8 x = nDataBytes(eDatumExt,eEncoding);
//          i8 y = g_iDatumEncoding[eDatumExt][eEncoding];
//          i8 z = 1;
//        }
//        RASSERT(i_Offset1_ == i_LastOffset_);
        
        //! experiment
        //        if (eEncoding > MN_8B) {
        //          eDM eDatumExt = pCoda->m_aeExtDatum[i_Nibble];
        //          i_Offset_ += nDataBytes(eDatumExt,eEncoding);
        //        }
        //        else {
        //          i_Offset_ += eEncoding;
        //        }
        
      }
      
      i_LastColumn = i_ForCol;
      
      return eEncodingDatum;
    }
    
    /*----------------------------------------------------------------------------*/
    /* get the extended encoding (and offset) of a column from given head line    */
    /*----------------------------------------------------------------------------*/
    //! consider passing a eDatum array instead of colum vector
    
    static inline eMN eGetEncodingExtGenericFast(HeadLine* aHeadLine, i8 i_ForCol,
                                          Coda* pCoda, i8& i_LastColumn, i8& i_LastOffset_)
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
      
      RASSERT(i_LastColumn <= i_ForCol);
      
      for (i8 i_Nibble = i_LastColumn; i_Nibble < i_ForCol; i_Nibble++) {
        
        eMN eEncoding = (eMN)aHeadLine->iMetaNibbleFast(i_Nibble);
        
        eDM eDatumExt = pCoda->m_aeExtDatum[i_Nibble];
        //i_Offset_ += nDataBytes(eDatumExt,eEncoding);
        i_LastOffset_ += g_iDatumEncoding[eDatumExt][eEncoding];
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
      i_LastColumn = i_ForCol;
      
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
        i_Offset_ += g_iDatumEncoding[eDatumExt][eEncoding];
        
      }
      eEncoding = (eMN)aHeadLine->iMetaNibbleFast(i_ForCol);
      
      return eEncoding;
    }
    
  public:
    /*----------------------------------------------------------------------------*/
    /* retrieve and decode row from cache line into a datu                        */
    /*----------------------------------------------------------------------------*/
    static inline void vRetrieve(Datum& oDatum, HeadLine* aHeadLine, ColumnVec* pColVec,
                                 Column* pCol, Coda* pCoda, i8& i_LastColumn, i8& i_LastOffset)
    {
      //vHeadDump(aHeadLine, pColVec);
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
//      i8 iRem = (i8)aHeadLine % 64;
//      RASSERT(! iRem);
    
      // get the encoded value into datum
      vDecode(&oDatum,aHeadLine,pColVec,pCol,pCoda,i_LastColumn,i_LastOffset);
    
//      i8 iSum = 0;
//      byte* ayByte = (byte*)aHeadLine;
//      for (i8 _i = 0; _i < 64; _i++)
//        iSum+=*ayByte++;
//      oDatum.m_iInt = iSum;
    }
  
    /*----------------------------------------------------------------------------*/
    /* retrieve and decode row from cache line                                    */
    /*----------------------------------------------------------------------------*/
    static inline void vRetrieveAll(HeadLine* aHeadLine, ColumnVec* pColVec, Coda* pCoda)
    {
      Datum oDatum;
      
      //vHeadDump(aHeadLine, pColVec);
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      
      //      i8 iRem = (i8)aHeadLine % 64;
      //      RASSERT(! iRem);
      
      // get no of columns
      i8 nColumns = pColVec->nItems();
      i8 i_LastColumn = 0;
      i8 i_LastOffset = iBaseOffset(pColVec);
      
      // for each column
      for (i8 i_Col = 0; i_Col < nColumns; i_Col++) {
        
        // get the column at index
        Column* pCol = pColVec->pGet(i_Col);
        
        // get the encoded value into datum
        vDecode(&oDatum,aHeadLine,pColVec,pCol,pCoda,i_LastColumn,i_LastOffset);
      }
      
      //      i8 iSum = 0;
      //      byte* ayByte = (byte*)aHeadLine;
      //      for (i8 _i = 0; _i < 64; _i++)
      //        iSum+=*ayByte++;
      //      oDatum.m_iInt = iSum;
    }
    
    /*----------------------------------------------------------------------------*/
    /* calc size of the meta tape needed for given column vector                  */
    /*----------------------------------------------------------------------------*/
    static inline i8 nMetaData(ColumnVec* pColVec)
    {
      i8 nColumns = pColVec->nItems();
    
      i8 nBits = C_BITS_PER_BYTE + (nColumns * C_METABITS);
    
      i8 nMetaBytes = nBits / C_BITS_PER_BYTE
                        + (nBits % C_BITS_PER_BYTE ? 1 : 0);
    
      return nMetaBytes;
    }
  
    /*----------------------------------------------------------------------------*/
    /* calc size of the head line for column vector                               */
    /*----------------------------------------------------------------------------*/
    static inline i8 nHeadBytes(ColumnVec* pColVec)
    {
    i8 nBytes = sizeof(CacheLine) + (2 * C_BYTES_PER_WORD);
    
      return nBytes;
    }
  
    

    /*----------------------------------------------------------------------------*/
    /* calc base offset for column vector                                         */
    /*----------------------------------------------------------------------------*/
    static inline i8 iBaseOffset(ColumnVec* pColVec)
    {
      i8 nColumns = pColVec->nItems();
    
      i8 iOffset  = nMetaData(pColVec);
    
      //! temporary                                                          FIX
      if (iOffset < 8)
        iOffset = 8;
    
      return iOffset;
    }
    /*----------------------------------------------------------------------------*/
//    static inline i8 iBaseOffset(ColumnVec* pColVec)
//    {
//      i8 nColumns = pColVec->nItems();
//
//      i8 iOffset  = (nColumns + 1) / 2;
//
//      //! temporary                                                          FIX
//      if (iOffset < 8)
//        iOffset = 8;
//
//      return iOffset;
//    }
  
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
