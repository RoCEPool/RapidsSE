/*============================================================================*/
/* WorkingBuffer.hpp -- Working Buffer                                        */
/*============================================================================*/
#pragma once

namespace DBobjects
{
  class WorkingBuffer : public Object
  {
  protected:
    i8     m_nWorkingRows; // number of WorkingRows in this buffer
    bool   m_bIsLast;      // is this the last buffer in its sequence?jj
    i8     m_iPartition;   // partitionn this result buffer came from
    Table* m_pTable;       // table which describes the rows

    WspFxd<C_SIZE_LARGE_OBJECT - C_1KB*4> m_xWsp;

    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    WorkingBuffer(Table* pTable, i8 iPartition);
   ~WorkingBuffer(void) noexcept(false);
 
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  public:
    bool        bAppendRow(Row* aRow, i8 nRowSize);
    bool        bAppendRow(RowWrk xRowWrk);
    WorkingRow* pAllotRow(i8 nBytes);
    WorkingRow* pAllotSpace(i8 nBytes);
    char*       aDataArea(void);
    WorkingRow* pFirstRow(void);
    void        vIsLast(bool bIsLast);
    bool        bIsLast(void);
    WorkingRow* pNextRow(WorkingRow* pCurrentItem);
    i8          nPartitionNo(void);
    void        vRestore(void);
    void        vShow(void);
    i8          nWorkingRows(void);
    void        vShow(bool bCompressed);
    i8          nUsedBytes(void);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  private:
    i8          nFreeBytes(void);
    void        vItemCountRestore(void);
  
    Footer* aFooter(void);
  };
  
  WorkingBuffer* _pWorkingBuffer(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
