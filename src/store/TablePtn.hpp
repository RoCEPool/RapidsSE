/*============================================================================*/
/* TablePtn.hpp -- Table Partition                                            */
/*============================================================================*/
#pragma once

namespace STORE
{
  class TablePtn : public Object
  {
  private:
    Lock   m_okLock;       // lock on the partition
    Table* m_pTable;       // table this partition belongs to
    tPN    m_tPtnNo;       // partition number (in grid)

    CrwBuffer* m_pRootBuffer;    // the data root buffer
    CrwBuffer* m_pCurrentBuffer; // the data current buffer for insert
    
    IndexTreeVec* m_pIndexTreeVec;  // vector of index trees
  
    ReadApi* m_pReadApi;            // read consistency api
    
    // must be last
    Footer m_xFooter;
    
  public:
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
  
    void* operator new(size_t);
    void  operator delete(void*);
    
    TablePtn(Table* pTable, tPN tPtnNo);
    ~TablePtn(void) noexcept(false);
  
    void vAddIndex(IndexTree* pIndexTree);
    void vAppendBuffer(CrwBuffer* pCrwBuffer);
    void vAppendBuffer(RowBuffer* pRowBuffer);
    i8   nBuffers(void);
    void vDeleteCrw(IndexTree* pIndexTree, EntBuffer* pEntBuffer, Ent* aEnt);
    void vDropRead(void);
    void vDropWrite();
    void vDump(void);
    bool bGrabRead(void);
    bool bGrabWrite(void);
    IndexTree* pIndexTree(i8 iIndexNo);
    IndexTreeVec* pIndexTreeVec(void);
    void vInsertIndexEntries(CrwBuffer* pCrwBuffer);
    bool bIsLocked(void);
    CrwBuffer* pNewCurrentCrwBuffer(void);
    void vParentJobId(i8 iJobId);
    i8   iParentJobId(void);
    tPN  tPtnNo(void);
    bool bPopulateIndex(i8 iIndexNo);
    bool bPurge(i8 iOlderThanSeconds);
    ReadApi* pReadApi(void);
    void vRemoveIndex(i8 iIndexNo);
    CrwBuffer* pRootCrwBuffer(void);
    CrwBuffer* pSnapshot(CrwBuffer* pCrwBufferSrc, t8 tPerspective);
    Table* pTable(void);
    void vTruncate(void);
    void vTruncateIndex(Index* pIndex);
//    void vUpsertBuffer(CrwBuffer* pCrwBuffer, t8 tPerspective);
//    void vUpsertRow_insert(Crw* aCrw_nosuch, EntWrk* aEntWrk,
//                           Encoding_v1* pEncoding_upsert, t8 tPerspective);
//    void vUpsertRow_insert(RowBuffer* pRowBuffer, i8 i_Row, EntWrk* aEntWrk,
//                           Encoding_v1* pEncoding_upsert, t8 tPerspective);
//    void vUpsertRow_update(Crw* aCrw_exists, Ent* aEnt_Found,
//                           Encoding_v1* pEncoding_upsert, t8 tPerspective);
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
