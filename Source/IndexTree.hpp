/*============================================================================*/
/* IndexTree.hpp - index portion of table partition                           */
/*============================================================================*/
#pragma once

namespace Data
{
  static const i8 C_HIGHER_LEVEL_ID = 0xFFFF;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  
  class IndexTree : public Object
  {
    
  private:
    Lock         m_okLock;         // lock on the buffer tree
    Index*       m_pIndex;         // which index
    i8           m_iPartitionNo;   // which partition no
    TablePtn*    m_pPartition;     // which partition                     UNUSED
    EntryBuffer* m_pRootBuffer;    // the root buffer
    EntryBuffer* m_pCurrentBuffer; // current buffer for insert hidden index
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void* operator new(size_t, IndexTree* pIndexTree);
    void  operator delete(void*);
    
    IndexTree(Index* pIndex, i8 iPartitionNo);
    ~IndexTree(void) noexcept(false);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
    static void vFinalize(eCN eClass, Object* pObject);
    static void vMonitorPortrait(s48K& sPortrait);
  
    Stz*         pAnalyze(void);
    
    Item*       pAppendRow(i8 iTxId,
                            Stz* aRow_Data,
                            i8 iRowLength);
    void         vAppendBuffer(EntryBuffer* pEntryBuffer);
    void         vCompact(void);
    void         vCopyReInitL1(EntryBuffer* pL1, EntryBuffer* pLeftL0);
    void         vCopyLeftV2(EntryBuffer** ppLeftL0, EntryBuffer* pRightL0);
    void         vCopyLeftV3(EntryBuffer** ppLeftL0, EntryBuffer* pRightL0);
  //void         vCopyLeftV1(EntryBuffer** ppLeftL0, EntryBuffer* pRightL0);
    void         vDump(bool bSameHeight);
    EntryBuffer* pFirstL1Buffer(void);
    EntryBuffer* pFirstH0Buffer(void);
    Index*       pIndex(void);
    i8           nEntryBuffers(void);
    
    bool         bInsertEntry(Item* aRowItem);
    void         vInsertEntriesFromBuffer(EntryBuffer* pEntryBuffer);
    void         vLazyBuild(void);
    EntryBuffer* pPopSubList(EntryBuffer** ppSubList);
    EntryBuffer* pPositionH0Buffer(KeyWrk& xKeyWrk);
    void         vPublish(void);
    bool         bPurge(i8 iOlderThanSeconds);
    EntryBuffer* pRoot(void);
    void         vTidyUp(void);
    void         vTruncate(void);
    bool         bVerify(void);
    bool         bVerifyIndex(void);
    void         vVisualize(void);
    
    Footer*      aFooter(void);
    
  private:
    void         vDumpLevel(EntryBuffer* pEntryBuffer);
  };
  
  IndexTree* _pIndexTree(void* pAddr);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  
 }

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
