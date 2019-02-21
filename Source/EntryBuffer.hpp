/*============================================================================*/
/* EntryBuffer.hpp -- Item Buffer                                            */
/*============================================================================*/
#pragma once

#define USE_EB_LAZY_CHAIN
#define USE_EB_SINGLE_COLIDX
//#define USE_EB_WATCH

namespace Data
{
  static const bool C_NOT_FINAL = false;
  static const bool C_FINAL     = true;

  class EntryBuffer : public ItemHeap
  {
  friend class IndexTree;

  protected:
    Lock   m_okLock; // lock
    Index* m_pIndex; // index this buffer belongs to
 
    SkipRoots m_xSkipRoots;

    ////////////////////////////////////////////////////////////////////////////
    // only used in index entry buffers
    //Item* m_aHeadEntry; // skiplist head (not first logical entry in buffer)
    Item* m_aLastEntry; // last entry (would append to this)
    Item* m_aLazyChain; // first lazy entry
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    EntryBuffer(void);
    ~EntryBuffer(void) noexcept(false);

    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
    static EntryBuffer* pRefdEntryBuffer(Item* pItem);
    static EntryBuffer* pRefdEntryBuffer(Id& xId);
  
    void    vDropWrite(void);
    void    vGrabWrite(void);
    void    vIndex(Index* pIndex);
    Index*  pIndex(void);


    ////////////////////////////////////////////////////////////////////////////
    // buffer functions
  
    void         vBufferCount(i8& nBufferCount);
    EntryBuffer* pHigherBuffer(void);
    void         vHigherBuffer(EntryBuffer* pHigherBuffer);
    EntryBuffer* pNextBuffer(void);
    void         vNextBuffer(EntryBuffer* pNextBuffer);
    EntryBuffer* pPrevBuffer(void);
    void         vPrevBuffer(EntryBuffer* pPrevBuffer);
    void         vTruncate(void);

    ////////////////////////////////////////////////////////////////////////////
    // entry functions
  
    Item* aGetEntry_Item(i8 i_Item);
    Item* aLeadEntry(void);
    Item* aLastEntry(void);
    void  vShowEntry(Item* pEntry);

    ////////////////////////////////////////////////////////////////////////////
    // row functions
  
    Item* aAppendRow(RowWrk& xRowWrk);
  
  private:
    Item* aAllotEntry(i8 nBytes_Without_Offsets);
    Item* aAllotRowEntry(RowWrk& xRowWrk);

  
  
    ////////////////////////////////////////////////////////////////////////////
  
  
  public:
    void vAssertEntryBuffer(void);
    bool bValidEntryBuffer(void);
  
 
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // complex functions
  
    void         vAnalyze(s48K& sAnalysis, bool bDetailed, i8& iBuffers, i8& iEntries);
    void         vBuildSkipListSorted(void);
    void         vDeleteEntry(Item** aaItem);
    //bool         bDeleteKey(KeyData* aDeleteKey, EntryBuffer* pToBuffer);
    void         vDump(void);
    EntryBuffer* pFirstChildBuffer(void);
    Item*       aInsertKeyAbs(KeyWrk& xKeyWrk);
    Item*       aInsertKeyAbsL0(KeyWrk& xKeyWrk, Item** aaPriorEntry);
    static Item* aInsertKeyTree(KeyWrk& xKeyWrk,
                                 EntryBuffer** ppRootKeyBuffer);
    void         vLazyBuild(void);
    i8           iLengthList(void);
    i8           iLengthSubList(void);
    Item*       aNextEntry(Item* aItem, i8 i_Offset);
  
    Item*       aPositionQuery(KeyData* aGivenKey, eRO eRelOp);
    Item*       aPositionQuery(KeyData* aGivenKey, i8 nKeyCols);
    EntryBuffer* pPositionHeight(KeyData* aGivenKey, i8 nKeyCols, eJC eJuxtaCompare, i8 iHeight);
    
    void         vReset(void);
    void         vRestore(void);
    void         vRestoreIndex(Index* pIndex);
    void         vShow(Item* aMarkEntry = nullptr);
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
    void         vCutEntry(Item* aItem, Item* aPriorVec[]);
    void         vDeleteUpwards(KeyData* aDeleteKey,
                                EntryBuffer** ppRootKeyBuffer);
//    void         vDeleteUpwards(KeyData* aDeleteKey, EntryBuffer* pToBuffer,
//                                EntryBuffer** ppRootKeyBuffer);
    void         vDumpPriorVec(Item* aPriorVec[]);
    Item*        aGetEntryOffset(i8 iOffset);
    Item*        aGetEntrySplit(i8 iSplitEntryNo, Item** aaPriorEntry);
    void         vHandleFirstInsert(Item* aInsertedKeyEntry,
                                    Item* aOriginalFirstEntry,
                                    EntryBuffer** ppRootKeyBuffer);
    void         vHandleLeftInsert(Item* aInsertedKeyEntry,
                                   Item* aOriginalFirstEntry,
                                   EntryBuffer** ppRootKeyBuffer);
    Item*        aSkipRoots(void);
    void         vHeadEntryNew(void);
    void         vHeadEntryRestore(void);
    EntryBuffer*  pInsertableBuffer(KeyWrk& xKeyWrk,
                                   EntryBuffer** ppRootKeyBuffer);
    Item*        aInsertKeyAbsLazy(KeyWrk& xKeyWrk);
    void         vInsertKeySkip(Item* aKeyEntry);
    Item*        aInsertKeyBuffer(KeyWrk& xKeyWrk,
                                  EntryBuffer** ppRootKeyBuffer);
    Item*        aInsertKeyUpwards(KeyWrk& xKeyWrk,
                                   EntryBuffer** ppRootKeyBuffer);
    void        vItemCountRestore(void);
    bool        bKeyBeforeBuffer(KeyWrk& xKey);
    FreeItem*    aMakeFreeEntry(Item* aItem);
    EntryBuffer* pNewHigherBuffer(void);
    EntryBuffer* pNewInsertBuffer(KeyWrk& xInsKey, i8 nEntryPlusOffsets,
                                  EntryBuffer** ppRootKeyBuffer);
    EntryBuffer* pNewNextBuffer(void);
    Item*       aPopLazyChain(void);
    EntryBuffer* pPositionPriorBuffer(KeyData* aFindKey);
    Item*       pPositionPriorEntry(KeyData* aFindKey);
    void         vPositionPriorVecEntry(KeyData* aFindKey, Item** aaPriorVec,
                                        Item* aFindEntry);
    void         vPositionPriorVecBuffer(KeyData* aFindKey, Item** aaPriorVec,
                                         EntryBuffer* pToBuffer);
//    void         vPositionPriorVecBuffer2(KeyData* aFindKey, Item** aaPriorVec,
//                                          EntryBuffer* pToBuffer);
//    void         vPositionPriorVecBuffer3(KeyData* aFindKey, Item** aaPriorVec,
//                                          EntryBuffer* pToBuffer);
    EntryBuffer* pSplit2(void);
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // verify functions
  
    bool         bCheckPriorVec(Item* aPriorVec[]);
    void         vNumberEntries(void);
  
    void         vQuickVerify(Item* aSeeEntry=nullptr);
    bool         bQuickVerify(Item* aNotSeeEntry);
  
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
