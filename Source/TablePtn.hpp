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
    Lock  m_xLock;        // lock on the partition
    i8    m_iPartitionId; // partition number
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
    ~TablePtn(void);
  
    void vAddIndex(IndexTree* pIndexTree);
    void vDropRead(void);
    void vDropWrite();
    bool bGrabRead(void);
    bool bGrabWrite(void);
    i8   iPartitionId(void);
    void vPartitionId(i8 iPartitionId);
    bool bPopulateIndex(i8 iIndexNo);
    IndexTree*    pIndexTree(i8 iIndexNo);
    IndexTree*    pHiddenIndexTree(void);
    IndexTreeVec* pIndexTreeVec(void);
    void vTidyUp(void);
    void vTidyUpIndex(i8 iIndexNo);
    
  private:
    Footer* aFooter(void);
  };
  
  /*--------------------------------------------------------------------------*/
  /* TablePtnVec -- Table Partition Vector                                    */
  /*--------------------------------------------------------------------------*/
  typedef Vector<CN_TABLEPTNVEC,TablePtn,CN_TABLEPTN,C_SLOTS_PER_NODE,u8> TablePtnVec;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
