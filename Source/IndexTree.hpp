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
    
    EntryBuffer* m_pRootBuffer;    // the root buffer

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
    void         vCompact(void);
    void         vCopyReInitL1(EntryBuffer* pL1, EntryBuffer* pLeftL0);
    void         vCopyLeftV2(EntryBuffer** ppLeftL0, EntryBuffer* pRightL0);
    void         vCopyLeftV3(EntryBuffer** ppLeftL0, EntryBuffer* pRightL0);
    void         vDump(bool bSameHeight);
    i8           nEntryBuffers(void);
    EntryBuffer* pFirstH0Buffer(void);
    Index*       pIndex(void);
    bool         bInsertEntry(RowNub* pRowNub, i8 i_Row, CacheLine* aRowData, Id& xId);
    void         vInsertEntriesFromBuffer(RowBuffer* pRowBuffer);
    void         vLazyBuild(void);    
    EntryBuffer* pPositionH0Buffer(KeyWrk& xKeyWrk);
    EntryBuffer* pRoot(void);
    void         vTidyUp(void);
    void         vTruncate(void);
    bool         bVerify(void);
    bool         bVerifyIndex(void);
    void         vVisualize(void);
    
    Footer*      aFooter(void);
    
  private:
    void         vDumpLevel(EntryBuffer* pEntryBuffer);
    EntryBuffer* pFirstL1Buffer(void);
    EntryBuffer* pPopSubList(EntryBuffer** ppSubList);
  };
  
  IndexTree* _pIndexTree(void* pAddr);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
 }

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
