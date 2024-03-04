/*============================================================================*/
/* TablePtn.hpp -- Table Partition                                            */
/*============================================================================*/
#pragma once

namespace STORE { class CrwTablePtn;}

namespace STORE
{
  class TablePtn : public Object
  {
  private:
    Lock   m_okLock;  // lock on the partition
    eCN    m_eKind;   // what kind of table partition

    IndexTreeVec* m_pIndexTreeVec;  // vector of index trees

    Table* m_pTable;  // table this partition belongs to


    LglPtn m_tLglPtn;  // logical partition (in grid)
    PhyPtn m_tPhyPtn;  // physical partition (in grid)

    CrwTablePtn* m_pCrwTablePtn;

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    TablePtn(Table* pTable, LglPtn tLglPtn, PhyPtn tPhyPtn);
    ~TablePtn(void) noexcept(false);

    void vCleanup(void);
    void vDropRead(void);
    void vDropWrite();
    bool bGrabRead(void);
    bool bGrabWrite(void);
    bool bIsLocked(void);
    eCN  eKind(void);

    LglPtn  tLglPtn(void);

    Table* pTable(void);

    void vAddIndex(IndexTree* pIndexTree);
    void vAppendBuffer(DataBuffer* pDataBuffer, i8& nRowsInserted);
    i8   nBuffers(void);
    void vDump(void);
    BufferId oExtractBufferId(void* pItemAddr);
    IndexTree* pIndexTree(i8 iIndexNo);
    IndexTreeVec* pIndexTreeVec(void);
    void vInsertIndexEntries(CrwBuffer* pCrwBuffer);
    bool bPopulateIndex(i8 iIndexNo);
    void vRemoveIndex(i8 iIndexNo);
    BufferId oRootBufferId(void);
    void vAnalyze(i8& nBuffers, i8& nRows, i8& jSumRowBytes);
    void vTruncate(void);
    void vTruncateIndex(Index* pIndex);
    bool bVerifyTable(void);
    void vVisualizeTable(void);

  private:
    Footer* aFooter(void);
  };

  TablePtn* _pTablePtn(void* aAddr);


  typedef VectorNL_t<CN_TABLEPTNVEC,STORE::TablePtn,C_MAX_PTNSPERNODE>TablePtnVec;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
