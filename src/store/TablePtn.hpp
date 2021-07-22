/*============================================================================*/
/* TablePtn.hpp -- Table Partition                                            */
/*============================================================================*/
#pragma once

namespace STORE { class CrwTablePtn; class RowTablePtn; }

namespace STORE
{
  class TablePtn : public Object
  {
  private:

    /// will change to vector of bucket
    Bucket* m_pBucket;

    Table* m_pTable;  // table this partition belongs to
    tPN    m_tPtnNo;  // partition number (in grid)

  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    TablePtn(Table* pTable, tPN tPtnNo);
   ~TablePtn(void) noexcept(false);
  
    void vDropRead(void);
    void vDropWrite();
    bool bGrabRead(void);
    bool bGrabWrite(void);
    bool bIsLocked(void);
    eCN  eKind(void);
    tPN  tPtnNo(void);
    Table* pTable(void);

    void vAddIndex(IndexTree* pIndexTree);
    void vAppendBuffer(DataBuffer* pDataBuffer);
    i8   nBuffers(void);
    void vDump(void);
    IndexTree* pIndexTree(i8 iIndexNo);
    IndexTreeVec* pIndexTreeVec(void);
    void vInsertIndexEntries(CrwBuffer* pCrwBuffer);
    bool bPopulateIndex(i8 iIndexNo);
    void vRemoveIndex(i8 iIndexNo);
    CrwBuffer* pRootCrwBuffer(void);
    RowBuffer* pRootRowBuffer(void);
    DataBuffer* pRootBuffer(void);
    void vTruncate(void);
    void vTruncateIndex(Index* pIndex);
    bool bVerifyTable(void);
    void vVisualizeTable(void);
    
  private:
    Footer* aFooter(void);
  };
  
  TablePtn* _pTablePtn(void* aAddr);


  typedef VectorNL_t<CN_TABLEPTNVEC,STORE::TablePtn,C_MAX_TABLEPTNS>TablePtnVec;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
