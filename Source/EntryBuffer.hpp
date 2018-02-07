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
    i2      m_nBufferLevel; // index buffer level
  
    i4*     m_pEntryOffset0;   // array of entry offset 0
    i4      m_nFreeOffsets;    // number of free entry offsets
    i4      m_iFreeOffsetHead; // head of free entry offset chain
  
    Entry*     m_pHeadEntry;  // head entry
    Entry*     m_pFirstEntry; // first entry in buffer
    Entry*     m_pLastEntry;  // last entry in buffer
    FreeEntry* m_pFirstFree;  // head of free entry list
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    EntryBuffer(void);
    EntryBuffer(i8 iDataBytes);
    EntryBuffer(Index* pIndex, i8 nLevel, i8 nPartitionNo);
    ~EntryBuffer(void);

    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // simple member functions
  
    eBD  eDurability(void);
    void vDurability(eBD eDurability);
  
    void         vIsLast(bool bIsLast);
    bool         bIsLast(void);
  
    EntryBuffer* pHigherBuffer(void);
    void         vHigherBuffer(EntryBuffer* pHigherLevel);
  
    EntryBuffer* pNextBuffer(void);
    void         vNextBuffer(EntryBuffer* pNextBuffer);
  
    EntryBuffer* pPrevBuffer(void);
    void         vPrevBuffer(EntryBuffer* pPrevBuffer);
 
    void         vBufferLevel(i8 nBufferLevel);
    i8           nBufferLevel(void);
  
    Entry*       pFirstEntry(void);
    FreeEntry*   pFirstFree(void);
    Entry*       pHeadEntry(void);
    
    void         vIndex(Index* pIndex);
    Index*       pIndex(void);
  
    void         vPartitionNo(i8 nPartitionNo);
    i8           nPartitionNo(void);
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // complex functions
  
    //void         vAddFree(Entry* pEntry);
    //Entry*       pAllotEntry(i8 nBytes);
    Entry*         pAllotEntryBytes(i8 nBytes);
    NEO*           aAllotNEO(i8 nBytes);
    void           vAnalyze(s48K& sAnalysis, bool bDetailed, i8& iBuffers, i8& iEntries);
    Entry*         pAppendEntry(EntWrk& xEntWrk,EntryVec& xPriorVec);
    bool           bAppendEntry(Entry* pRowEntry, Entry** ppNewEntry);
    //bool         bAppendEntry(i8 iTxId, Stz* pRow, i8 iLength);
    Entry*         pAppendRow(i8 iTxId, Stz* pRow, i8 iRowLength);
    //bool         bAppendRow(i8 iTxId, Stz* pRow, i8 iLength);
    void           vBufferCount(i8& nBufferCount);
    void           vCompactBuffer(void);
    i8             iDecile(void);
    void           vDeleteEntry(Entry* pEntry);
    void           vDump(void);
    Item*          pFirstItem(void);
    Entry*         pGetEntryRow(i8 iRowNo);
    bool           bHasSpace(i8 nBytes);
    bool           bInsert(EntWrk& xEntWrk);
    //bool         bInsertEntry(i8 iTxId, Stz* pRow, i8 iLength);
    //Entry*       pInsertKey_v3(KeyWrk& xKeyWrk);
    Entry*         pInsertKey_v3a(i8 iTxId, Key* pKey, i8 iKeyLength);
    //bool         bInsertRow(EntWrk& xEntRow, Entry** ppRow);
    bool           bInsertRow(Entry* pOldRow, Entry** ppRow);
    EntryBuffer*   pNewNextBuffer(void);
    Entry*         pNextEntry(Entry* pEntry, i8 nOffsetNo);
    Item*          pNextItem(Item* pItem);
    Entry*         pPosition(KeyWrk& xKeyWrk, i8 nKeyCols, eJC eJuxtaCompare);
    Entry*         pPosition(KeyWrk &xKeyWrk, eRO eRelOp);
    EntryBuffer*   pPositionLevel(KeyWrk& xKeyWrk, i8 nKeyCols, eJC eJuxtaCompare, i8 iLevel);
    void           vPriorVecHead(EntryVec& xPriorVec);
    //bool         bRemove(Key* aKey);
  
    //void         vRemoveFree(Entry* pEntry);
  
    void         vRowCount(void);
    void           vShow(void);
    EntryBuffer* pSplitBuffer(void);
    void         vTruncate(void);
    bool         bVerify(void);
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    private:
    
    //void         vAdjustOffsets(Entry* pEntry, i8 nAdjustAmount);
    //Entry*       pAllotEntryAfter(i8 nBytes, i8 nLevels, Entry* aAfterEntry);
    //void         vAllotHeadEntry(void);
    //i8           iCopyEntries(EntryBuffer_v3* pFromBuffer, Entry* aFromEntry);
    void           vDumpPriorVec(Entry* pPriorVec[]);
    //void         vFixHigherLevel(Entry* aNewEntry);
    //void         vFixPriorOffset(EntryVec* pPriorVec, Entry* aNewEntry);
    void           vFreeChains(void);
    Entry*         pGetEntry(i8 iOffset);
    Entry*         pGetEntry(i8 iEntryNo, ln nLevelNo);
    i4             iGetFreeEntry(void);
    Entry*         pGetSplitEntry(i8 iSplitEntryNo);
    void           vHeadEntry(void);
    //void         vMoveLeft(void* aDst, i8 nBytes);
    //void         vMoveRight(void* aSrc, i8 nBytes);
    //Entry*       pPosition(Key* aGivenKey, i8 nKeyCols,
    //                       eJC eJuxtaCompare, EntryVec* pPriorVec,
    //                       bool& bIsEqual, bool bIsQuery);
    void           vPositionPrior_v3(Key* aFindKey, Entry** pPriorVec);
    //void         vPriorVec(Entry* pEntry, EntryVec* pPriorVec);
    //void         vReplaceKey(KeyWrk& xOldKeyWrk, KeyWrk& xNewKeyWrk);
    //void         vReplaceKeyEqual(KeyWrk& xOldKeyWrk, KeyWrk& xNewKeyWrk);
    //void         vReplaceKeyLarger(KeyWrk& xOldKeyWrk, KeyWrk& xNewKeyWrk);
    //void         vReplaceKeySmaller(KeyWrk& xOldKeyWrk, KeyWrk& xNewKeyWrk);
    //Entry*       pRightEntry(Entry* pEntry, ln nLevelNo);
    //i8           iToEndOffsetAbs(Entry* pEntry);
    i8             iToOffsetAbs(Entry* pEntry);
    //i8           iToRightOffsetAbs(Entry* pEntry, on nOffsetNo);
    //u8           uOffsetBetween(Entry* pEntryA, Entry* pEntryB);
  
  
    public:
    Footer* aFooter(void);
  };
  
  EntryBuffer* _pEntryBuffer(void* aAddr);
  
  void vEntryBuffer_v3_UnitTest(Table* pTable);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
