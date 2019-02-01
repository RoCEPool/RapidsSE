/*============================================================================*/
/* FetchBuffer.hpp -- Fetch Buffer                                            */
/*============================================================================*/
#pragma once

namespace Jobs
{
  class FetchBuffer : public Object
  {
    static const i8 C_WSP_FETCHBUFFER = C_SIZE_LARGE_OBJECT - C_4KB;
  
  protected:
    i8     m_nFetchedRows; // number of Fetched rows in this buffer
    bool   m_bIsLast;      // is this the last buffer in its sequence?
    i8     m_iPartition;   // partition this result buffer came from
    Table* m_pTable;       // table which describes the rows

    Wsp_t<C_WSP_FETCHBUFFER> m_xWsp;

    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    FetchBuffer(Table* pTable, i8 iPartition);
   ~FetchBuffer(void) noexcept(false);
 
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  public:
    bool  bAppendRow(Row* pRow);
    Row** aaAllotRowPtr(i8 nBytes);
    char* aDataArea(void);
    i8    nFetchedRows(void);
    Row** aaFirstRowPtr(void);
    void  vIsLast(bool bIsLast);
    bool  bIsLast(void);
    Row** aaNextRowPtr(Row** ppRow);
    i8    nPartitionNo(void);
    void  vRestore(void);
    void  vShow(bool bCompressed);
    i8    nUsedBytes(void);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  private:
    i8    nFreeBytes(void);
    void  vItemCountRestore(void);
  
    Footer* aFooter(void);
  };
  
  FetchBuffer* _pFetchBuffer(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
