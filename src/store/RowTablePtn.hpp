/*============================================================================*/
/* RowTablePtn.hpp -- Row Table Partition                                     */
/*============================================================================*/
#pragma once

namespace STORE
{
  class RowTablePtn : public Object
  {
  private:
    Table* m_pTable;        // table this partition belongs to
    Bucket*   m_pBucket;    // bucket this partition belongs to
#ifdef WITH_PTNID
    PtnId    m_tPtnNo;        // partition number (in grid)
#else
    tPN    m_tPtnNo;        // partition number (in grid)
#endif

    RowBuffer* m_pRootBuffer;    // the data root buffer
    RowBuffer* m_pCurrentBuffer; // the data current buffer for insert
          
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
#ifdef WITH_PTNID
    RowTablePtn(Table* pTable, PtnId tPtnNo, Bucket* pBucket);
#else
    RowTablePtn(Table* pTable, tPN tPtnNo, Bucket* pBucket);
#endif
   ~RowTablePtn(void) noexcept(false);
  
    void vAppendBuffer(RowBuffer* pRowBuffer);
    i8   nBuffers(void);
    void vDump(void);
#ifdef WITH_PTNID
    PtnId  tPtnNo(void);
#else
    tPN  tPtnNo(void);
#endif
    RowBuffer* pRootRowBuffer(void);
    Table* pTable(void);
    void vTruncate(void);
    bool bVerifyTable(void);
    void vVisualizeTable(void);
    
  private:
    Footer* aFooter(void);
  };
  
  RowTablePtn* _pRowTablePtn(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
