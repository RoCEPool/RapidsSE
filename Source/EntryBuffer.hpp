/*============================================================================*/
/* EntryBuffer.hpp -- Entry Buffer                                            */
/*============================================================================*/
#pragma once

namespace DBobjects
{
  class Index;
  class Table;
  class EntryBuffer;
  class FreeEntry;
  
  static const i8 C_ENTRIES_L0 = C_8KB;
  static const i4 C_ENTRY_NONE = C_16KB;
  static const EntryBuffer* C_ENTRYBUFFER_MARKER = (EntryBuffer*)0xFFFFFFFFFFFFFFFF;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class EntryBuffer : public ItemBuffer
  {
  friend class IndexTree;

  protected:
  
    bool  m_bIsLast;     // is this the last buffer in sequence
  
    EntryBuffer* m_pHigherBuffer; // higher level buffer that has entries
                                  // that point to this buffer
    EntryBuffer* m_pNextBuffer;   // next buffer
    EntryBuffer* m_pPrevBuffer;   // prev buffer
  
    Index*  m_pIndex;       // index this buffer belongs to
    i2      m_nPartitionNo; // partition no this buffer belongs to
    i8      m_iBufferNo;    // this EntryBuffer's buffer number 
    i2      m_iBufferLevel; // index buffer level
  
    i4*     m_aEntryOffsets;    // array of row entry offsets
    i4      m_i_FreeOffsetChain; // head of free entry offset chain
  
    Entry*     m_pHeadEntry;  // head entry
    Entry*     m_pLastEntry;  // last entry in buffer
    FreeEntry* m_pFreeEntryList;  // head of free entry list
  
    Entry*     m_pLazyChain;  // first lazy entry
  
    Since      m_oWhenNewed;  // when was this buffer created?
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    EntryBuffer(void);
    ~EntryBuffer(void) noexcept(false);

    void vAssertEntryBuffer(void);
    bool bValidEntryBuffer(void);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // simple member functions
  
    i8           iAgeInSeconds(void);
    void         vBufferLevel(i8 iBufferLevel);
    i8           iBufferLevel(void);
    EntryBuffer* pHigherBuffer(void);
    void         vHigherBuffer(EntryBuffer* pHigherLevel);
    void         vIndex(Index* pIndex);
    Index*       pIndex(void);
    void         vIsLast(bool bIsLast);
    bool         bIsLast(void);
    EntryBuffer* pNextBuffer(void);
    void         vNextBuffer(EntryBuffer* pNextBuffer);
    void         vPartitionNo(i8 nPartitionNo);
    i8           nPartitionNo(void);
    EntryBuffer* pPrevBuffer(void);
    void         vPrevBuffer(EntryBuffer* pPrevBuffer);
 
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // complex functions
  
    void         vAnalyze(s48K& sAnalysis, bool bDetailed, i8& iBuffers, i8& iEntries);
    Entry*       pAppendRow(RowWrk& xRowWrk);
    void         vBufferCount(i8& nBufferCount);
    void         vBuildSkipListSorted(void);
    i8           iDecile(void);
    void         vDeleteEntry(Entry** ppEntry);
    bool         bDeleteKey(Key* aDeleteKey, EntryBuffer* pToBuffer);
    void         vDump(void);
    EntryBuffer* pFirstChildBuffer(void);
    Entry*       pFirstEntry(void);
    Entry*       pGetEntryRowNo(i8 i_RowNo);
    bool         bHasSpace(i8 nBytes);
    Entry*       pInsertKeyAbs(KeyWrk& xKeyWrk);
    Entry*       pInsertKeyAbsL0(KeyWrk& xKeyWrk, Entry** ppPriorEntry);
    static Entry* pInsertKeyTree(KeyWrk& xKeyWrk,
                                 EntryBuffer** ppRootKeyBuffer);
    Entry*       pLastEntry(void);
    void         vLazyBuild(void);
    i8           iLengthList(void);
    i8           iLengthSubList(void);
    Entry*       pNextEntry(Entry* pEntry, i8 i_Offset);
  
    Entry*       pPositionQuery(Key* aGivenKey, eRO eRelOp);
    Entry*       pPositionQuery(Key* aGivenKey, i8 nKeyCols, eJC eJuxtaCompare);
    EntryBuffer* pPositionLevel(Key* aGivenKey, i8 nKeyCols, eJC eJuxtaCompare, i8 iLevel);
    
    void         vReset(void);
    void         vRestore(void);
    void         vRestoreIndex(Index* pIndex);
    void         vShow(Entry* pMarkEntry = nullptr);
    void         vShowShort(void);
    void         vShowSubList(void);
    void         vShowTree(bool bDumpBuffer=false);
    void         vSpliceAfter(EntryBuffer* pSplicedBuffer);
    void         vTruncate(void);
    EntryBuffer* pUnlinkSubList(void);
    bool         bVerify(void);
    Since&       oWhenNewed(void);
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    private:
    
    Entry*       pAllotEntry(i8 nBytes);
    NEO*         aAllotNEO(i8 nBytes);
    void         vAddFreeList(Entry* pEntry);
    void         vBuildOffsetL0(void);
    void         vBuildSkipList(void);
    void         vCompact(void);
    void         vCompactLeft(void);
    void         vCompactLeft2(void);
    void         vCutEntry(Entry* pEntry, Entry* pPriorVec[]);
    void         vDeleteUpwards(Key* aDeleteKey,
                                EntryBuffer** ppRootKeyBuffer);
    void         vDeleteUpwards(Key* aDeleteKey, EntryBuffer* pToBuffer,
                                EntryBuffer** ppRootKeyBuffer);
    void         vDumpPriorVec(Entry* pPriorVec[]);
    FreeEntry*   pFirstFree(void);
    Item*        pFirstItem(void);
    Entry*       pGetEntryOffset(i8 iOffset);
    Entry*       pGetEntrySplit(i8 iSplitEntryNo, Entry** ppPriorEntry);
    i4           iGetFreeRowEntry(void);
    void         vHandleFirstInsert(Entry* pInsertedKeyEntry,
                                    Entry* pOriginalFirstEntry,
                                    EntryBuffer** ppRootKeyBuffer);
    void         vHandleLeftInsert(Entry* pInsertedKeyEntry,
                                   Entry* pOriginalFirstEntry,
                                   EntryBuffer** ppRootKeyBuffer);
    Entry*       pHeadEntry(void);
    void         vHeadEntryNew(void);
    void         vHeadEntryRestore(void);
    void         vInitFreeChain(void);
    EntryBuffer* pInsertableBuffer(KeyWrk& xKeyWrk,
                                   EntryBuffer** ppRootKeyBuffer);
    Entry*       pInsertKeyAbsLazy(KeyWrk& xKeyWrk);
    void         vInsertKeySkip(Entry* pKeyEntry);
    Entry*       pInsertKeyBuffer(KeyWrk& xKeyWrk,
                                  EntryBuffer** ppRootKeyBuffer);
    Entry*       pInsertKeyUpwards(KeyWrk& xKeyWrk,
                                   EntryBuffer** ppRootKeyBuffer);
    void         vItemCountRestore(void);
    bool         bKeyBeforeBuffer(Key* aKey);
    FreeEntry*   pMakeFreeEntry(Entry* pEntry);
    EntryBuffer* pNewHigherBuffer(void);
    EntryBuffer* pNewInsertBuffer(Key* aInsKey, i8 nEntryPlusOffsets,
                                  EntryBuffer** ppRootKeyBuffer);
    EntryBuffer* pNewNextBuffer(void);
    Item*        pNextItem(Item* pItem);
    Entry*       pPopLazyChain(void);
    EntryBuffer* pPositionPriorBuffer(Key* aFindKey);
    Entry*       pPositionPriorEntry(Key* aFindKey);
    void         vPositionPriorVecEntry(Key* aFindKey, Entry** pPriorVec,
                                        Entry* pFindEntry);
    void         vPositionPriorVecBuffer(Key* aFindKey, Entry** pPriorVec,
                                         EntryBuffer* pToBuffer);
    void         vPositionPriorVecBuffer2(Key* aFindKey, Entry** pPriorVec,
                                          EntryBuffer* pToBuffer);
    void         vPositionPriorVecBuffer3(Key* aFindKey, Entry** pPriorVec,
                                          EntryBuffer* pToBuffer);
    EntryBuffer* pSplit(void);
    EntryBuffer* pSplit2(void);
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // verify functions
  
    bool         bCheckPriorVec(Entry* pPriorVec[]);
    void         vNumberEntries(void);
  
    void         vQuickVerify(Entry* pSeeEntry=nullptr);
    bool         bQuickVerify(Entry* pNotSeeEntry);
  
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
