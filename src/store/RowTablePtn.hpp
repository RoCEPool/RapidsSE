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
    TablePtn* m_pTablePtn;  // table this partition belongs to
    tPN    m_tPtnNo;        // partition number (in grid)

    RowBuffer* m_pRootBuffer;    // the data root buffer
    RowBuffer* m_pCurrentBuffer; // the data current buffer for insert
          
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    RowTablePtn(Table* pTable, tPN tPtnNo, TablePtn* pTablePtn);
   ~RowTablePtn(void) noexcept(false);
  
    void vAppendBuffer(RowBuffer* pRowBuffer);
    i8   nBuffers(void);
    void vDump(void);
    tPN  tPtnNo(void);
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
