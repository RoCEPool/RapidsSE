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
  
  /*----------------------------------------------------------------------------*/
  
  class RowCoder : public Object
  {
  public:
  
    ColumnVec* m_pColVec;
    i8         m_nHeadBytes;   // size of head line for this colvec/table
    i8         m_nMetaData;    // size of meta data for this colvec/table
    i8         m_i_BaseOffset; // base offset for this colvec/table
  
    RowNub*    m_pIntoNub;
    i8         m_i_IntoHead;
    HeadLine*  m_aIntoHead;
    i8         m_i_IntoOffset; // encoding offset
  
    RowNub*    m_pFromNub;
    i8         m_i_FromHead;
    HeadLine*  m_aFromHead;
    i8         m_i_FromOffset; // encoding offset
    eMC        m_eFromEncoding;
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
  
    i8 m_anMetaCode[MC_ENUMS];

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
    void vEncodeEnd(void);

    void vReport(RowCoder* pRowCoder_Encode);
  
  private:
    void vAnalyseFree(void);
    
    void vCodeIvl(Datum* pTgtDatum, Datum* pSrcDatum);
  
    // MARK III
    void vCodeBol(Datum* pTgtDatum, Datum* pSrcDatum);
    void vCodeBcd(Datum* pTgtDatum, Datum* pSrcDatum);
    void vCodeDte(Datum* pTgtDatum, Datum* pSrcDatum);
    void vCodeFlt(Datum* pTgtDatum, Datum* pSrcDatum);
    void vCodeInt(Datum* pTgtDatum, Datum* pSrcDatum);
    void vCodeTme(Datum* pTgtDatum, Datum* pSrcDatum);
    void vCodeTsp(Datum* pTgtDatum, Datum* pSrcDatum);
    void vCodeVch(Datum* pTgtDatum, Datum* pSrcDatum);
    
    void vCopyHead_Large(void);
    void vCopyHead_Short(void);
  
    void vDataPut(Datum* pDatum);
    void vDataPut(w8 wDatum, i8 nBytes);
    void vEraDatePut(Datum* pDatum);
  
    void vEncodeIvl(Datum* pSrcDatum, Column* pCol);
    
    void vEncodeVchShoco(Datum* pSrcDatum, Column* pCol);
    void vEncodeVchSmall(Datum* pSrcDatum, Column* pCol);
    
    // MARK III
    void vEncodeBol(Datum* pSrcDatum, Column* pCol);
    void vEncodeBcd(Datum* pSrcDatum, Column* pCol);
    void vEncodeDte(Datum* pSrcDatum, Column* pCol);
    void vEncodeFlt(Datum* pSrcDatum, Column* pCol);
    void vEncodeInt(Datum* pSrcDatum, Column* pCol);
    void vEncodeTme(Datum* pSrcDatum, Column* pCol);
    void vEncodeTsp(Datum* pSrcDatum, Column* pCol);
    void vEncodeVch(Datum* pSrcDatum, Column* pCol);
    
    eMC  eGetEncoding(Column* pCol,
                      i8& i_PriorColumn_, i8& i_Offset_);
    void vHashColumn(Datum* pSrcDatum, Column* pCol);
  
    void vMetaCode(Column* pCol, eMC eMetaCode);
    void vMetaPut(Datum* pDatum, Column* pCol);

    void vRecode_Large(void);

    void vRecodeIvl(Column* pCol);
    void vRecodeVchShoco(Column* pCol);
    bool bRecodeVchShocoDupe(Column* pCol);
    bool bRecodeVchShocoFold(Column* pCol);
    bool bRecodeVchShocoSplit(Column* pCol);
    void vRecodeVchShocoTail(Column* pCol);

    // MARK III
    void vRecode_Short(void);

    // MARK III
    void vRecodeBol(Column* pCol);
    void vRecodeBcd(Column* pCol);
    void vRecodeDte(Column* pCol);
    void vRecodeFlt(Column* pCol);
    void vRecodeInt(Column* pCol);
    bool bRecodeIntDigit(Column* pCol);
    void vRecodeTme(Column* pCol);
    void vRecodeTsp(Column* pCol);
    void vRecodeVch(Column* pCol);
    
    void vUsageIncEncoded(Column* pCol, i8 nBytes, eMC eMetaCode = MC_NULL);
    void vUsageIncEncoded(Column* pCol, Datum* pDatum, eMC eMetaCode = MC_NULL);
    void vUsageIncRecoded(Column* pCol, i8 nBytes, eMC eMetaCode = MC_NULL);
    void vUsageIncOops(Column* pCol, i8 nBytes, i8 nMiss, eMC eMetaCode = MC_NULL);
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
    static void vAppend(Record* pRec, Datum* pDatum, Column* pCol);
    static void vDecode(Datum* pDatum, RowNub* pNub, i8 i_Row, Column* pCol,
                        i8& i_LastColumn, i8& i_LastOffset);
                        
    static void vDecode(Datum* pDatum, HeadLine* aHeadLine,
                        ColumnVec* pColVec, Column* pCol,
                        i8& i_LastColumn, i8& i_LastOffset);
    
    static void vHeadDump(HeadLine* aHeadLine, ColumnVec* pColVec);
    static void vHeadDump(HeadLine* aHeadLine, ColumnVec* pColVec, Column* pCol);
    
    static void vRecord(Record* pRec, RowNub* pNub, i8 i_Row);
    static void vRecord(Record* pRec, HeadLine* aHeadLine, ColumnVec* pColVec);

  private:
  
    static void vAppend(Record* pRec, HeadLine* aHeadLine,
                        ColumnVec* pColVec, Column* pCol,
                        i8& i_LastColumn, i8& i_LastOffset);
  
    static i8   nDataBytes(eMC eMetaCode);

    static void vDateIntoDelta(Datum* pDatum, i8& iEra);
    static void vDateFromDelta(Datum* pDatum, i8& iEra);
    
    
    static void vDecodeIvl(Datum* pDatum, HeadLine* aHeadLine,
                           ColumnVec* pColVec, Column* pCol,
                           i8& i_LastColumn, i8& i_LastOffset);
    
    static void vDecodeVchShoco(Datum* pDatum, HeadLine* aHeadLine,
                                Column* pCol, i8 i_Offset);
    static void vDecodeVchShocoFit(Datum* pDatum, HeadLine* aHeadLine,
                                   Column* pCol, i8 i_Offset);
    static void vDecodeVchShocoSplit(Datum* pDatum, HeadLine* aHeadLine,
                                     Column* pCol, i8 i_Offset);

    // MARK III
    static void vDecodeBol(Datum* pDatum, HeadLine* aHeadLine,
                                 ColumnVec* pColVec, Column* pCol,
                                 i8& i_LastColumn, i8& i_LastOffset);
    static void vDecodeBcd(Datum* pDatum, HeadLine* aHeadLine,
                                 ColumnVec* pColVec, Column* pCol, 
                                 i8& i_LastColumn, i8& i_LastOffset);
    static void vDecodeDte(Datum* pDatum, HeadLine* aHeadLine,
                                 ColumnVec* pColVec, Column* pCol,
                                 i8& i_LastColumn, i8& i_LastOffset);
    static void vDecodeFlt(Datum* pDatum, HeadLine* aHeadLine,
                                 ColumnVec* pColVec, Column* pCol,
                                 i8& i_LastColumn, i8& i_LastOffset);
    static void vDecodeInt(Datum* pDatum, HeadLine* aHeadLine,
                                 ColumnVec* pColVec, Column* pCol,
                                 i8& i_LastColumn, i8& i_LastOffset);
    static void vDecodeTme(Datum* pDatum, HeadLine* aHeadLine,
                                 ColumnVec* pColVec, Column* pCol,
                                 i8& i_LastColumn, i8& i_LastOffset);
    static void vDecodeTsp(Datum* pDatum, HeadLine* aHeadLine,
                                 ColumnVec* pColVec, Column* pCol, 
                                 i8& i_LastColumn, i8& i_LastOffset);
    static void vDecodeVch(Datum* pDatum, HeadLine* aHeadLine,
                                 ColumnVec* pColVec, Column* pCol,
                                 i8& i_LastColumn, i8& i_LastOffset);
    static void vDecodeVchSmall(Datum* pDatum, HeadLine* aHeadLine,
                                      Column* pCol, eMC eEncoding, i8 i_Offset);
                                      
    static i8 iFindTail(RowNub* pNub, TailLine* aFindLine);
  
    static i8 nFreeSpaceHead(HeadLine* aHeadLine, ColumnVec* pColVec);
    
    static eMC eGetEncoding(HeadLine* aHeadLine, Column* pCol,
                            ColumnVec* pColVec,
                            i8& i_PriorColumn_, i8& i_Offset_);
    
    static void vRestoreTme(Datum* pDatum);
    static void vRestoreTsp(Datum* pDatum);
    
    static i8 nShocoTailSize(RowNub* pNub, HeadLine* aHeadLine);

  private:
    Footer* aFooter(void);


    
  public:
    /*----------------------------------------------------------------------------*/
    /* retrieve and decode row from cache line into a datu                        */
    /*----------------------------------------------------------------------------*/
    static inline void vRetrieve(Datum& oDatum, HeadLine* aHeadLine, ColumnVec* pColVec,
                                 Column* pCol, i8& i_LastColumn, i8& i_LastOffset)
    {
      //vHeadDump(aHeadLine, pColVec);
      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
//      i8 iRem = (i8)aHeadLine % 64;
//      RASSERT(! iRem);
    
      // get the encoded value into datum
      vDecode(&oDatum,aHeadLine,pColVec,pCol,i_LastColumn,i_LastOffset);
    
//      i8 iSum = 0;
//      byte* ayByte = (byte*)aHeadLine;
//      for (i8 _i = 0; _i < 64; _i++)
//        iSum+=*ayByte++;
//      oDatum.m_iInt = iSum;
    }
  
    /*----------------------------------------------------------------------------*/
    /* retrieve and decode row from cache line                                    */
    /*----------------------------------------------------------------------------*/
    static inline void vRetrieveAll(HeadLine* aHeadLine, ColumnVec* pColVec)
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
        vDecode(&oDatum,aHeadLine,pColVec,pCol,i_LastColumn,i_LastOffset);
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

  
  };

  
  RowCoder* _pRowCoder(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
