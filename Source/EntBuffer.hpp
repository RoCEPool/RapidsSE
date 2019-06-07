/*============================================================================*/
/* EntBuffer.hpp -- Item Buffer                                            */
/*============================================================================*/
#pragma once

#define USE_EB_LAZY_CHAIN
//#define USE_EB_WATCH

namespace Data
{
  static const bool C_NOT_FINAL = false;
  static const bool C_FINAL     = true;

  class EntBuffer : public DataBuffer,
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
  
    EntBuffer(void);
    ~EntBuffer(void) noexcept(false);

    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
    static Item* aGetNextEntry(EntBuffer** ppEntBuffer, Item* aPrevEntry);
    static EntBuffer* pRefdEntBuffer(Item* pItem);
    static EntBuffer* pRefdEntBuffer(Id& xId);
  
    void    vDropWrite(void);
    void    vGrabWrite(void);
    void    vIndex(Index* pIndex);
    Index*  pIndex(void);

    ////////////////////////////////////////////////////////////////////////////
    // buffer functions
  
    void         vBufferCount(i8& nBufferCount);
    EntBuffer* pHigherBuffer(void);
    void         vHigherBuffer(EntBuffer* pHigherBuffer);
    EntBuffer* pNextBuffer(void);
    void         vNextBuffer(EntBuffer* pNextBuffer);
    EntBuffer* pPrevBuffer(void);
    void         vPrevBuffer(EntBuffer* pPrevBuffer);

    ////////////////////////////////////////////////////////////////////////////
    // entry functions
  
    Item* aLeadEntry(void);
    Item* aLastEntry(void);
    void  vShowEntry(Item* pEntry);
  
  private:
    Item* aAllotEntry(i8 nBytes_Without_Offsets);
    
    ////////////////////////////////////////////////////////////////////////////
  
  public:
    void vAssertEntBuffer(void);
    bool bValidEntBuffer(void);
    
    ////////////////////////////////////////////////////////////////////////////
    // static move functions
    
    static Item* pMoveToExactEntry(Item* aItem, KeyWrk& xKeyWrk, EntBuffer** pAEntBuffer);
    static Item* pMoveToGreaterThanEntry(Item* aItem, KeyWrk& xKeyWrk, EntBuffer** pAEntBuffer);
    static Item* pMoveToGreaterEqualEntry(Item* aItem, KeyWrk& xKeyWrk, EntBuffer** pAEntBuffer);
    static Item* pMoveToPrevEntry(Item* aItem, KeyWrk& xKeyWrk, EntBuffer** pAEntBuffer);
    static Item* pMoveToNextEntry(Item* aItem, KeyWrk& xKeyWrk, EntBuffer** pAEntBuffer);
    
    ////////////////////////////////////////////////////////////////////////////
    // key positioning functions
    
    EntBuffer* pPositionH0Buffer(KeyWrk& xGivenKey);
    Item*        pPositionPriorEntry(KeyWrk& xFindKey);
    Item*        pPositionPriorEntry(Item* aFindKey);
    void         vPositionPriorVecEntry(Item* aFindKey, Item** aaPriorVec,
                                        Item* aFindEntry);
    
    void         vPositionPriorVecBuffer(Item* aFindKey, Item** aaPriorVec,
                                         EntBuffer* pToBuffer);
    
    ////////////////////////////////////////////////////////////////////////////
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // complex functions
  
    void        vAnalyze(s48K& sAnalysis, bool bDetailed, i8& iBuffers, i8& iEntries);
    void        vBuildSkipListSorted(void);
    void        vDeleteEntry(Item** aaItem);
    void        vDump(void);
    EntBuffer* pFirstChildBuffer(void);
    Item*       aInsertKeyAbs(KeyWrk& xKeyWrk);
    Item*       aInsertKeyAbsL0(KeyWrk& xKeyWrk, Item** aaPriorEntry);
    static Item* aInsertKeyTree(KeyWrk& xKeyWrk,
                                 EntBuffer** ppRootKeyBuffer);
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
    void        vSpliceAfter(EntBuffer* pSplicedBuffer);
    void        vTruncate(void);
    EntBuffer* pUnlinkSubList(void);
    bool        bVerify(void);
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    private:
    
    void        vBuildSkipList(void);
    void        vCompact(void);
    void        vCompactLeft(void);
    void        vCutEntry(Item* aItem, Item* aPriorVec[]);
    void        vDeleteUpwards(Item* aDeleteKey,
                                EntBuffer** ppRootKeyBuffer);
    void        vDumpPriorVec(Item* aPriorVec[]);
    Item*       aGetEntryOffset(i8 iOffset);
    Item*       aGetEntrySplit(i8 iSplitEntryNo, Item** aaPriorEntry);
    void        vHandleFirstInsert(Item* aInsertedKeyEntry,
                                   Item* aOriginalFirstEntry,
                                  EntBuffer** ppRootKeyBuffer);
    void        vHandleLeftInsert(Item* aInsertedKeyEntry,
                                   Item* aOriginalFirstEntry,
                                   EntBuffer** ppRootKeyBuffer);
    Item*       aSkipRoots(void);
    EntBuffer* pInsertableBuffer(KeyWrk& xKeyWrk,
                                   EntBuffer** ppRootKeyBuffer);
    Item*       aInsertKeyAbsLazy(KeyWrk& xKeyWrk);
    void        vInsertKeySkip(Item* aKeyEntry);
    Item*       aInsertKeyBuffer(KeyWrk& xKeyWrk,
                                  EntBuffer** ppRootKeyBuffer);
    Item*       aInsertKeyUpwards(KeyWrk& xKeyWrk,
                                   EntBuffer** ppRootKeyBuffer);
    bool         bKeyBeforeBuffer(KeyWrk& xKey);
    FreeItem*    aMakeFreeEntry(Item* aItem);
    EntBuffer* pNewHigherBuffer(void);
    EntBuffer* pNewInsertBuffer(KeyWrk& xInsKey, i8 nEntryPlusOffsets,
                                  EntBuffer** ppRootKeyBuffer);
    EntBuffer* pNewNextBuffer(void);
    Item*       aPopLazyChain(void);

    EntBuffer* pSplitBuffer(void);
  
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
  
  EntBuffer* _pEntBuffer(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
