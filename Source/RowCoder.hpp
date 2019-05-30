/*============================================================================*/
/* RowCoder.hpp -- Row Coder Mark III                                         */
/*============================================================================*/
#pragma once

namespace Data
{
  static const i8 C_METABITS = 8;
  
  static const i8 _2B_FT = 2;
  static const i8 _2B_IT = 2;
  static const i8 _1B_C  = 1;
  static const i8 _1B_L  = 1;
  static const i8 _1B_U  = 1;
  static const i8 _1B_W  = 1;

  static const i8 C_OFFSET_UNITS = 0;
  static const i8 C_OFFSET_COMPRESSED = C_OFFSET_UNITS + _1B_U;


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
    
    void vCodeBol(Datum* pTgtDatum, Datum* pSrcDatum);
    void vCodeBcd(Datum* pTgtDatum, Datum* pSrcDatum);
    void vCodeDte(Datum* pTgtDatum, Datum* pSrcDatum);
    void vCodeFlt(Datum* pTgtDatum, Datum* pSrcDatum);
    void vCodeInt(Datum* pTgtDatum, Datum* pSrcDatum);
    void vCodeIvl(Datum* pTgtDatum, Datum* pSrcDatum);
    void vCodeTme(Datum* pTgtDatum, Datum* pSrcDatum);
    void vCodeTsp(Datum* pTgtDatum, Datum* pSrcDatum);
    void vCodeVch(Datum* pTgtDatum, Datum* pSrcDatum);
    void vCopyHead_Large(void);
    void vCopyHead_Short(void);
  
    void vDataPut(Datum* pDatum);
    void vDataPut(w8 wDatum, i8 nBytes);
    
    void vEraDatePut(Datum* pDatum);
    void vEncodeBol(Datum* pSrcDatum, Column* pCol);
    void vEncodeBcd(Datum* pSrcDatum, Column* pCol);
    void vEncodeDte(Datum* pSrcDatum, Column* pCol);
    void vEncodeFlt(Datum* pSrcDatum, Column* pCol);
    void vEncodeInt(Datum* pSrcDatum, Column* pCol);
    void vEncodeIvl(Datum* pSrcDatum, Column* pCol);
    void vEncodeTme(Datum* pSrcDatum, Column* pCol);
    void vEncodeTsp(Datum* pSrcDatum, Column* pCol);
    void vEncodeVch(Datum* pSrcDatum, Column* pCol);
    void vEncodeVchShoco(Datum* pSrcDatum, Column* pCol);
    void vEncodeVchSmall(Datum* pSrcDatum, Column* pCol);

    eMC  eGetEncoding(Column* pCol, i8& i_PriorColumn_, i8& i_Offset_);
                      
    void vHashColumn(Datum* pSrcDatum, Column* pCol);
  
    void vMetaCode(Column* pCol, eMC eMetaCode);
    void vMetaPut(Datum* pDatum, Column* pCol);

    void vRecode_Large(void);
    void vRecode_Short(void);
    void vRecodeVch(Column* pCol);
    void vRecodeVchShoco(Column* pCol);
    bool bRecodeVchShocoDupe(Column* pCol);
    bool bRecodeVchShocoFold(Column* pCol);
    bool bRecodeVchShocoSplit(Column* pCol);
    void vRecodeVchShocoTail(Column* pCol);

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
    void vXchkVchRecode(Column* pCol);
    
  public:
    static void vAnalyse(RowNub* pNub);
  
    //! alka
    static HeadLine* aCompactRow(RowBuffer* pRowBuffer, i8 i_Row);
  
    static void vHeadDump(HeadLine* aHeadLine, ColumnVec* pColVec);
    static void vHeadDump(HeadLine* aHeadLine, ColumnVec* pColVec, Column* pCol);
  
    static void vInflate(Row* pRow, HeadLine* aHeadLine);
  
  private:
  
    static i8   nDataBytes(eMC eMetaCode);

    static void vDecode(Datum* pDatum, RowNub* pNub, i8 i_Row, Column* pCol,
                        i8& i_LastColumn, i8& i_LastOffset);
    static void vDecode(Datum* pDatum, HeadLine* aHeadLine,
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
    static void vDecodeIvl(Datum* pDatum, HeadLine* aHeadLine,
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
    static void vDecodeVchShocoFit(Datum* pDatum, HeadLine* aHeadLine,
                                   Column* pCol, i8 i_Offset);
    static void vDecodeVchShocoSplit(Datum* pDatum, HeadLine* aHeadLine,
                                     Column* pCol, i8 i_Offset);
    static void vDecodeVchShocoTail(Datum* pDatum, HeadLine* aHeadLine,
                                     Column* pCol, i8 i_Offset);
    static void vDecodeVchSmall(Datum* pDatum, HeadLine* aHeadLine,
                                     Column* pCol, i8 i_Offset);
    static void vDteIntoEdt(Datum* pDatum, i8& iEra_);
    static void vDteFromEdt(Datum* pDatum, i8 iEra);
    static void vDteFromEdt(Word* aWord, i8 iEra);
                                     
    static i8 iFindTail(RowNub* pNub, TailLine* aFindLine);
  
    static i8 nFreeSpaceHead(HeadLine* aHeadLine, ColumnVec* pColVec);
  
    static w8   wGetLogical(HeadLine* aHeadLine, i8 i_Offset, i8 nBytes);
    static void vPutLogical(HeadLine* aHeadLine, i8 i_Offset, i8 nBytes, w8 wBytes);

    static i8 nShocoTailSize(RowNub* pNub, HeadLine* aHeadLine);

  private:
    Footer* aFooter(void);


    
  public:
  
    /*----------------------------------------------------------------------------*/
    /* decode boolean from given head line                                        */
    /*----------------------------------------------------------------------------*/
    static inline void vDecodeBol(Datum* pDatum, HeadLine* aHeadLine,Column* pCol)
    {
      pDatum->m_eDatum = DM_BOL;
      
      //eMC eEncoding = eGetEncoding(aHeadLine,pCol,pColVec,i_LastColumn,i_Offset);
      
      i8 i_Column = pCol->iColumnNo();
      eMC eEncoding = aHeadLine->eMetaCode(i_Column);
      
      if (MC_FALSE == eEncoding)
        pDatum->m_iInt = 0;
      else if (MC_TRUE == eEncoding)
        pDatum->m_iInt = 1;
      else
        pDatum->m_iInt = C_DATUM_NULL;
        
     /*
      switch (eEncoding) {
        
        case MC_NULL:  pDatum->m_iInt = C_DATUM_NULL; break;
        case MC_FALSE: pDatum->m_iInt = (i8)0;        break;
        case MC_TRUE:  pDatum->m_iInt = (i8)1;        break;
        
        default:
        NEVER_REACH;
      }
      */
    }

    /*----------------------------------------------------------------------------*/
    /* get the encoding (and offset) of a column datum                            */
    /*----------------------------------------------------------------------------*/
    static inline eMC eGetEncoding(HeadLine* aHeadLine, Column* pCol,
                                   ColumnVec* pColVec,
                                   i8& i_PriorColumn_, i8& i_Offset_)
    {
      i8 i_ForCol = pCol->iColumnNo();
      
      if (i_ForCol < i_PriorColumn_) {
        i_PriorColumn_ = 0;
        i_Offset_ = iBaseOffset(pColVec);
      }
      
      for (i8 i_Col = i_PriorColumn_; i_Col < i_ForCol; i_Col++) {
        
        eMC eEncoding = aHeadLine->eMetaCode(i_Col);
        
        i_Offset_ += nDataBytes(eEncoding);
        
        if (MC_STR_FIT_1L == eEncoding) {
          i_Offset_ += wGetLogical(aHeadLine,(i_Offset_ - 1),C_1B);
        }
      }
      
      eMC eEncoding  = aHeadLine->eMetaCode(i_ForCol);
      i_PriorColumn_ = i_ForCol;
      
      return eEncoding;
    }
  
    /*----------------------------------------------------------------------------*/
    /* calc size of the meta tape needed for given column vector                  */
    /*----------------------------------------------------------------------------*/
    static inline i8 nMetaData(ColumnVec* pColVec)
    {
      i8 nColumns = pColVec->nItems();
    
      i8 nBits = (i8)C_BITS_PER_BYTE + (nColumns * (i8)C_METABITS);
    
      i8 nMetaBytes = nBits / (i8)C_BITS_PER_BYTE
                        + (nBits % (i8)C_BITS_PER_BYTE ? 1 : 0);
    
      return nMetaBytes;
    }
  
    /*----------------------------------------------------------------------------*/
    /* calc size of the head line for column vector                               */
    /*----------------------------------------------------------------------------*/
    static inline i8 nHeadBytes(ColumnVec* pColVec)
    {
      // experiment 1
      i8 nBytes = sizeof(CacheLine) + (1 * C_BYTES_PER_WORD);
      i8 nHeadBytes = MemLib::nQuantaRequired(nBytes, sizeof(Word));
      
      // experiment 2
      //i8 nBytes = pColVec->nItems() * 4;
      //i8 nHeadBytes = MemLib::nQuantaRequired(nBytes, sizeof(Word));
      
      // experiment 3
      //i8 nBytes = pColVec->nItems() * 5;
      //i8 nHeadBytes = MemLib::nQuantaRequired(nBytes, sizeof(Word));
      
      // experiment 4
      //i8 nBytes = pColVec->nItems() * 6;
      //i8 nHeadBytes = MemLib::nQuantaRequired(nBytes, sizeof(Word));
      
      // seth
      //i8 nBytes = pColVec->nItems() * 10;
      //i8 nHeadBytes = MemLib::nQuantaRequired(nBytes, sizeof(Word));
      
      return nHeadBytes * (i8)C_BYTES_PER_WORD;
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
