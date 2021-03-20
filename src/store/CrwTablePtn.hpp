/*============================================================================*/
/* CrwTablePtn.hpp -- Crw Table Partition                                     */
/*============================================================================*/
#pragma once

namespace STORE
{
  class CrwTablePtn : public Object
  {
  private:
    Table* m_pTable;        // table this partition belongs to
    TablePtn* m_pTablePtn;  // table this partition belongs to
    tPN    m_tPtnNo;        // partition number (in grid)

    CrwBuffer* m_pRootBuffer;    // the data root buffer
    CrwBuffer* m_pCurrentBuffer; // the data current buffer for insert
    
    IndexTreeVec* m_pIndexTreeVec;  // vector of index trees
      
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    CrwTablePtn(Table* pTable, tPN tPtnNo, TablePtn* pTablePtn);
   ~CrwTablePtn(void) noexcept(false);
  
    void vAddIndex(IndexTree* pIndexTree);
    void vAppendBuffer(CrwBuffer* pCrwBuffer);
    void vAppendBuffer(RowBuffer* pRowBuffer);
    i8   nBuffers(void);
    void vDump(void);
    IndexTree* pIndexTree(i8 iIndexNo);
    IndexTreeVec* pIndexTreeVec(void);
    void vInsertIndexEntries(CrwBuffer* pCrwBuffer);
    tPN  tPtnNo(void);
    bool bPopulateIndex(i8 iIndexNo);
    void vRemoveIndex(i8 iIndexNo);
    CrwBuffer* pRootCrwBuffer(void);
    Table* pTable(void);
    void vTruncate(void);
    void vTruncateIndex(Index* pIndex);
    bool bVerifyTable(void);
    void vVisualizeTable(void);
    
  private:
    Footer* aFooter(void);
  };
  
  CrwTablePtn* _pCrwTablePtn(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
