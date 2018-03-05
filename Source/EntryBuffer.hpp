/*============================================================================*/
/* EntryBuffer.hpp -- Entry Buffer                                            */
/*============================================================================*/
#pragma once

namespace DBobjects
{
  class Index;
  class Table;
  class FreeEntry;
  
  enum eBD // Buffer Durability
  {
    /* 0 */ BD_NONE=0,
    /* 1 */ BD_TEMPORARY,
    /* 2 */ BD_PERMANENT,
    /* 3 */ BD_ENUMS
  };
  
  static Stz* aBD[]
  {
    /* 0 */ "NONE"_Stz,
    /* 1 */ "TEMPORARY"_Stz,
    /* 2 */ "PERMANENT"_Stz,
    /* 3 */ "ENUMS"_Stz,
  };
  
  static const i8 C_ENTRIES_0 = C_8KB;
  static const i4 C_ENTRY_NONE = C_16KB;
  static const EntryBuffer* C_ENTRYBUFFER_MARKER = (EntryBuffer*)0xFFFFFFFFFFFFFFFF;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class EntryBuffer : public ItemBuffer
  {
  protected:
  
    eBD   m_eDurability; // buffer durability
    bool  m_bIsLast;     // is this the last buffer in sequence
  
    EntryBuffer* m_pHigherBuffer; // higher level buffer that has entries
                                  // that point to this buffer
    EntryBuffer* m_pNextBuffer;   // next buffer
    EntryBuffer* m_pPrevBuffer;   // prev buffer
  
    Index*  m_pIndex;       // index this buffer belongs to
    i2      m_nPartitionNo; // partition no this buffer belongs to
    i8      m_iBufferNo;    // this EntryBuffer's buffer number 
    i2      m_iBufferLevel; // index buffer level
  
    i4*     m_pEntryOffset;     // array of entry offsets
    i4      m_i_FreeOffsetHead; // head of free entry offset chain
  
    Entry*     m_pHeadEntry;  // head entry
    Entry*     m_pLastEntry;  // last entry in buffer
    FreeEntry* m_pFirstFree;  // head of free entry list
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    EntryBuffer(void);
    ~EntryBuffer(void);

    void vAssertEntryBuffer(void);
    bool bValidEntryBuffer(void);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // simple member functions
  
    void         vBufferLevel(i8 iBufferLevel);
    i8           iBufferLevel(void);
    eBD          eDurability(void);
    void         vDurability(eBD eDurability);
    FreeEntry*   pFirstFree(void);
    Entry*       pHeadEntry(void);
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
    i8           iDecile(void);
    void         vDeleteEntry(Entry* pEntry);
    void         vDump(void);
    Entry*       pFirstEntry(void);
    Entry*       pGetEntryNo(i8 i_Entry);
    static Entry* pInsertKeyTree(KeyWrk& xKeyWrk,
                                 EntryBuffer** ppRootKeyBuffer);
    bool         bInsertRow(Entry* pOldRow, Entry** ppRow);
    Entry*       pNextEntry(Entry* pEntry, i8 i_Offset);
  
    Entry*       pPositionQuery(Key* aGivenKey, eRO eRelOp);
    Entry*       pPositionQuery(Key* aGivenKey, i8 nKeyCols, eJC eJuxtaCompare);
    EntryBuffer* pPositionLevel(Key* aGivenKey, i8 nKeyCols, eJC eJuxtaCompare, i8 iLevel);
    
    void         vRestore(void);
    void         vShow(void);
    void         vShowShort(void);
    void         vShowTree(bool bDumpBuffer=false);
    void         vTruncate(void);
    bool         bVerify(void);
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    private:
    
    Entry*       pAllotEntry(i8 nBytes);
    NEO*         aAllotNEO(i8 nBytes);
    void         vAddFreeList(Entry* pEntry);
    void         vCompact(void);
    void         vCutEntry(Entry* pEntry, Entry* pPriorVec[]);
    void         vDeleteUpwards(Key* aDeleteKey, EntryBuffer** ppRootKeyBuffer);
    void         vDumpPriorVec(Entry* pPriorVec[]);
    Item*        pFirstItem(void);
    void         vFreeChains(void);
    Entry*       pGetEntryOffset(i8 iOffset);
    Entry*       pGetEntrySplit(i8 iSplitEntryNo);
    i4           iGetFreeEntry(void);
    void         vHandleLeftInsert(Entry* pInsertedKeyEntry,
                                   Entry* pOriginalFirstEntry,
                                   EntryBuffer** ppRootKeyBuffer);
    bool         bHasSpace(i8 nBytes);
    void         vHeadEntryNew(void);
    void         vHeadEntryRestore(void);
    EntryBuffer* pInsertableBuffer(KeyWrk& xKeyWrk,
                                   EntryBuffer** ppRootKeyBuffer);
    Entry*       pInsertKey(KeyWrk& xKeyWrk,
                            i8 iTxId,
                            Id xIdx,
                            Key* pKey, i8
                            iKeyLength,
                            EntryBuffer** ppRootKeyBuffer,
                            i8 iRefBufNo,
                            EntryBuffer* pToBuffer);
    Entry*       pInsertKeyAbs(KeyWrk& xKeyWrk,
                               EntryBuffer** ppRootKeyBuffer,
                               EntryBuffer* pToBuffer);
    Entry*       pInsertKeyBuffer(KeyWrk& xKeyWrk,
                                  EntryBuffer** ppRootKeyBuffer,
                                  EntryBuffer* pToBuffer);
    Entry*       pInsertKeyUpwards(KeyWrk& xKeyWrk,
                                   EntryBuffer** ppRootKeyBuffer,
                                   EntryBuffer* pToBuffer);
    void         vItemCountRestore(void);
    bool         bKeyBeforeBuffer(Key* aKey);
    EntryBuffer* pNewHigherBuffer(void);
    EntryBuffer* pNewInsertBuffer(Key* aInsKey, i8 nEntryPlusOffsets,
                                  EntryBuffer** ppRootKeyBuffer);
    EntryBuffer* pNewNextBuffer(void);
    Item*        pNextItem(Item* pItem);
    EntryBuffer* pPositionInsert(Key* aInsKey);
    Entry*       pPositionPrior(Key* aFindKey);
    void         vPositionPriorL0(Key* aFindKey, Entry** pPriorVec,
                                Entry* pFindEntry=nullptr);
    void         vPositionPriorLn(Key* aFindKey, Entry** pPriorVec,
                                  i8 iBufferNo);
    void         vPositionPriorLnAfter(Key* aFindKey, Entry** pPriorVec,
                                       EntryBuffer* pToBuffer);
    EntryBuffer* pSplit(EntryBuffer** ppRootKeyBuffer);
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // verify functions
  
    void         vQuickVerify(Entry* pSeeEntry=nullptr);
    bool         bQuickVerify(Entry* pNotSeeEntry);
  
    bool         bVerifyL0Hidden(void);
    bool         bVerifyL0Other(void);
    bool         bVerifyLnAny(void);

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
    void         vVerifyFault(void);

  
    public:
    Footer* aFooter(void);
  
    static void vUnitTest_b(Table* pTable);
    static void vUnitTest_c(Table* pTable);
  };
  
  EntryBuffer* _pEntryBuffer(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
