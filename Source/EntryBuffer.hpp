/*============================================================================*/
/* EntryBuffer.hpp -- Entry Buffer                                            */
/*============================================================================*/
#pragma once

#define USE_EB_LAZY_CHAIN
#define USE_EB_SINGLE_COLIDX
//#define USE_EB_WATCH

namespace DBobjects
{
  class Index;
  class Table;
  class EntryBuffer;
  
  static const i8 C_ENTRIES_L0 = C_8KB;
  static const i4 C_ENTRY_NONE = C_16KB;
  static const EntryBuffer* C_ENTRYBUFFER_MARKER = (EntryBuffer*)0xFFFFFFFFFFFFFFFF;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class EntryBuffer : public ItemHeap
  {
  friend class IndexTree;

  protected:
    Lock   m_okLock;       // lock
  
    Index* m_pIndex;       // index this buffer belongs to

    ////////////////////////////////////////////////////////////////////////////
    // only used in row entry buffers
    i4*    m_iaEntryOffsets;    // array of row entry offsets
    i4     m_i_FreeOffsetChain; // head of free entry offset chain
  
    ////////////////////////////////////////////////////////////////////////////
    // only used in index entry buffers
    Entry* m_aHeadEntry; // head entry (not the first logical entry in buffer)
    Entry* m_aLastEntry; // last entry (would append to this)
    Entry* m_aLazyChain; // first lazy entry
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    EntryBuffer(void);
    ~EntryBuffer(void) noexcept(false);

    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
    static EntryBuffer* pRefdEntryBuffer(Id& xId);

  
    void    vDropWrite(void);
    void    vGrabWrite(void);
    void    vIndex(Index* pIndex);
    Index*  pIndex(void);


    ////////////////////////////////////////////////////////////////////////////
    // buffer functions
  
    void         vBufferCount(i8& nBufferCount);
    EntryBuffer* pHigherBuffer(void);
    void         vHigherBuffer(EntryBuffer* pHigherLevel);
    EntryBuffer* pNextBuffer(void);
    void         vNextBuffer(EntryBuffer* pNextBuffer);
    EntryBuffer* pPrevBuffer(void);
    void         vPrevBuffer(EntryBuffer* pPrevBuffer);
    void         vTruncate(void);

    ////////////////////////////////////////////////////////////////////////////
    // entry functions
  
    Entry* aFirstEntry(void);
    Entry* aLastEntry(void);

    ////////////////////////////////////////////////////////////////////////////
    // row functions
  
    Entry* aAppendRow(RowWrk& xRowWrk);
  
  private:
    Entry* aAllotEntry(i8 nBytes);
    NEO*   aAllotNEO(i8 nBytes);
    void   vInitFreeChain(void);

  
  
    ////////////////////////////////////////////////////////////////////////////
  
  
  public:
    void vAssertEntryBuffer(void);
    bool bValidEntryBuffer(void);
  
 
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // complex functions
  
    void         vAnalyze(s48K& sAnalysis, bool bDetailed, i8& iBuffers, i8& iEntries);
    void         vBuildSkipListSorted(void);
    void         vDeleteEntry(Entry** aaEntry);
    bool         bDeleteKey(Key* aDeleteKey, EntryBuffer* pToBuffer);
    void         vDump(void);
    EntryBuffer* pFirstChildBuffer(void);
    Entry*       aGetEntryRowNo(i8 i_RowNo);
    Entry*       aInsertKeyAbs(KeyWrk& xKeyWrk);
    Entry*       aInsertKeyAbsL0(KeyWrk& xKeyWrk, Entry** aaPriorEntry);
    static Entry* aInsertKeyTree(KeyWrk& xKeyWrk,
                                 EntryBuffer** ppRootKeyBuffer);
    void         vLazyBuild(void);
    i8           iLengthList(void);
    i8           iLengthSubList(void);
    Entry*       aNextEntry(Entry* aEntry, i8 i_Offset);
  
    Entry*       aPositionQuery(Key* aGivenKey, eRO eRelOp);
    Entry*       aPositionQuery(Key* aGivenKey, i8 nKeyCols, eJC eJuxtaCompare);
    EntryBuffer* pPositionLevel(Key* aGivenKey, i8 nKeyCols, eJC eJuxtaCompare, i8 iLevel);
    
    void         vReset(void);
    void         vRestore(void);
    void         vRestoreIndex(Index* pIndex);
    void         vShow(Entry* aMarkEntry = nullptr);
    void         vShowShort(void);
    void         vShowSubList(void);
    void         vShowTree(bool bDumpBuffer=false);
    void         vSpliceAfter(EntryBuffer* pSplicedBuffer);
    EntryBuffer* pUnlinkSubList(void);
    bool         bVerify(void);
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    private:
    
    void         vBuildSkipList(void);
    void         vCompact(void);
    void         vCompactLeft2(void);
    void         vCutEntry(Entry* aEntry, Entry* aPriorVec[]);
    void         vDeleteUpwards(Key* aDeleteKey,
                                EntryBuffer** ppRootKeyBuffer);
    void         vDeleteUpwards(Key* aDeleteKey, EntryBuffer* pToBuffer,
                                EntryBuffer** ppRootKeyBuffer);
    void         vDumpPriorVec(Entry* aPriorVec[]);
    FreeItem*   aFirstFree(void);
    Item*        aFirstItem(void);
    Entry*       aGetEntryOffset(i8 iOffset);
    Entry*       aGetEntrySplit(i8 iSplitEntryNo, Entry** aaPriorEntry);
    i4           iGetFreeRowEntry(void);
    void         vHandleFirstInsert(Entry* aInsertedKeyEntry,
                                    Entry* aOriginalFirstEntry,
                                    EntryBuffer** ppRootKeyBuffer);
    void         vHandleLeftInsert(Entry* aInsertedKeyEntry,
                                   Entry* aOriginalFirstEntry,
                                   EntryBuffer** ppRootKeyBuffer);
    Entry*       aHeadEntry(void);
    void         vHeadEntryNew(void);
    void         vHeadEntryRestore(void);
    EntryBuffer* pInsertableBuffer(KeyWrk& xKeyWrk,
                                   EntryBuffer** ppRootKeyBuffer);
    Entry*       aInsertKeyAbsLazy(KeyWrk& xKeyWrk);
    void         vInsertKeySkip(Entry* aKeyEntry);
    Entry*       aInsertKeyBuffer(KeyWrk& xKeyWrk,
                                  EntryBuffer** ppRootKeyBuffer);
    Entry*       aInsertKeyUpwards(KeyWrk& xKeyWrk,
                                   EntryBuffer** ppRootKeyBuffer);
    void         vItemCountRestore(void);
    bool         bKeyBeforeBuffer(Key* aKey);
    FreeItem*    aMakeFreeEntry(Entry* aEntry);
    EntryBuffer* pNewHigherBuffer(void);
    EntryBuffer* pNewInsertBuffer(Key* aInsKey, i8 nEntryPlusOffsets,
                                  EntryBuffer** ppRootKeyBuffer);
    EntryBuffer* pNewNextBuffer(void);
    Entry*       aPopLazyChain(void);
    EntryBuffer* pPositionPriorBuffer(Key* aFindKey);
    Entry*       pPositionPriorEntry(Key* aFindKey);
    void         vPositionPriorVecEntry(Key* aFindKey, Entry** aaPriorVec,
                                        Entry* aFindEntry);
    void         vPositionPriorVecBuffer(Key* aFindKey, Entry** aaPriorVec,
                                         EntryBuffer* pToBuffer);
    void         vPositionPriorVecBuffer2(Key* aFindKey, Entry** aaPriorVec,
                                          EntryBuffer* pToBuffer);
    void         vPositionPriorVecBuffer3(Key* aFindKey, Entry** aaPriorVec,
                                          EntryBuffer* pToBuffer);
    EntryBuffer* pSplit2(void);
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // verify functions
  
    bool         bCheckPriorVec(Entry* aPriorVec[]);
    void         vNumberEntries(void);
  
    void         vQuickVerify(Entry* aSeeEntry=nullptr);
    bool         bQuickVerify(Entry* aNotSeeEntry);
  
    bool         bVerifyL0Hidden(void);
    bool         bVerifyL0Other(void);
    bool         bVerifyLnAny(void);

    bool         bVerifyAdjacent(void);
    bool         bVerifyBufferRefs(void);
    bool         bVerifyEntryOffset(void);
    bool         bVerifyFirstKeys(void);
    bool         bVerifyFreeList(void);
    bool         bVerifyItems(void);
    bool         bVerifyKeyBufferOrder(void);
    bool         bVerifyKeyOrder(void);
    bool         bVerifyNextPrev(void);
    bool         bVerifyRows(void);
    bool         bVerifyRowRefs(void);
    bool         bVerifySkipListEntry(bool bShowKeys = false);
    bool         bVerifySkipListLevels(bool bShowKeys = false);
    void         vVerifyFault(void);

  
    public:
    Footer* aFooter(void);
  
    static void vUnitTest_c(Table* pTable);
  };
  
  EntryBuffer* _pEntryBuffer(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
