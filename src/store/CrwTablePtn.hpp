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
    Bucket*   m_pBucket;    // bucket this partition belongs to
#ifdef WITH_PTNID
    PtnId    m_tPtnNo;        // partition number (in grid)
#else
    tPN    m_tPtnNo;        // partition number (in grid)
#endif

    CrwBuffer* m_pRootBuffer;    // the data root buffer
    CrwBuffer* m_pCurrentBuffer; // the data current buffer for insert

    // Note that the IndexTreeVec gets moved upto TablePtn
    IndexTreeVec* m_pIndexTreeVec;  // vector of index trees
      
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);

#ifdef WITH_PTNID
    CrwTablePtn(Table *pTable, PtnId tPtnNo, Bucket *pBucket);
#else
    CrwTablePtn(Table* pTable, tPN tPtnNo, Bucket* pBucket);
#endif
   ~CrwTablePtn(void) noexcept(false);
  
    void vAddIndex(IndexTree* pIndexTree);
    void vAppendBuffer(CrwBuffer* pCrwBuffer);
    i8   nBuffers(void);
    void vDump(void);
    IndexTree* pIndexTree(i8 iIndexNo);
    IndexTreeVec* pIndexTreeVec(void);
    void vInsertIndexEntries(CrwBuffer* pCrwBuffer);

#ifdef WITH_PTNID

    PtnId tPtnNo(void);

#else
    tPN  tPtnNo(void);
#endif
    bool bPopulateIndex(i8 iIndexNo);
    bool bPopulateIndexTree(IndexTree* pIndexTree);
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
