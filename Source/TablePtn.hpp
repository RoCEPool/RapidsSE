/*============================================================================*/
/* TablePtn.hpp -- Table Partition                                            */
/*============================================================================*/
#pragma once

namespace Partitioning
{
  typedef VectorNL_t<CN_INDEXTREEVEC, IndexTree, C_MAX_INDEXES> IndexTreeVec;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
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
  
    //! TO GO
    i8   m_nWriteBehindBuffers;     // buffers to be written
    i8   m_nBuffersWritten;         // buffers written so far
    i8   m_nSubLoadersDone;         // number of subloader which have completed
    i8   m_nSubInsertersDone;       // number of subInserters which have completed
    i8   m_iParentJobId;            // parent jobid
    
    // must be last
    Footer m_xFooter;
    
  public:
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
    static i8   iPartitionToNode(i8 iPartition);
    static i8   iPartitionToSlot(i8 iPartition);
    static i8   iSlotToPartition(i8 iSlot);
  
    void* operator new(size_t);
    void  operator delete(void*);
    
    TablePtn(Table* pTable);
    ~TablePtn(void) noexcept(false);
  
    void vAddIndex(IndexTree* pIndexTree);
    Crw* aAllotFixedSizeCrw(Ref& xRowRef);
    void vAppendBuffer(CrwBuffer* pCrwBuffer);
    void vAppendRow(RowCompactor* pRowCompactor, RowSingle* pRow);
    i8   nCrwBuffers(void);
    void vDropRead(void);
    void vDropWrite();
    void vDump(void);
    bool bGrabRead(void);
    bool bGrabWrite(void);
    IndexTree* pIndexTree(i8 iIndexNo);
    IndexTreeVec* pIndexTreeVec(void);
    void vInsertIndexEntries(Crw* aCrw, Ref& xRowRef);
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
    void vResetWrite(void);
    CrwBuffer* pRootCrwBuffer(void);
    CrwBuffer* pSnapshot(CrwBuffer* pCrwBufferSrc, t8 tPerspective);
    Table* pTable(void);
    void vTruncate(void);
    void vTruncateIndex(Index* pIndex);
    void vUpsertBuffer(CrwBuffer* pCrwBuffer, t8 tPerspective);
    void vUpsertRow_insert(Crw* aCrw_nosuch, EntWrk* aEntWrk,
                           Encoding* pEncoding_upsert, t8 tPerspective);
    void vUpsertRow_update(Crw* aCrw_exists, Ent* aEnt_Found,
                           Encoding* pEncoding_upsert, t8 tPerspective);
    bool bVerifyTable(void);
    void vVisualizeTable(void);

    //! TO GO
    void vAddLoadWriteBehindBuffers(i8 nBuffersLoadable);
    void vAddInsertWriteBehindBuffers(i8 nBuffersInsertable);
    bool bAllBuffersWritten(eDS eDataSource);
    void vAppendBufferOLD(CrwBuffer* pCrwBuffer);
    void vIncBuffersWritten(void);
    i8   nWriteBehindBuffers(void);
    void vWriteBehindBuffers(i8 nWriteBehindBuffers);
    
  private:
    Footer* aFooter(void);
  };
  
  TablePtn* _pTablePtn(void* aAddr);

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
