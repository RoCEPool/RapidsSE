/*============================================================================*/
/* EntryBuffer_v2.hpp -- Entry Buffer (version 2)                             */
/*============================================================================*/
#pragma once

namespace DBobjects
{
  class Index;
  class Table;
  class FreeEntry;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Offset // item in the Offset2 and Offset1 arrays
  {
  public:
    i4 m_iEntryNo;   // which entry this is
    i4 m_iNextEntry; // next entry
    i4 m_iOffset;    // offset where the entry is
  };
  
  static const i8 C_ENTRIES_0_v2 = C_8KB;
  static const i8 C_ENTRIES_1_v2 = C_1KB;
  static const i8 C_ENTRIES_2_v2 = 128;
  
  static const i4 C_ENTRY_NONE_v2 = C_16KB;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class EntryBuffer_v2 : public ItemBuffer
  {
  protected:
  
    eBD   m_eDurability; // buffer durability
    bool  m_bIsLast;     // is this the last buffer in
  
    EntryBuffer_v2* m_pHigherBuffer; // higher level buffer that has entries
                                     // that point to this buffer
    EntryBuffer_v2* m_pNextBuffer;   // next buffer
    EntryBuffer_v2* m_pPrevBuffer;   // prev buffer
  
    Index*  m_pIndex;       // index this buffer belongs to
    i2      m_nBufferLevel; // index buffer level
    i2      m_nPartitionNo; // partition no this buffer belongs to
  
    Offset* m_pEntryOffset2; // array of level 2 offsets
    Offset* m_pEntryOffset1; // array of level 1 offsets
    Offset* m_pEntryOffset0; // array of level 0 offsets
  
    i4      m_nEntryOffset2; // number of level 2 offsets
    i4      m_nEntryOffset1; // number of level 1 offsets
  
    i4      m_iHeadEntry2;   // head of entry free chain for L2 offsets
    i4      m_iHeadEntry1;   // head of entry free chain for L1 offsets
    i4      m_iHeadEntry0;   // head of entry free chain for L0 offsets
  
    i4      m_iHeadFree2;    // head of entry free chain for L2 offsets
    i4      m_iHeadFree1;    // head of entry free chain for L1 offsets
    i4      m_iHeadFree0;    // head of entry free chain for L0 offsets
  
    Entry*  m_pEntries;      // where entries start
  
    i4      m_nReclaimSpace; // amount of reclaimable space in the buffer
    FreeEntry*  m_pFirstFree;// first free entry in buffer
    FreeEntry*  m_pLastFree; // last free entry in buffer
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    EntryBuffer_v2(void);
    ~EntryBuffer_v2(void);

    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // simple member functions
  
    eBD  eDurability(void);
    void vDurability(eBD eDurability);
  
    void         vIsLast(bool bIsLast);
    bool         bIsLast(void);
  
    EntryBuffer_v2* pHigherBuffer(void);
    void         vHigherBuffer(EntryBuffer_v2* pHigherLevel);
  
    EntryBuffer_v2* pNextBuffer(void);
    void         vNextBuffer(EntryBuffer_v2* pNextBuffer);
  
    EntryBuffer_v2* pPrevBuffer(void);
    void         vPrevBuffer(EntryBuffer_v2* pPrevBuffer);
 
    void         vBufferLevel(i8 nBufferLevel);
    i8           nBufferLevel(void);
  
    void         vIndex(Index* pIndex);
    Index*       pIndex(void);
  
    void         vPartitionNo(i8 nPartitionNo);
    i8           nPartitionNo(void);

    FreeEntry*   pFirstFree(void);
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // complex functions
  
    void         vAddFree(Entry* pEntry);
    Entry*       pAllotEntry(i8 nBytes);
    Entry*       pAllotEntryBytes(i8 nBytes);
    void         vAnalyze(s48K& sAnalysis, bool bDetailed, i8& iBuffers, i8& iEntries);
    Entry*       pAppendEntry(EntWrk& xEntWrk,EntryVec& xPriorVec);
    bool         bAppendEntry(Entry* pRowEntry, Entry** ppNewEntry);
    bool         bAppendEntry(i8 iTxId, Stz* pRow, i8 iLength);
    Entry*       pAppendRow_v2(EntWrk_v2& xEntWrk_v2);
    bool         bAppendRow(i8 iTxId, Stz* pRow, i8 iLength);
    void         vBufferCount(i8& nBufferCount);
    i8           iDecile(void);
    void         vDump(void);
    Entry*       pFirstEntry(void);
    bool         bHasSpace(i8 nBytes);
    Entry*       pHeadEntry(void);
    bool         bInsert(EntWrk& xEntWrk);
    bool         bInsertEntry(i8 iTxId, Stz* pRow, i8 iLength);
    Entry*       pInsertKey_v2(KeyWrk_v2& xKeyWrk);
    bool         bInsertRow(EntWrk& xEntRow, Entry** ppRow);
    bool         bInsertRow(Entry* pOldRow, Entry** ppRow);
    EntryBuffer_v2* pNewNextBuffer(void);
    Entry*       pNextEntry(Entry* pEntry);
    Entry*       pPosition(KeyWrk& xKeyWrk, i8 nKeyCols, eJC eJuxtaCompare);
    Entry*       pPosition(KeyWrk &xKeyWrk, eRO eRelOp);
    EntryBuffer_v2* pPositionLevel(KeyWrk& xKeyWrk, i8 nKeyCols, eJC eJuxtaCompare, i8 iLevel);
    void         vPriorVecHead(EntryVec& xPriorVec);
    bool         bRemove(Key* aKey);
  
    void         vRemoveFree(Entry* pEntry);
  
    void         vRowCount(void);
    void         vShow(void);
    EntryBuffer_v2* pSplitBuffer(void);
    void         vTruncate(void);
    bool         bVerify(void);
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    private:
    
    void         vAdjustOffsets(Entry* pEntry, i8 nAdjustAmount);
    Entry*       pAllotEntryAfter(i8 nBytes, i8 nLevels, Entry* aAfterEntry);
    void         vAllotHeadEntry(void);
    i8           iCopyEntries(EntryBuffer_v2* pFromBuffer, Entry* aFromEntry);
    void         vDumpPriorVec(i8* pPriorVec);
    void         vFixHigherLevel(Entry* aNewEntry);
    void         vFixPriorOffset(EntryVec* pPriorVec, Entry* aNewEntry);
    void         vFreeChains(void);
    Entry*       pGetEntry(i8 iEntryNo, ln nLevelNo);
    i4           iGetFreeEntry(void);
    void         vMoveLeft(void* aDst, i8 nBytes);
    void         vMoveRight(void* aSrc, i8 nBytes);
    Entry*       pPosition(Key* aGivenKey, i8 nKeyCols,
                           eJC eJuxtaCompare, EntryVec* pPriorVec,
                           bool& bIsEqual, bool bIsQuery);
    void         vPositionPrior_v2(Key* aFindKey, i8* pPriorVec);
    void         vPriorVec(Entry* pEntry, EntryVec* pPriorVec);
    void         vReplaceKey(KeyWrk& xOldKeyWrk, KeyWrk& xNewKeyWrk);
    void         vReplaceKeyEqual(KeyWrk& xOldKeyWrk, KeyWrk& xNewKeyWrk);
    void         vReplaceKeyLarger(KeyWrk& xOldKeyWrk, KeyWrk& xNewKeyWrk);
    void         vReplaceKeySmaller(KeyWrk& xOldKeyWrk, KeyWrk& xNewKeyWrk);
    Entry*       pRightEntry(Entry* pEntry, ln nLevelNo);
    i8           iToEndOffsetAbs(Entry* pEntry);
    i8           iToOffsetAbs(Entry* pEntry);
    i8           iToRightOffsetAbs(Entry* pEntry, on nOffsetNo);
    u8           uOffsetBetween(Entry* pEntryA, Entry* pEntryB);
  
  
    public:
    Footer* aFooter(void);
  };
  
  EntryBuffer_v2* _pEntryBuffer_v2(void* aAddr);
  
  void vEntryBuffer_v2_UnitTest(Table* pTable);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
