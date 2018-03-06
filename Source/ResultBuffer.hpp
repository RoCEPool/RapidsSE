/*============================================================================*/
/* ResultBuffer.hpp -- Result Buffer                                          */
/*============================================================================*/
#pragma once

namespace DBobjects
{
  class ResultBuffer : public Object
  {
  protected:
    bool   m_bIsLast;     // is this the last buffer in its sequence?jj
    i8     m_iPartition;  // partitionn this result buffer came from
    i8     m_nResultRows; // number of ResultRows in this buffer
    Table* m_pTable;      // table which describes the rows

    WspFxd<C_SIZE_LARGE_OBJECT - C_1KB*4> m_xWsp;

    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    ResultBuffer(Table* pTable, i8 iPartition);
   ~ResultBuffer(void);
 
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  public:
    bool        bAppendResult(Row* aRow, i8 nRowSize);
    bool        bAppendRow(RowWrk xRowWrk);
    ResultRow*  pFirstResult(void);
    void        vIsLast(bool bIsLast);
    bool        bIsLast(void);
    ResultRow*  pNextResult(ResultRow* pCurrentItem);
    i8          nPartitionNo(void);
    i8          nResultRows(void);
    void        vShow(void);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  private:
    ResultRow*  pAllotResult(i8 nBytes);
    i8          nFreeBytes(void);
  
    Footer* aFooter(void);
  };
  
  ResultBuffer* _pResultBuffer(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
