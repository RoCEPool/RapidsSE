/*============================================================================*/
/* RowCompactor.hpp -- Row Coder Mark IV                                          */
/*============================================================================*/
#pragma once

namespace Data
{
  static const i8 C_METABITS = 8;
  
  static const i8 _1B_C   = 1;  // <C>  1B no. of compressed bytes
  static const i8 _1B_L   = 1;  // <L>  1B string length in bytes
  static const i8 _1B_M   = 1;  // <M>  1B overflowed metacode
  
  static const i8 _2B_FT  = 2;  // |FT| absolute from-tail i_Line
  static const i8 _2B_IT  = 2;  // [IT] relative into-tail i_Line
  static const i8 _2B_OT  = 2;  // [OT] relative over-tail i_Line
  
  static const i8 _3B_DSR = 3; // [DSR] relative distance to shoco

  static const i8 C_OFFSET_UNITS = 0;
  static const i8 C_OFFSET_COMPRESSED = C_OFFSET_UNITS + _1B_U;
  
  static const i8 C_HEAD_MIN_BYTES = 24;

  /*----------------------------------------------------------------------------*/
  
  class RowCompactor : public Object,
                       private RowCoder
  {
  public:
  
    bool       m_bEncoding;    // are we encoding (or copying) ?
    ColumnVec* m_pColVec;
    i8         m_nMetaData;    // size of meta data for this colvec/table
    i8         m_i_BaseOffset; // base offset for this colvec/table
  
    RowNub*    m_pIntoNub;
    i8         m_i_IntoHead;
    HeadLine*  m_aIntoHead;
    i8         m_nHeadBytes;   // size of head line for this colvec/table
    //---
    i8         m_i_IntoOver;
    OverLine*  m_aIntoOver;
    i8         m_nOverBytes;
    bool       m_bCopyOver;    // is there a overflow portion to be copied?
    //---
    i8         m_i_IntoOffset; // encoding offset
    
    // + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + +
    
    IrwBuffer* m_pRowSamples; // the set of sample rows
    
    RowCompactor*  m_pRowEncoder;

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
  
    RowCompactor(void);
    ~RowCompactor(void) noexcept(false);
    
    void vCompactBegin(CrwBuffer* pCrwBuffer);
    void vCompactEnd(void);
    bool bCompactRow(Row* pRow);


  public:
    void vReport(RowCompactor* pRowCoder_Encode);
  
  private:
    void vAnalyseFree(void);
    
    void vCompact(RowEncoding* pRowEncoding);
    
    void vExtendIntoHead(i8 nBytes, Column* pCol);

    void vHeadCode(eMC eMetaCode, Dtm wBytes, i8 nBytes, Column* pCol);

    void vMetaCode(Column* pCol, eMC eMetaCode);
    void vMetaPut(Datum* pDatum, Column* pCol);

    byte* ayShoco(HeadLine* aHeadLine, ColumnVec* pColVec, Column* pCol);
    
    void vUsageIncEncoded(Column* pCol, i8 nBytes, eMC eMetaCode = MC_NULL);
    void vUsageIncEncoded(Column* pCol, Datum* pDatum, eMC eMetaCode = MC_NULL);
    void vUsageIncRecoded(Column* pCol, i8 nBytes, eMC eMetaCode = MC_NULL);
    void vUsageIncOops(Column* pCol, i8 nBytes, i8 nMiss, eMC eMetaCode = MC_NULL);
    void vUsageReset(void);
    
  public:
    static void vAnalyse(RowNub* pNub);
  
    //! alka
    static HeadLine* aCompactRow(CrwBuffer* pCrwBuffer, i8 i_Row);
    
    static void vDteFromEdt(Datum* pDatum, i8 iEra);
    static void vDteFromEdt(Word* aWord, i8 iEra);
    
    static void vDumpHead(Stz* szTitle,
                          HeadLine* aHeadLine,
                          ColumnVec* pColVec);
    
  
  private:
  
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
    static void vDecodeVchShocoDup(Datum* pDatum, HeadLine* aHeadLine,
                                   Column* pCol, i8 i_Offset);
    static void vDecodeVchShocoFit(Datum* pDatum, HeadLine* aHeadLine,
                                   Column* pCol, i8 i_Offset);
    static void vDecodeVchShocoSplit(Datum* pDatum, HeadLine* aHeadLine,
                                     Column* pCol, i8 i_Offset);
    static void vDecodeVchShocoTail(Datum* pDatum, HeadLine* aHeadLine,
                                     Column* pCol, i8 i_Offset);
    static void vDecodeVchStringTail(Datum* pDatum, HeadLine* aHeadLine,
                                    Column* pCol, i8 i_Offset);
    static void vDecodeVchSmall(Datum* pDatum, HeadLine* aHeadLine,
                                     Column* pCol, i8 i_Offset);
                                     
    static i8 iFindTail(RowNub* pNub, TailLine* aFindLine);
  
    static i8 nFreeSpaceHead(HeadLine* aHeadLine, ColumnVec* pColVec);
  
    static byte* ayAdrLogical(HeadLine* aHeadLine, i8 i_Offset);
    static w8    wGetLogical(HeadLine* aHeadLine, i8 i_Offset, i8 nBytes);
    static void  vPutLogical(HeadLine* aHeadLine, i8 i_Offset, i8 nBytes, w8 wBytes);

    static OverLine* aGetOver(HeadLine* aHeadLine);
    static OverLine* aGetOverRel(HeadLine* aHeadLine, i8 i_Over_rel);
 
    static void vMetaCode(HeadLine* aHeadLine, Column* pCol, eMC eMetaCode);

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
        
        if (MC_OVERFLOW_2T == eEncoding) {
          i_Offset_ = aHeadLine->nBytes() + _1B_U;
          eEncoding = (eMC)wGetLogical(aHeadLine,i_Offset_++,C_1B);
          i_Offset_ += nDataBytes(eEncoding);
        }
        else {
          i_Offset_ += nDataBytes(eEncoding);
        }
        
        if (MC_STR_FIT_1L == eEncoding) {
          i_Offset_ += wGetLogical(aHeadLine,(i_Offset_ - 1),C_1B);
        }
        else if (MC_SHOCO_FIT_1C == eEncoding) {
          i_Offset_ += wGetLogical(aHeadLine,(i_Offset_ - 1),C_1B);
        }
      }
      
      eMC eEncoding = aHeadLine->eMetaCode(i_ForCol);
      if (MC_OVERFLOW_2T == eEncoding) {
        i_Offset_ = aHeadLine->nBytes() + _1B_U + _1B_M;
        OverLine* aOverLine = aGetOver(aHeadLine);
        eEncoding = (eMC)aOverLine->w8_1B(1);
      }
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
      i8 nHeadWords = MemLib::nQuantaRequired(nBytes, sizeof(Word));
      
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
      
      //nBytes     = 24;
      //nHeadWords = MemLib::nQuantaRequired(nBytes, sizeof(Word));

      //i8 nBytesHead = nHeadWords * (i8)C_BYTES_PER_WORD;
      i8 nBytesHead = 256;
      RASSERT(nBytesHead >= C_HEAD_MIN_BYTES);
      
      return nBytesHead;
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

  RowCompactor* _pRowCoder(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
