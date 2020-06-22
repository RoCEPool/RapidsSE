/*============================================================================*/
/* EntBuffer.hpp -- Ent Buffer                                                */
/*============================================================================*/
#pragma once

namespace Indexing
{
  static const bool C_NOT_FINAL    = false;
  static const bool C_FINAL        = true;
  
  class EntBuffer : public DataBuffer,
                    public MiniHeap_t<Ent>
  {
  friend class IndexTree;

  protected:
    Index*    m_pIndex;     // index this buffer belongs to
    SkipRoots m_xSkipRoots; // skip roots
    Ent*      m_aLazyChain; // first lazy entry
  
    SeeMe m_xSeeMe;
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    EntBuffer(void);
    ~EntBuffer(void) noexcept(false);

    static void vCopyLeftH0(EntBuffer** ppLeftH0, EntBuffer* pRightH0);
    static void vCopyReInitH1(EntBuffer* pH1, EntBuffer* pLeftH0);
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
    static Ent* aGetNextEntry(EntBuffer** ppEntBuffer, Ent* aPrevEntry);
    static EntBuffer* pRefdEntBuffer(Ent* pItem);
    static EntBuffer* pRefdEntBuffer(Ref& RowRef);
  
    void    vDropWrite(void);
    void    vGrabWrite(void);
    void    vIndex(Index* pIndex);
    Index*  pIndex(void);
    void    vPrintRows(void);

    ////////////////////////////////////////////////////////////////////////////
    // buffer functions
  
    void       vBufferCount(i8& nBufferCount);
    EntBuffer* pHigherBuffer(void);
    void       vHigherBuffer(EntBuffer* pHigherBuffer);
    EntBuffer* pNextBuffer(void);
    void       vNextBuffer(EntBuffer* pNextBuffer);
    EntBuffer* pPrevBuffer(void);
    void       vPrevBuffer(EntBuffer* pPrevBuffer);

    //SrtBuffer* pToSrtBuffer(i8 iKeyCol);
    
    ////////////////////////////////////////////////////////////////////////////
    // entry functions
  
    Ent* aLeadEntry(void);
    Ent* aLastEntry(void);
    void vShowEntry(Ent* pEntry);
  
  private:
    Ent* aAllotEntry(i8 nSize);
    
    ////////////////////////////////////////////////////////////////////////////
  
  public:
    void vAssertEntBuffer(void);
    bool bValidEntBuffer(void);
    
    ////////////////////////////////////////////////////////////////////////////
    // static move functions
    
    static Ent* pMoveToGreaterThanEntry(Ent* aEnt, Key* aGivenKey,
                                        EntBuffer** ppEntBuffer);
    static Ent* pMoveToGreaterEqualEntry(Ent* aEnt, Key* aGivenKey,
                                         EntBuffer** ppEntBuffer);
    static Ent* pMoveToNextEntry(Ent* aEnt,
                                 EntBuffer** ppEntBuffer);
    static Ent* pFindPrevKeyEntry(Ent* aEnt, Key* aGivenKey,
                                  EntBuffer** ppEntBuffer);
    
    ////////////////////////////////////////////////////////////////////////////
    // key positioning functions
    
    EntBuffer* pPositionH0Buffer(Key* aFindKey);
  
    Ent*       aPositionPriorEntry(Key* aFindKey);
  
    void       vPositionPriorVecBuffer(Key* aFindKey,
                                       Ent* aPriorVec[],
                                       EntBuffer* pToBuffer);
  
    void       vPositionPriorVecEntry(Key* aFindKey,
                                      Ent* aPriorVec[],
                                      Ent* aFindEntry);
  
    void       vPositionPriorVecEntry_Opt01(Key* aFindKey,
                                            Ent* aPriorVec[],
                                            Ent* aFindEntry);
  
    void       vPositionPriorVecEntry_Alka01(Key* aFindKey,
                                             Ent* aPriorVec[],
                                             Ent* aFindEntry);
  
    void       vPositionPriorVecEntry_Ray01(Key* aFindKey,
                                            Ent* aPriorVec[],
                                            Ent* aFindEntry);
  
    void       vPositionPriorVecEntry_Ray02(Key* aFindKey,
                                            Ent* aPriorVec[]);

    void       vPositionPriorVecEntry_Ray03(Key* aFindKey,
                                            Ent* aPriorVec[]);


    ////////////////////////////////////////////////////////////////////////////
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // complex functions
  
    //void        vAnalyze(s48K& sAnalysis, bool bDetailed, i8& iBuffers, i8& iEntries);
    void        vAnalyze(i8& iBuffers, i8& iEntries);
    void        vBuildSkipListSorted(void);
    void        vDeleteEntry(Ent** aaEnt);
    void        vDeleteUpwards(Ent* aDeleteEntry, EntBuffer** ppRootBuffer);
    void        vDump(void);
    EntBuffer*  pFirstChildBuffer(void);
    Ent*        aInsertEntAbs(Ent* aEntry);
    Ent*        aInsertEntAbsL0(Ent* aEntry, Ent** aaPriorEntry);
    static Ent* aInsertEntTree(Ent* aEntry, EntBuffer** ppRootBuffer);
    void        vLazyBuild(void);
    i8          iLengthList(void);
    i8          iLengthSubList(void);
    Ent*        aNextEntry(Ent* aEnt, i8 i_Offset);
    void        vPurgeUpwards(Ent* aDeleteEntry, EntBuffer** ppRootBuffer);
    void        vReset(void);
    void        vRestore(void);
    void        vRestoreIndex(Index* pIndex);
    void        vShow(Ent* aMarkEntry = nullptr);
    void        vShowShort(void);
    void        vShowSubList(void);
    void        vShowTree(bool bDumpBuffer=false);
    i8          nSkipLength(i8 i_Level);
    void        vSkipLengths(i8& iL0Length_, i8& iL1Length_, i8& iL2Length_);
    void        vSpliceAfter(EntBuffer* pSplicedBuffer);
    IrwBuffer*  pToIrwBuffer(void);
    void        vTruncate(void);
    EntBuffer*  pUnlinkSubList(void);
    bool        bVerify(void);
    bool        bVerifyKeyOrder(void);
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    private:
    
    void       vBuildSkipList(void);
    void       vCompact(void);
    void       vCompactLeft(void);
    void       vCutEntry(Ent* aEnt, Ent* aPriorVec[]);
    void       vDumpPriorVec(Ent* aPriorVec[]);
    Ent*       aGetEntryOffset(i8 iOffset);
    Ent*       aGetEntrySplit(i8 iSplitEntryNo, Ent** aaPriorEntry);
    void       vHandleFirstInsert(Ent* aInsertedKeyEntry,
                                  Ent* aOriginalFirstEntry,
                                  EntBuffer** ppRootBuffer);
    void       vHandleLeftInsert(Ent* aInsertedKeyEntry,
                                 Ent* aOriginalFirstEntry,
                                 EntBuffer** ppRootBuffer);
    Ent*       aSkipRoots(void);
    EntBuffer* pInsertableBuffer(Ent* aEntry, EntBuffer** ppRootBuffer);
    Ent*       aInsertEntAbsLazy(Ent* aEntry);
    Ent*       aInsertEntBuffer(Ent* aEntry, EntBuffer** ppRootBuffer);
    void       vInsertEntSkipList(Ent* aEntry);
    Ent*       aInsertEntUpwards(Ent* aEntry, EntBuffer** ppRootBuffer);
    bool       bKeyBeforeBuffer(Key* aKey);
    FreeItem*  aMakeFreeEntry(Ent* aEnt);
    EntBuffer* pNewHigherBuffer(void);
    EntBuffer* pNewInsertBuffer(Ent* aEntry, i8 nEntryPlusOffsets,
                                EntBuffer** ppRootBuffer);
    EntBuffer* pNewNextBuffer(void);
    Ent*       aPopLazyChain(void);

    EntBuffer* pSplitBuffer(void);
    
    //NEO*       aAllotNEO(i8 nBytes);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // verify functions
  
    bool bCheckPriorVec(Ent* aPriorVec[]);
    void vNumberEntries(void);
  
    void vQuickVerify(Ent* aSeeEntry=nullptr);
    bool bQuickVerify(Ent* aNotSeeEntry);
  
    bool bVerifyH0Other(void);
    bool bVerifyLnAny(void);

    bool bVerifyAdjacent(void);
    bool bVerifyBufferRefs(void);
    bool bVerifyFirstKeys(void);
    bool bVerifyItems(void);
    bool bVerifyKeyBufferOrder(void);
    bool bVerifyNextPrev(void);
    bool bVerifyRowRefs(void);
    bool bVerifySkipListEntry(bool bShowKeys = false);
    bool bVerifySkipListLevels(bool bShowKeys = false);
    void vVerifyFault(void);

    public:
    Footer* aFooter(void);
  };
  
  EntBuffer* _pEntBuffer(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
