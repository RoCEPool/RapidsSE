/*============================================================================*/
/* TablePtn.hpp -- Table Partition                                            */
/*============================================================================*/
#pragma once

namespace Data
{
  class IndexTree;
  
  typedef VectorNL_t<CN_INDEXTREEVEC,
                     IndexTree,
                     CN_INDEXTREE,
                     C_INDEXES_PER_TABLE,
                     u8> IndexTreeVec;
}

namespace MetaData
{
  class Index;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  class TablePtn : public Object
  {
  private:
    Lock m_okLock; // lock on the partition
  
    i8 m_iPartitionId; // partition number
    bool m_bLowMemLoad;          // was low memory detected during this load
  
    i8 m_nWriteBehindBuffers;    // buffers to be written
    i8 m_nBuffersWritten;        // buffers written so far
    
    i8 m_nSubLoadersDone;        // number of subloader which have completed
    i8 m_nSubInsertersDone;      // number of subInserters which have completed
    i8 m_iParentJobId;           // parent jobid
    
    Data::IndexTreeVec* m_pIndexTreeVec; // vector of index trees
  
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
    void vWriteBehindBuffers(i8 nWriteBehindBuffers);
    void vBuffersWritten(i8 m_nBuffersWritten);
    i8   nWriteBehindBuffers(void);
    i8   nBuffersWritten(void);
    void vDropRead(void);
    void vDropWrite();
    bool bGrabRead(void);
    bool bGrabWrite(void);
    Data::IndexTree* pHiddenIndexTree(void);
    void vIncBuffersWritten(void);
    Data::IndexTree* pIndexTree(i8 iIndexNo);
    Data::IndexTreeVec* pIndexTreeVec(void);
    bool bIsLocked(void);
    void vParentJobId(i8 iJobId);
    i8   iParentJobId(void);
    i8   iPartitionId(void);
    void vPartitionId(i8 iPartitionId);
    bool bPopulateIndex(i8 iIndexNo);
    void vResetWrite(void);
    void vTidyUp(void);
    void vTidyUpIndex(i8 iIndexNo);
    
  private:
    Footer* aFooter(void);
  };
  
  TablePtn* _pTablePtn(void* aAddr);

  /*--------------------------------------------------------------------------*/
  /* TablePtnVec -- Table Partition Vector                                    */
  /*--------------------------------------------------------------------------*/
  typedef VectorNL_t<CN_TABLEPTNVEC,TablePtn,CN_TABLEPTN,C_SLOTS_PER_NODE,u8> TablePtnVec;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
