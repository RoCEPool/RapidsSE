/*============================================================================*/
/* ResultBuffer.hpp -- Result Buffer                                          */
/*============================================================================*/
#pragma once

namespace DBobjects
{
  class ResultBuffer //: public ItemBuffer
  {
  protected:
    i8    m_nItems;    // number of items in this buffer
    WspFxd<C_SIZE_LARGE_OBJECT - C_1KB*4> m_xWsp;
    
    bool   m_bIsLast; // is this the last buffer in its sequence?jj
    Table* m_pTable;  // table which describes the rows
    i8     m_iPartition;  // partitionn this result buffer came from

    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    ResultBuffer(Table* pTable, i8 iPartition);
   ~ResultBuffer(void);
 
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    ResultRow*  pAllotResult(i8 nBytes);
    char*  aDataArea(void);
    ResultRow*  pFirstResult(void);
    i8     nFreeBytes(void);
    i8     nItems(void);
    ResultRow*  pNextResult(ResultRow* pCurrentItem);
    i8     nUsedBytes(void);
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    bool bAppendRow(Row* aRow, i8 nRowSize);
  
    void vIsLast(bool bIsLast);
    bool bIsLast(void);
  
    ResultRow* pFirstRow(void);
    ResultRow* pNextRow(ResultRow* pCurrentRow);
    i8         nPartitionNo(void);
  
  
    void vShow(void);
  
    Footer* aFooter(void);
  };
  
  ResultBuffer* _pResultBuffer(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
