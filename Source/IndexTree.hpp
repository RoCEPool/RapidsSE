/*============================================================================*/
/* IndexTree.hpp - index portion of table partition                           */
/*============================================================================*/
#pragma once

namespace DBobjects
{

  static const i8 C_HIGHER_LEVEL_ID = 0xFFFF;
  
  class Table;
  class Partition;
  class Index;
  class WorkingBuffer;
  
  class IndexTree : public Object
  {
    
  private:
    Lock         m_zLock;          // lock on the buffer tree
    Index*       m_pIndex;         // which index
    i8           m_iPartitionNo;   // which partition
    EntryBuffer* m_pRoot;          // the root buffer
    EntryBuffer* m_pCurrentBuffer; // current buffer for insert hidden index
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void* operator new(size_t, IndexTree* pIndexTree);
    void  operator delete(void*);
    
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
    static void vFinalize(eCN eClass, Object* pObject);
    
    IndexTree(Index* pIndex, i8 iPartitionNo);
    ~IndexTree(void);
  
    Stz*         pAnalyze(void);
    void         vAppendEntriesFromBuffer(i8 iTxId,
                                          WorkingBuffer* pWorkingBuffer,
                                          EntryBuffer** ppFirstBuffer,
                                          Entry** ppFirstEntry);
    void         vAppendBuffer(EntryBuffer* pEntryBuffer);
    void         vAppendEntry(KeyWrk& xKeyWrk, EntryVec& xPriorVec);
    void         vDump(bool bSameLevel);
    EntryBuffer* pFirstL0Buffer();
    Index*       pIndex(void);
    i8           nEntryBuffers(void);
    
    void         vInsertEntriesFromBuffer(EntryBuffer* pEntryBuffer, Entry* pRowEntry);
    EntryBuffer* pPositionLevel(KeyWrk& xKeyWrk, i8 nKeyCols, eJC eJuxtaCompare, i8 iLevel);
    EntryBuffer* pRoot(void);
    void         vTidyUp(void);
    void         vTruncate(void);
    bool         bVerify(void);
    bool         bVerifyIndex(void);
    void         vVisualize(void);
    
    Footer*      aFooter(void);
    
  private:
    //EntryBuffer* pAppendL0Entry(EntryBuffer* pRoot,KeyWrk& xKeyWrk, Id xId,
    //                            EntryVec& xPriorVec);
    //EntryBuffer* pAppendL0Entry(EntryBuffer* pRoot,KeyWrk& xKeyWrk, Id xId);
    //void         vFixHigherBuffer(EntryBuffer* pEntryBuffer, i8 iLevel);
    //bool         bInsertEntry(EntryBuffer* pRoot,KeyWrk& xKeyWrk, Id xId, i8 iLevel);
    //void         vInsertFirstEntry(EntryBuffer* pToBuffer, EntryBuffer* pFromBuffer);
    //EntryBuffer* pSplitBuffer(EntryBuffer* pEntryBuffer);
    void         vDumpLevel(EntryBuffer* pEntryBuffer);
    bool         bInsertEntry(Entry* pRow);
  };
  
  IndexTree* _pIndexTree(void* pAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
