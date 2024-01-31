/*============================================================================*/
/* RowCompactor.hpp -- Row Compactor Mark V                                   */
/*============================================================================*/
#pragma once

namespace STORE
{
  class RowCompactor : public  Object,
                       private RowCoder,
                       private CrwPacker
  {
  public:
    CrwBuffer* m_pCrwBuffer;   // crw buffer
    RowNub*    m_pCrwNub;      // nub of crw buffer
    StreamDef* m_pStreamDef;   // stream def
  
    Encoding* m_pEncoding;  // encoding of row

    HeadLine*  m_aHeadLine;    // head of crw
    OverLine*  m_aOverLine;    // over of crw

    // - - - - - - - - - - - -

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
    bool bCompactRow(Row* aRow);
    bool bCompactTail(Stz* szTailStr);

  private:

    // - - - - - - - - - - - - - - - - - - - - -
    // Row CrwPacker
  
    bool bAddRowAsCrw(Row* pRow);
    bool bAddRowAsCrw_2(Row* pRow);
    bool bAllotCrw(void);
    bool bCreateTail(Stz* szTailStr);
    void vEncodeDupeTailsForRow(Row* pRow);

    // - - - - - - - - - - - - - - - - - - - - -
    // Row Sampling (structural order)

    void vCreateSampleTail(Column* pCol,
                           Row* pOuterRowSample,
                           Row* pInnerRowSample);
  private:
    Footer* aFooter(void);
  };

  RowCompactor* _pRowCompactor(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
