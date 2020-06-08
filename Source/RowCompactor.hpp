/*============================================================================*/
/* RowCompactor.hpp -- Row Compactor Mark V                                   */
/*============================================================================*/
#pragma once

namespace CompactBuffers
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
    
    IrwBuffer* m_pIrwSamples;  // the set of sample rows
    bool       m_bCollecting;  // has sample set been encoded?
  
    RowSlab*   m_pIrwSampleSlab;
  
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
    bool bCompactRsb(Rsb* aRsb);
    bool bCompactSrw(RowSingle* pSrw);
  
  private:

    // - - - - - - - - - - - - - - - - - - - - -
    // Row Packing
  
    bool bAddIrwAsCrw(Row* pIrw);
    bool bAddRsbAsCrw(Rsb* aRsb);
    bool bAddSrwAsCrw(RowSingle* pSrw);
    bool bAllotCrw(void);
    void vEncodeHeadForIrw(Row* pIrw);
    void vEncodeHeadForSrw(RowSingle* pSrw);
    void vEncodeTailsForIrw(Row* pIrw);
    void vEncodeTailsForSrw(RowSingle* pSrw);
    void vEncodeSplit(void);

    // - - - - - - - - - - - - - - - - - - - - -
    // Row Sampling (structural order)
  
    bool bCollectSample(RowSingle* pSrw);
  
    void vPackSamples(void);
    void vAnalyzeSamples(void);
    void vCreateTail(Column* pCol,
                     Row* pOuterIrwSample,
                     Row* pInnerIrwSample);
  private:
    Footer* aFooter(void);
  };

  RowCompactor* _pRowCompactor(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
