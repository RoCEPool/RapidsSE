/*============================================================================*/
/* CrwTablePtn.hpp -- Crw Table Partition                                     */
/*============================================================================*/
#pragma once

namespace STORE
{
  struct xBufferId {
    i8 iKey;
    BufferId oBufferId;
  };

  class CrwTablePtn : public Object
  {
  private:
    Table* m_pTable;        // table this partition belongs to
    LglPtn    m_tLglPtn;        // partition number (in grid)

    // TODO: Arjun - m_pRootBuffer needs to be removed
    CrwBuffer* m_pRootBuffer;    // the data root buffer
    CrwBuffer* m_pCurrentBuffer; // the data current buffer for insert

    Pds_t<xBufferId>* m_pPdsOfBufferIds;

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
    CrwBuffer* pGetBuffer(SkipListBlock* pSLBlock);
    void* pPds(void);




    bool bPopulateIndexTree(IndexTree* pIndexTree);

    CrwBuffer* pRootCrwBuffer(void);
    BufferId oRootCrwBufferId(void);
    Table* pTable(void);
    void vTruncate(void);

    void vVerifyPds(void);
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
