/*============================================================================*/
/* EntBuffer.hpp -- Ent Buffer                                            */
/*============================================================================*/
#pragma once

#define USE_EB_LAZY_CHAIN
//#define USE_EB_WATCH

namespace MetaData
{
  static const bool C_NOT_FINAL = false;
  static const bool C_FINAL     = true;

  class EntBuffer : public DataBuffer,
                      public MiniHeap_t<Ent>
  {
  friend class IndexTree;

  protected:
    Index*    m_pIndex;     // index this buffer belongs to
    SkipRoots m_xSkipRoots; // skip roots
    Ent*     m_aLazyChain; // first lazy entry
  
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
    static Ent* aGetNextEntry(EntBuffer** ppEntBuffer, Ent* aPrevEntry);
    static EntBuffer* pRefdEntBuffer(Ent* pItem);
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
  
    Ent* aLeadEntry(void);
    Ent* aLastEntry(void);
    void  vShowEntry(Ent* pEntry);
  
  private:
    Ent* aAllotEntry(i8 nSizeEnt_Without_NEO);
    
    ////////////////////////////////////////////////////////////////////////////
  
  public:
    void vAssertEntBuffer(void);
    bool bValidEntBuffer(void);
    
    ////////////////////////////////////////////////////////////////////////////
    // static move functions
    
    static Ent* pMoveToExactEntry(Ent* aEnt, EntWrk& xEntWrk, EntBuffer** pAEntBuffer);
    static Ent* pMoveToGreaterThanEntry(Ent* aEnt, EntWrk& xEntWrk, EntBuffer** pAEntBuffer);
    static Ent* pMoveToGreaterEqualEntry(Ent* aEnt, EntWrk& xEntWrk, EntBuffer** pAEntBuffer);
    static Ent* pMoveToPrevEntry(Ent* aEnt, EntWrk& xEntWrk, EntBuffer** pAEntBuffer);
    static Ent* pMoveToNextEntry(Ent* aEnt, EntWrk& xEntWrk, EntBuffer** pAEntBuffer);
    
    ////////////////////////////////////////////////////////////////////////////
    // key positioning functions
    
    EntBuffer* pPositionH0Buffer(EntWrk& xGivenKey);
    Ent*       pPositionPriorEntry(Ent* aFindKey);
    void       vPositionPriorVecEntry(Ent* aFindKey, Ent** aaPriorVec,
                                      Ent* aFindEntry);
    
    void       vPositionPriorVecBuffer(Ent* aFindKey, Ent** aaPriorVec,
                                       EntBuffer* pToBuffer);
    
    ////////////////////////////////////////////////////////////////////////////
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // complex functions
  
    void        vAnalyze(s48K& sAnalysis, bool bDetailed, i8& iBuffers, i8& iEntries);
    void        vBuildSkipListSorted(void);
    void        vDeleteEntry(Ent** aaEnt);
    void        vDump(void);
    EntBuffer*  pFirstChildBuffer(void);
    Ent*        aInsertKeyAbs(EntWrk& xEntWrk);
    Ent*        aInsertKeyAbsL0(EntWrk& xEntWrk, Ent** aaPriorEntry);
    static Ent* aInsertKeyTree(EntWrk& xEntWrk,
                                 EntBuffer** ppRootKeyBuffer);
    void        vLazyBuild(void);
    i8          iLengthList(void);
    i8          iLengthSubList(void);
    Ent*       aNextEntry(Ent* aEnt, i8 i_Offset);
    
    void        vReset(void);
    void        vRestore(void);
    void        vRestoreIndex(Index* pIndex);
    void        vShow(Ent* aMarkEntry = nullptr);
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
    void        vCutEntry(Ent* aEnt, Ent* aPriorVec[]);
    void        vDeleteUpwards(Ent* aDeleteKey,
                                EntBuffer** ppRootKeyBuffer);
    void        vDumpPriorVec(Ent* aPriorVec[]);
    Ent*       aGetEntryOffset(i8 iOffset);
    Ent*       aGetEntrySplit(i8 iSplitEntryNo, Ent** aaPriorEntry);
    void        vHandleFirstInsert(Ent* aInsertedKeyEntry,
                                   Ent* aOriginalFirstEntry,
                                  EntBuffer** ppRootKeyBuffer);
    void        vHandleLeftInsert(Ent* aInsertedKeyEntry,
                                   Ent* aOriginalFirstEntry,
                                   EntBuffer** ppRootKeyBuffer);
    Ent*       aSkipRoots(void);
    EntBuffer* pInsertableBuffer(EntWrk& xEntWrk,
                                   EntBuffer** ppRootKeyBuffer);
    Ent*       aInsertKeyAbsLazy(EntWrk& xEntWrk);
    void        vInsertKeySkip(Ent* aKeyEntry);
    Ent*       aInsertKeyBuffer(EntWrk& xEntWrk,
                                  EntBuffer** ppRootKeyBuffer);
    Ent*       aInsertKeyUpwards(EntWrk& xEntWrk,
                                   EntBuffer** ppRootKeyBuffer);
    bool         bKeyBeforeBuffer(EntWrk& xKey);
    FreeItem*    aMakeFreeEntry(Ent* aEnt);
    EntBuffer* pNewHigherBuffer(void);
    EntBuffer* pNewInsertBuffer(EntWrk& xInsKey, i8 nEntryPlusOffsets,
                                  EntBuffer** ppRootKeyBuffer);
    EntBuffer* pNewNextBuffer(void);
    Ent*       aPopLazyChain(void);

    EntBuffer* pSplitBuffer(void);
  
    NEO*   aAllotNEO(i8 nBytes);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // verify functions
  
    bool         bCheckPriorVec(Ent* aPriorVec[]);
    void         vNumberEntries(void);
  
    void         vQuickVerify(Ent* aSeeEntry=nullptr);
    bool         bQuickVerify(Ent* aNotSeeEntry);
  
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
