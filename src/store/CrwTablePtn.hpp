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
    LglPtn    m_tLglPtn;        // partition number (in grid)
    i8     m_iSynthKey;     // Synthetic key for the Pds

    Pds_t<PdsItem_BufferId>* m_pPdsOfBufferIds;

    // Note that the IndexTreeVec gets moved upto TablePtn
    //IndexTreeVec* m_pIndexTreeVec;  // vector of index trees

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    CrwTablePtn(Table *pTable, LglPtn tLglPtn);
    ~CrwTablePtn(void) noexcept(false);

    void vAppendBuffer(CrwBuffer* pCrwBuffer);
    i8   nBuffers(void);
    void vCleanup(void);
    void vDropAllPagingLocks(void);
    void vDump(void);
    BufferId oExtractBufferId(void* pItemAddr);
    CrwBuffer* pGetBuffer(SkipListBlock* pSLBlock);
    void* pPds(void);




    bool bPopulateIndexTree(IndexTree* pIndexTree);

    CrwBuffer* pRootCrwBuffer(void);
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
