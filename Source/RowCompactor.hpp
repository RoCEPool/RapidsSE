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
    
    IrwBuffer* m_pRowSamples;  // the set of sample rows
    Encoding*  m_pEncoding;    // encoding of row
    
    HeadLine*  m_aHeadLine;    // head of crw
    
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
    void vAllotOver(void);
    void vPackColVal(_ColEnc& xColEnc, Column* pCol,i8& i_Offset_);
    void vPackCrw(Encoding* pEncoding);

  private:
    Footer* aFooter(void);
  };

  RowCompactor* _pRowCoder(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
