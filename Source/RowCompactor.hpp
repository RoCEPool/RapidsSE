/*============================================================================*/
/* RowCompactor.hpp -- Row Compactor Mark IV                                  */
/*============================================================================*/
#pragma once

namespace Rows
{
  // numer of rows to sample before creating tails set
  static const i8 C_SAMPLE_ROWS = 100;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class RowCompactor : public Object,
                       private RowCoder
  {
  public:
    RowNub*    m_pCrwNub;      // nub of crw buffer
    ColumnVec* m_pColVec;      // column vector
    i8         m_nHeadSize;    // size of head line for this colvec/table
    i8         m_nMetaSize;    // size of meta data for this colvec/table
    i8         m_i_BaseOffset; // base offset for this colvec/table
    
    // - - - - - - - - - - - -
    
    IrwBuffer* m_pRowSamples;  // the set of sample rows
    bool       m_bCollecting;  // has sampel set been encoded?
    
    Encoding*  m_pEncoding;    // encoding of row
    
    HeadLine*  m_aHeadLine;    // head of crw
    OverLine*  m_aOverLine;    // over of crw

    // - - - - - - - - - - - -
    i8 m_nTotalBuffers;        // total number of crw buffers created
    i8 m_nTotalCrws;           // total number of rows compacted
    FreqDist m_oFreqDist;      // free space analysis

    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    RowCompactor(void);
    ~RowCompactor(void) noexcept(false);
    
    void vCompactBegin(CrwBuffer* pCrwBuffer);
    void vCompactEnd(void);
    bool bCompactRow(Row* pInflatedRow);
  
  private:

    // - - - - - - - - - - - - - - - - - - - - -
    // Sampling (structural order)
  
    bool bCollectSample(Row* pInflatedRow);
  
    void vCompactSamples(void);
    void vAnalyzeSamples(void);
    void vAnalyzeVch(Column* pCol, Row* pOuterRow, Row* pInnerRow);

    // - - - - - - - - - - - - - - - - - - - - -
    // Compact Row Packing (structural order)
  
    bool bCompactInflated(Row* pInflatedRow);

    void vPackCrw(Encoding* pEncoding);
  
    void vPackColVal(_ColEnc& xColEnc);
  
    void vPutMetaCode(_ColEnc& xColEnc);
    void vPutMeta_split(_ColEnc& xColEnc);
    void vPutMeta_usual(_ColEnc& xColEnc);
  
    void vPutDataCode(_ColEnc& xColEnc);
    void vPutData_Bytes(_ColEnc& xColEnc);
    void vPutData_BSTR(_ColEnc& xColEnc);
    void vPutData_SHOCO(_ColEnc& xColEnc);
    void vPutData_STR(_ColEnc& xColEnc);
    void vPutData_TAIL(_ColEnc& xColEnc);
  
    // - - - - - - - - - - - - - - - - - - - - -
    // Support Functions (alpha order)

    void vAllotCrw(void);
    void vDumpTails(void);
    void vEncodeTails(Row* pInflatedRow);
    i8   iFindTail(TailLine* aFindLine);
  
  private:
    Footer* aFooter(void);
  };

  RowCompactor* _pRowCoder(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
