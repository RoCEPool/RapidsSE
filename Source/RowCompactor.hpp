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
  
  private:
    void vAnalyseFree(void);
    
    void vCompact(RowEncoding* pRowEncoding);
    
    void vExtendIntoHead(i8 nBytes, Column* pCol);

    void vHeadCode(eMC eMetaCode, Dtm wBytes, i8 nBytes, Column* pCol);

    void vMetaCode(Column* pCol, eMC eMetaCode);

    byte* ayShoco(HeadLine* aHeadLine, ColumnVec* pColVec, Column* pCol);
    
    void vUsageIncEncoded(Column* pCol, i8 nBytes, eMC eMetaCode = MC_NULL);
    void vUsageIncEncoded(Column* pCol, Datum* pDatum, eMC eMetaCode = MC_NULL);
    void vUsageIncRecoded(Column* pCol, i8 nBytes, eMC eMetaCode = MC_NULL);
    void vUsageIncOops(Column* pCol, i8 nBytes, i8 nMiss, eMC eMetaCode = MC_NULL);
    void vUsageReset(void);
    
    
  public:
    static void vAnalyse(RowNub* pNub);
  
    static HeadLine* aCompactRow(CrwBuffer* pCrwBuffer, i8 i_Row);
    
    static void vDumpHead(Stz* szTitle,
                          HeadLine* aHeadLine,
                          ColumnVec* pColVec);
    static void vDumpHead_IV(Stz* szTitle,
                             HeadLine* aHeadLine,
                             ColumnVec* pColVec);
  
  private:
    static i8 iFindTail(RowNub* pNub, TailLine* aFindLine);
    
    static i8 nHeadBytes_(ColumnVec* pColVec);
    
    static i8 nShocoTailSize(RowNub* pNub, HeadLine* aHeadLine);
    
  private:
    Footer* aFooter(void);
  };

  RowCompactor* _pRowCoder(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
