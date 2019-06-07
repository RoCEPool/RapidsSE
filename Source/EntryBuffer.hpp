/*============================================================================*/
/* EntryBuffer.hpp -- Item Buffer                                            */
/*============================================================================*/
#pragma once

#define USE_EB_LAZY_CHAIN
//#define USE_EB_WATCH

namespace Data
{
  static const bool C_NOT_FINAL = false;
  static const bool C_FINAL     = true;

  class EntryBuffer : public DataBuffer,
                      public MiniHeap_t<Item>
  {
  friend class IndexTree;

  protected:
    Index*    m_pIndex;     // index this buffer belongs to
    SkipRoots m_xSkipRoots; // skip roots
    Item*     m_aLazyChain; // first lazy entry
  
    SeeMe m_xSeeMe;
  
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

    ////////////////////////////////////////////////////////////////////////////
    // entry functions
  
    Item* aLeadEntry(void);
    Item* aLastEntry(void);
    void  vShowEntry(Item* pEntry);
  
  private:
    Item* aAllotEntry(i8 nBytes_Without_Offsets);
    
    ////////////////////////////////////////////////////////////////////////////
  
  public:
    void vAssertEntryBuffer(void);
    bool bValidEntryBuffer(void);
    
    ////////////////////////////////////////////////////////////////////////////
    // static move functions
    
    static Item* pMoveToExactEntry(Item* aItem, KeyWrk& xKeyWrk, EntryBuffer** pAEntryBuffer);
    static Item* pMoveToGreaterThanEntry(Item* aItem, KeyWrk& xKeyWrk, EntryBuffer** pAEntryBuffer);
    static Item* pMoveToGreaterEqualEntry(Item* aItem, KeyWrk& xKeyWrk, EntryBuffer** pAEntryBuffer);
    static Item* pMoveToPrevEntry(Item* aItem, KeyWrk& xKeyWrk, EntryBuffer** pAEntryBuffer);
    static Item* pMoveToNextEntry(Item* aItem, KeyWrk& xKeyWrk, EntryBuffer** pAEntryBuffer);
    
    ////////////////////////////////////////////////////////////////////////////
    // key positioning functions
    
    EntryBuffer* pPositionH0Buffer(KeyWrk& xGivenKey);
    Item*        pPositionPriorEntry(KeyWrk& xFindKey);
    Item*        pPositionPriorEntry(Item* aFindKey);
    void         vPositionPriorVecEntry(Item* aFindKey, Item** aaPriorVec,
                                        Item* aFindEntry);
    
    void         vPositionPriorVecBuffer(Item* aFindKey, Item** aaPriorVec,
                                         EntryBuffer* pToBuffer);
    
    ////////////////////////////////////////////////////////////////////////////
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // complex functions
  
    void        vAnalyze(s48K& sAnalysis, bool bDetailed, i8& iBuffers, i8& iEntries);
    void        vBuildSkipListSorted(void);
    void        vDeleteEntry(Item** aaItem);
    void        vDump(void);
    EntryBuffer* pFirstChildBuffer(void);
    Item*       aInsertKeyAbs(KeyWrk& xKeyWrk);
    Item*       aInsertKeyAbsL0(KeyWrk& xKeyWrk, Item** aaPriorEntry);
    static Item* aInsertKeyTree(KeyWrk& xKeyWrk,
                                 EntryBuffer** ppRootKeyBuffer);
    void        vLazyBuild(void);
    i8          iLengthList(void);
    i8          iLengthSubList(void);
    Item*       aNextEntry(Item* aItem, i8 i_Offset);
    
    void        vReset(void);
    void        vRestore(void);
    void        vRestoreIndex(Index* pIndex);
    void        vShow(Item* aMarkEntry = nullptr);
    void        vShowShort(void);
    void        vShowSubList(void);
    void        vShowTree(bool bDumpBuffer=false);
    void        vSpliceAfter(EntryBuffer* pSplicedBuffer);
    void        vTruncate(void);
    EntryBuffer* pUnlinkSubList(void);
    bool        bVerify(void);
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    private:
    
    void        vBuildSkipList(void);
    void        vCompact(void);
    void        vCompactLeft(void);
    void        vCutEntry(Item* aItem, Item* aPriorVec[]);
    void        vDeleteUpwards(Item* aDeleteKey,
                                EntryBuffer** ppRootKeyBuffer);
    void        vDumpPriorVec(Item* aPriorVec[]);
    Item*       aGetEntryOffset(i8 iOffset);
    Item*       aGetEntrySplit(i8 iSplitEntryNo, Item** aaPriorEntry);
    void        vHandleFirstInsert(Item* aInsertedKeyEntry,
                                   Item* aOriginalFirstEntry,
                                  EntryBuffer** ppRootKeyBuffer);
    void        vHandleLeftInsert(Item* aInsertedKeyEntry,
                                   Item* aOriginalFirstEntry,
                                   EntryBuffer** ppRootKeyBuffer);
    Item*       aSkipRoots(void);
    EntryBuffer* pInsertableBuffer(KeyWrk& xKeyWrk,
                                   EntryBuffer** ppRootKeyBuffer);
    Item*       aInsertKeyAbsLazy(KeyWrk& xKeyWrk);
    void        vInsertKeySkip(Item* aKeyEntry);
    Item*       aInsertKeyBuffer(KeyWrk& xKeyWrk,
                                  EntryBuffer** ppRootKeyBuffer);
    Item*       aInsertKeyUpwards(KeyWrk& xKeyWrk,
                                   EntryBuffer** ppRootKeyBuffer);
    bool         bKeyBeforeBuffer(KeyWrk& xKey);
    FreeItem*    aMakeFreeEntry(Item* aItem);
    EntryBuffer* pNewHigherBuffer(void);
    EntryBuffer* pNewInsertBuffer(KeyWrk& xInsKey, i8 nEntryPlusOffsets,
                                  EntryBuffer** ppRootKeyBuffer);
    EntryBuffer* pNewNextBuffer(void);
    Item*       aPopLazyChain(void);

    EntryBuffer* pSplitBuffer(void);
  
    NEO*   aAllotNEO(i8 nBytes);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // verify functions
  
    bool         bCheckPriorVec(Item* aPriorVec[]);
    void         vNumberEntries(void);
  
    void         vQuickVerify(Item* aSeeEntry=nullptr);
    bool         bQuickVerify(Item* aNotSeeEntry);
  
    bool         bVerifyL0Other(void);
    bool         bVerifyLnAny(void);

    bool         bVerifyAdjacent(void);
    bool         bVerifyBufferRefs(void);
    bool         bVerifyFirstKeys(void);
    bool         bVerifyItems(void);
    bool         bVerifyKeyBufferOrder(void);
    bool         bVerifyKeyOrder(void);
    bool         bVerifyNextPrev(void);
    bool         bVerifyRowRefs(void);
    bool         bVerifySkipListEntry(bool bShowKeys = false);
    bool         bVerifySkipListLevels(bool bShowKeys = false);
    void         vVerifyFault(void);

    public:
    Footer* aFooter(void);
  };
  
  EntryBuffer* _pEntryBuffer(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
