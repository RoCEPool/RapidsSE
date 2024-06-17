/*============================================================================*/
/* CrwTablePtn.hpp -- Crw Table Partition                                     */
/*============================================================================*/
#pragma once

namespace STORE
{
  typedef struct {
    i8 iKey;
    BufferId oBufferId;
  } PdsBufferIdItem;

  class CrwTablePtn : public Object
  {
  private:
    Table* m_pTable;        // table this partition belongs to
    LglPtn    m_tLglPtn;        // partition number (in grid)
    i8     m_iSynthKey;     // Synthetic key for the Pds

    Pds_t<PdsBufferIdItem>* m_pPdsOfBufferIds;
    BufferId m_oRootPdsBufferId;

    // Note that the IndexTreeVec gets moved upto TablePtn
    //IndexTreeVec* m_pIndexTreeVec;  // vector of index trees

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    CrwTablePtn(Table *pTable, LglPtn tLglPtn);
    ~CrwTablePtn(void) noexcept(false);

    void vAnalyze(i8& nBuffers, i8& nRows, i8& jSumRowBytes);
    void vAppendBuffer(CrwBuffer* pCrwBuffer);
    void vAppendBuffer(Pds* pPds);
    i8   nBuffers(void);
    void vCleanup(void);
    void vDropAllPagingLocks(void);
    void vDump(void);
    BufferId oExtractBufferId(void* pItemAddr);
    CrwBuffer* pGetBuffer(SkipListBlock* pSLBlock);

    bool bPopulateIndexTree(IndexTree* pIndexTree);
    bool bPopulateGblIndexTree(IndexTree* pIndexTree, LglPtn tLglPtn);

    BufferId oRootCrwBufferId(void);
    Table* pTable(void);
    void vTruncate(void);

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
