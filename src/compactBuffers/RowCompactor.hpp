/*============================================================================*/
/* RowCompactor.hpp -- Row Compactor Mark V                                   */
/*============================================================================*/
#pragma once

namespace CompactBuffers
{
  class RowCompactor : public  Object,
                       private RowCoder,
                       private Packing_v2
  {
  public:
    CrwBuffer* m_pCrwBuffer;   // crw buffer
    RowNub*    m_pCrwNub;      // nub of crw buffer
    StreamDef* m_pStreamDef;   // stream def
  
    Encoding_v2* m_pEncoding_v2;  // encoding of row

    HeadLine*  m_aHeadLine;    // head of crw
    OverLine*  m_aOverLine;    // over of crw

    // - - - - - - - - - - - -
    
    IrwBuffer* m_pIrwSamples;  // the set of sample rows
    bool       m_bCollecting;  // has sample set been encoded?
  
    RowSlab*   m_pSampleSlab;
  
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
    bool bCompactTail(Stz* szTailStr);

  private:

    // - - - - - - - - - - - - - - - - - - - - -
    // Row Packing
  
    bool bAddIrwAsCrw(Row* pRow);
    bool bAddRsbAsCrw(Rsb* aRsb);
    bool bAddSrwAsCrw(RowSingle* pSrw);
    bool bAllotCrw(void);
    bool bCreateTail(Stz* szTailStr);
    void vEncodeHeadForIrw(Row* pRow);
    void vEncodeHeadForSrw(RowSingle* pSrw);
    void vEncodeTailsForIrw(Row* pRow);
    void vEncodeTailsForRsb(Rsb* aRsb);
    void vEncodeTailsForSrw(RowSingle* pSrw);

    // - - - - - - - - - - - - - - - - - - - - -
    // Row Sampling (structural order)
  
    bool bCollectSample(RowSingle* pSrw);
  
    void vPackSamples(void);
    void vAnalyzeSamples(void);
    void vCreateSampleTail(Column* pCol,
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
