/*============================================================================*/
/* TablePtn.hpp -- Table Partition                                            */
/*============================================================================*/
#pragma once

namespace Partitioning
{
  class TablePtn : public Object
  {
  private:
    Lock m_okLock;         // lock on the partition
    Table* m_pTable;       // table this partition belongs to
    i8   m_iPartitionId;   // partition number
    bool m_bLowMemLoad;    // was low memory detected during this load

    CrwBuffer* m_pRootCrwBuffer;    // the data root buffer
    CrwBuffer* m_pCurrentCrwBuffer; // the data current buffer for insert
    
    IndexTreeVec* m_pIndexTreeVec;  // vector of index trees
  
    ReadApi* m_pReadApi;            // read consistency api
    
    // must be last
    Footer m_xFooter;
    
  public:
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
    static i8   iPtnNoToNode(i8 iPtnNo);
    static i8   iPtnNoToTablePtn(i8 iPtnNo);
    static i8   iTablePtnToPtnNo(i8 i_TablePtn);
  
    void* operator new(size_t);
    void  operator delete(void*);
    
    TablePtn(Table* pTable);
    ~TablePtn(void) noexcept(false);
  
    void vAddIndex(IndexTree* pIndexTree);
    Crw* aAllotFixedSizeCrw(Rrf& xRowRef);
    void vAppendBuffer(CrwBuffer* pCrwBuffer);
    void vAppendRow(RowCompactor* pRowCompactor, RowSingle* pRow);
    i8   nCrwBuffers(void);
    void vDeleteRow(IndexTree* pIndexTree, EntBuffer* pEntBuffer, Ent* aEnt);
    void vDropRead(void);
    void vDropWrite();
    void vDump(void);
    bool bGrabRead(void);
    bool bGrabWrite(void);
    IndexTree* pIndexTree(i8 iIndexNo);
    IndexTreeVec* pIndexTreeVec(void);
    //void vInsdelBuffer(CrwBuffer* pCrwBuffer, t8 tPerspective);
    //void vInsdelBuffer(RsbBuffer* pCrwBuffer, t8 tPerspective);
    void vInsertIndexEntries(Crw* aCrw, Rrf& xRowRef);
    void vInsertIndexEntries(CrwBuffer* pCrwBuffer);
    bool bIsLocked(void);
    CrwBuffer* pNewCurrentCrwBuffer(void);
    void vParentJobId(i8 iJobId);
    i8   iParentJobId(void);
    i8   iPartitionId(void);
    void vPartitionId(i8 iPartitionId);
    bool bPopulateIndex(i8 iIndexNo);
    void vPublish(void);
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
//    void vUpsertRow_insert(RowSlab* pRowSlab, i8 i_Row, EntWrk* aEntWrk,
//                           Encoding_v1* pEncoding_upsert, t8 tPerspective);
//    void vUpsertRow_update(Crw* aCrw_exists, Ent* aEnt_Found,
//                           Encoding_v1* pEncoding_upsert, t8 tPerspective);
    bool bVerifyTable(void);
    void vVisualizeTable(void);
    
  private:
    Footer* aFooter(void);
  };
  
  TablePtn* _pTablePtn(void* aAddr);

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
