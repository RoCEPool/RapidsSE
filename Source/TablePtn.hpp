/*============================================================================*/
/* TablePtn.hpp -- Table Partition                                            */
/*============================================================================*/
#pragma once

namespace Partitioning
{
  
  typedef VectorNL_t  <CN_INDEXTREEVEC, IndexTree,
                       CN_INDEXTREE,    C_MAX_INDEXES, u8>  IndexTreeVec;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  class TablePtn : public Object
  {
  private:
    Lock m_okLock;               // lock on the partition
    Table* m_pTable;             // table this partition belongs to
    i8 m_iPartitionId;           // partition number
    bool m_bLowMemLoad;          // was low memory detected during this load
  
    i8 m_nWriteBehindBuffers;    // buffers to be written
    i8 m_nBuffersWritten;        // buffers written so far
    
    i8 m_nSubLoadersDone;        // number of subloader which have completed
    i8 m_nSubInsertersDone;      // number of subInserters which have completed
    i8 m_iParentJobId;           // parent jobid
    
    CrwBuffer* m_pRootCrwBuffer;      // the data root buffer
    CrwBuffer* m_pCurrentCrwBuffer;   // the data current buffer for insert
    
    IndexTreeVec* m_pIndexTreeVec; // vector of index trees
  
    // must be last
    Footer m_xFooter;
    
  public:
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
    static i8   iPartitionToNode(i8 iPartition);
    static i8   iPartitionToSlot(i8 iPartition);
    static i8   iSlotToPartition(i8 iSlot);
  
    void* operator new(size_t);
    void  operator delete(void*);
    
    TablePtn(void);
    ~TablePtn(void) noexcept(false);
  
    void vAddLoadWriteBehindBuffers(i8 nBuffersLoadable);
    void vAddInsertWriteBehindBuffers(i8 nBuffersInsertable);
    void vAddIndex(Data::IndexTree* pIndexTree);
    bool bAllBuffersWritten(eDS eDataSource);
    void vAppendBuffer(CrwBuffer* pCrwBuffer);
    void vAppendRow(RowCoder* pRowCoder, Row* pRow);
    void vDump(void);
    void vWriteBehindBuffers(i8 nWriteBehindBuffers);
    void vBuffersWritten(i8 m_nBuffersWritten);
    i8   nWriteBehindBuffers(void);
    i8   nBuffersWritten(void);
    void vDropRead(void);
    void vDropWrite();
    CrwBuffer* pFirstCrwBuffer(void);
    bool bGrabRead(void);
    bool bGrabWrite(void);
    void vIncBuffersWritten(void);
    IndexTree* pIndexTree(i8 iIndexNo);
    IndexTreeVec* pIndexTreeVec(void);
    bool bIsLocked(void);
    void vParentJobId(i8 iJobId);
    i8   iParentJobId(void);
    i8   iPartitionId(void);
    void vPartitionId(i8 iPartitionId);
    bool bPopulateIndex(i8 iIndexNo);
    void vPublish(void);
    bool bPurge(i8 iOlderThanSeconds);
    void vResetWrite(void);
    CrwBuffer* pRootCrwBuffer(void);
    i8   nCrwBuffers(void);
    void vTable(Table* pTable);
    Table* pTable(void);
    void vTidyUp(void);
    void vTidyUpIndex(i8 iIndexNo);
    void vTruncate(void);
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
