/*============================================================================*/
/* TablePtn.hpp -- Table Partition                                            */
/*============================================================================*/
#pragma once

namespace DBobjects
{
  class Index;
  class EntWrk;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  class TablePtn : public Object
  {
  private:
    Lock m_xLock; // lock on the partition
  
    i8 m_iPartitionId; // partition number
  
    i8 m_nSubLoadersDone;        // number of subloeader which have completed
    i8 m_nTotalBuffersLoadable;  // total no of buffers to be loaded
    i8 m_nTotalBuffersLoaded;    // total no of buffers loaded so far

    bool m_bLowMemLoad;          // was low memory detected during this load
  
    i8 m_nWriteBehindBuffers;   // buffers to be written
    i8 m_nBuffersWritten;       // buffers written so far
    i8 m_iEndInsertJobId;        // end insert jobid
    
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
  
    void vAddTotalBuffersLoadable(i8 nBuffersLoadable);
    void vAddIndex(IndexTree* pIndexTree);
    bool bAllBuffersLoaded(void);
    void vWriteBehindBuffers(i8 nWriteBehindBuffers);
    void vBuffersWritten(i8 m_nBuffersWritten);
    i8   nWriteBehindBuffers(void);
    i8   nBuffersWritten(void);
    void vDropRead(void);
    void vDropWrite();
    void vEndInsertJobId(i8 iJobId);
    i8   iEndInsertJobId(void);
    bool bGrabRead(void);
    bool bGrabWrite(void);
    IndexTree* pHiddenIndexTree(void);
    void vIncTotalBuffersLoaded(void);
    void vIncBuffersWritten(void);
    IndexTree* pIndexTree(i8 iIndexNo);
    IndexTreeVec* pIndexTreeVec(void);
    bool bIsLocked(void);
    i8   iPartitionId(void);
    void vPartitionId(i8 iPartitionId);
    bool bPopulateIndex(i8 iIndexNo);
    void vResetLoad(void);
    void vTidyUp(void);
    void vTidyUpIndex(i8 iIndexNo);
    
  private:
    Footer* aFooter(void);
  };
  
  TablePtn* _pTablePtn(void* aAddr);

  /*--------------------------------------------------------------------------*/
  /* TablePtnVec -- Table Partition Vector                                    */
  /*--------------------------------------------------------------------------*/
  typedef Vector<CN_TABLEPTNVEC,TablePtn,CN_TABLEPTN,C_SLOTS_PER_NODE,u8> TablePtnVec;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
