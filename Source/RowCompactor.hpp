/*============================================================================*/
/* RowCompactor.hpp -- Row Compactor Mark IV                                  */
/*============================================================================*/
#pragma once

namespace Data
{
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
    i8         m_i_Crw;        // index of comapct row in crw nub
    HeadLine*  m_aCrw;         // compact row being formed
    i8         m_i_DataOffset; // offset where next data bytes go
    
    // - - - - - - - - - - - -
    IrwBuffer* m_pRowSamples;  // the set of sample rows
    
    // - - - - - - - - - - - -
    i8 m_nTotalBuffers;        // total number of crw buffers created
    i8 m_nTotalCrws;           // total number of rows compacted

//  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
//    i8 m_nHeadFreeBytes;
//    i8 m_nHeadFreeBytes_0_7;
//    i8 m_nHeadFreeBytes_8_15;
//    i8 m_nHeadFreeBytes_16_23;
//    i8 m_nHeadFreeBytes_24_31;
//    i8 m_nHeadFreeBytes_32_39;
//    i8 m_nHeadFreeBytes_40_47;
//    i8 m_nHeadFreeBytes_48_55;
//    i8 m_nHeadFreeBytes_56_63;
//    i8 m_nHeadFreeBytes_64_;

    FreqDist m_oFreqDist;
//
//    //---
//
//    m_xFreq.vInit(7,15,23,31,39,47,55,63,-1);
//
//
//    m_xFreq.vAdd(nBytes);
//
//    m_xFreq.nBuckets();
//    m_xFreq.iBucket(i_Bucket);
//    m_xFreq.iDist(i_Bucket)
//    m_xFreq.nSamples();
//    m_xFreq.nSum();


    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    RowCompactor(void);
    ~RowCompactor(void) noexcept(false);
    
    void vCompactBegin(CrwBuffer* pCrwBuffer);
    void vCompactEnd(void);
    bool bCompactRow(Row* pRow);
  
  private:
    void vAnalyseFree(void);
    void vCompactEncoded(RowEncoding* pRowEncoding);
    void vDtmCode(eMC eMetaCode, Dtm wDataCode, i8 nDataSize, Column* pCol);
    
  private:
    Footer* aFooter(void);
  };

  RowCompactor* _pRowCoder(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
