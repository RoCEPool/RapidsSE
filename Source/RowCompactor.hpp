/*============================================================================*/
/* RowCompactor.hpp -- Row Compactor Mark V                                   */
/*============================================================================*/
#pragma once

namespace Buffers
{
  class RowCompactor : public  Object,
                       private RowCoder,
                       private Packing
  {
  public:
    RowNub*    m_pCrwNub;      // nub of crw buffer
    StreamDef* m_pStreamDef;   // stream def
    Encoding*  m_pEncoding;    // encoding of row

    HeadLine*  m_aHeadLine;    // head of crw
    OverLine*  m_aOverLine;    // over of crw

    // - - - - - - - - - - - -
    
    IrwBuffer* m_pRowSamples;  // the set of sample rows
    bool       m_bCollecting;  // has sample set been encoded?
  
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
    
    void vCompactBegin(CrwBuffer* pCrwBuffer, bool bCollectSamples);
    void vCompactEnd(void);
    bool bCompactRow(Row* pInflatedRow);
  
  private:

    // - - - - - - - - - - - - - - - - - - - - -
    // Sampling (structural order)
  
    bool bCollectSample(Row* pInflatedRow);
  
    void vPackSamples(void);
    void vAnalyzeSamples(void);
    void vCreateTail(Column* pCol, Row* pOuterRow, Row* pInnerRow);

    // - - - - - - - - - - - - - - - - - - - - -
    // Compact Row Packing
  
    bool bAddCrw(Row* pInflatedRow);
    bool bAllotCrw(void);
    void vEncodeHead(Row* pInflatedRow);
    void vEncodeTails(Row* pInflatedRow);
    void vSplitOver(void);

  private:
    Footer* aFooter(void);
  };

  RowCompactor* _pRowCoder(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
