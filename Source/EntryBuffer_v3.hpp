/*============================================================================*/
/* EntryBuffer_v3.hpp -- Entry Buffer (version 3)                             */
/*============================================================================*/
#pragma once

namespace DBobjects
{
  class Index;
  class Table;
  class FreeEntry;
  
  static const i8 C_ENTRIES_0_V3 = C_8KB;
  static const i4 C_ENTRY_NONE_V3 = C_16KB;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class EntryBuffer_v3 : public ItemBuffer
  {
  protected:
  
    eBD   m_eDurability; // buffer durability
    bool  m_bIsLast;     // is this the last buffer in
  
    EntryBuffer_v3* m_pHigherBuffer; // higher level buffer that has entries
                                     // that point to this buffer
    EntryBuffer_v3* m_pNextBuffer;   // next buffer
    EntryBuffer_v3* m_pPrevBuffer;   // prev buffer
  
    Index*  m_pIndex;       // index this buffer belongs to
    i2      m_nBufferLevel; // index buffer level
    i2      m_nPartitionNo; // partition no this buffer belongs to
  
    i4      m_iEntryOffset0[C_ENTRIES_0_V3];    // array of entry offset 0
    
    Entry*  m_pHeadEntry;    // head entry of buffer
    Entry*  m_pFirstEntry;   // first entry in buffer
    
    FreeEntry* m_pFirstFree;       // first free space in buffer
    i4         m_iFreeOffsetHead;  // head of entry offset free chain
    
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    EntryBuffer_v3(void);
    ~EntryBuffer_v3(void);

    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // simple member functions
  
    eBD  eDurability(void);
    void vDurability(eBD eDurability);
  
    void         vIsLast(bool bIsLast);
    bool         bIsLast(void);
  
    EntryBuffer_v3* pHigherBuffer(void);
    void         vHigherBuffer(EntryBuffer_v3* pHigherLevel);
  
    EntryBuffer_v3* pNextBuffer(void);
    void         vNextBuffer(EntryBuffer_v3* pNextBuffer);
  
    EntryBuffer_v3* pPrevBuffer(void);
    void         vPrevBuffer(EntryBuffer_v3* pPrevBuffer);
 
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
    //void         vAnalyze(s48K& sAnalysis, bool bDetailed, i8& iBuffers, i8& iEntries);
    //Entry*       pAppendEntry(EntWrk& xEntWrk,EntryVec& xPriorVec);
    //bool         bAppendEntry(Entry* pRowEntry, Entry** ppNewEntry);
    //bool         bAppendEntry(i8 iTxId, Stz* pRow, i8 iLength);
    //Entry*       pAppendRow_v3(EntWrk_v2& xEntWrk_v2);
    //bool         bAppendRow(i8 iTxId, Stz* pRow, i8 iLength);
    //void         vBufferCount(i8& nBufferCount);
    //i8           iDecile(void);
    //void         vDump(void);
    
    bool         bHasSpace(i8 nBytes);

    //bool         bInsert(EntWrk& xEntWrk);
    //bool         bInsertEntry(i8 iTxId, Stz* pRow, i8 iLength);
    //Entry*       pInsertKey_v3(KeyWrk_v2& xKeyWrk);
    //bool         bInsertRow(EntWrk& xEntRow, Entry** ppRow);
    //bool         bInsertRow(Entry* pOldRow, Entry** ppRow);
    //EntryBuffer_v3* pNewNextBuffer(void);
    //Entry*       pNextEntry(Entry* pEntry);
    //Entry*       pPosition(KeyWrk& xKeyWrk, i8 nKeyCols, eJC eJuxtaCompare);
    //Entry*       pPosition(KeyWrk &xKeyWrk, eRO eRelOp);
    //EntryBuffer_v3* pPositionLevel(KeyWrk& xKeyWrk, i8 nKeyCols, eJC eJuxtaCompare, i8 iLevel);
    //void         vPriorVecHead(EntryVec& xPriorVec);
    //bool         bRemove(Key* aKey);
  
    //void         vRemoveFree(Entry* pEntry);
  
    //void         vRowCount(void);
    void         vShow(void);
    //EntryBuffer_v3* pSplitBuffer(void);
    //void         vTruncate(void);
    //bool         bVerify(void);
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    private:
    
    //void         vAdjustOffsets(Entry* pEntry, i8 nAdjustAmount);
    //Entry*       pAllotEntryAfter(i8 nBytes, i8 nLevels, Entry* aAfterEntry);
    //void         vAllotHeadEntry(void);
    //i8           iCopyEntries(EntryBuffer_v3* pFromBuffer, Entry* aFromEntry);
    //void         vDumpPriorVec(i8* pPriorVec);
    //void         vFixHigherLevel(Entry* aNewEntry);
    //void         vFixPriorOffset(EntryVec* pPriorVec, Entry* aNewEntry);
    //void         vFreeChains(void);
    //Entry*       pGetEntry(i8 iEntryNo, ln nLevelNo);
    //i4           iGetFreeEntry(void);
    //void         vMoveLeft(void* aDst, i8 nBytes);
    //void         vMoveRight(void* aSrc, i8 nBytes);
    //Entry*       pPosition(Key* aGivenKey, i8 nKeyCols,
    //                       eJC eJuxtaCompare, EntryVec* pPriorVec,
    //                       bool& bIsEqual, bool bIsQuery);
    //void         vPositionPrior_v3(Key* aFindKey, i8* pPriorVec);
    //void         vPriorVec(Entry* pEntry, EntryVec* pPriorVec);
    //void         vReplaceKey(KeyWrk& xOldKeyWrk, KeyWrk& xNewKeyWrk);
    //void         vReplaceKeyEqual(KeyWrk& xOldKeyWrk, KeyWrk& xNewKeyWrk);
    //void         vReplaceKeyLarger(KeyWrk& xOldKeyWrk, KeyWrk& xNewKeyWrk);
    //void         vReplaceKeySmaller(KeyWrk& xOldKeyWrk, KeyWrk& xNewKeyWrk);
    //Entry*       pRightEntry(Entry* pEntry, ln nLevelNo);
    //i8           iToEndOffsetAbs(Entry* pEntry);
    i8           iToOffsetAbs(Entry* pEntry);
    //i8           iToRightOffsetAbs(Entry* pEntry, on nOffsetNo);
    //u8           uOffsetBetween(Entry* pEntryA, Entry* pEntryB);
  
  
    public:
    Footer* aFooter(void);
  };
  
  EntryBuffer_v3* _pEntryBuffer_v3(void* aAddr);
  
  void vEntryBuffer_v3_UnitTest(Table* pTable);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
