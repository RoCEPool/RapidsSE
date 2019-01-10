/*============================================================================*/
/* FetchBuffer.hpp -- Fetch Buffer                                            */
/*============================================================================*/
#pragma once

namespace DBobjects
{
  class FetchBuffer : public Object
  {
    static const i8 C_FETCHBUFFER_WSP = C_SIZE_LARGE_OBJECT - C_4KB;
  
  protected:
    i8     m_nFetchedRows; // number of Fetched rows in this buffer
    bool   m_bIsLast;      // is this the last buffer in its sequence?
    i8     m_iPartition;   // partition this result buffer came from
    Table* m_pTable;       // table which describes the rows

    WspFxd<C_FETCHBUFFER_WSP> m_xWsp;

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
    bool         bAppendRow(Row* pRow);
    Row**        pAllotRowPtr(i8 nBytes);
    char*        aDataArea(void);
    i8           nFetchedRows(void);
    Row**        pFirstRowPtr(void);
    void         vIsLast(bool bIsLast);
    bool         bIsLast(void);
    Row**        pNextRowPtr(Row** ppRow);
    i8           nPartitionNo(void);
    void         vRestore(void);
    void         vShow(bool bCompressed);
    i8           nUsedBytes(void);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  private:
    i8          nFreeBytes(void);
    void        vItemCountRestore(void);
  
    Footer* aFooter(void);
  };
  
  FetchBuffer* _pFetchBuffer(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
