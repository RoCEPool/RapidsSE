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
    i2      m_nBufferLevel; // index buffer level
  
    i4*     m_pEntryOffset;    // array of entry offsets
    i4      m_iFreeOffsetHead; // head of free entry offset chain
  
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
    EntryBuffer(i8 iDataBytes);
    EntryBuffer(Index* pIndex, i8 nLevel, i8 nPartitionNo);
    ~EntryBuffer(void);

    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // simple member functions
  
    void         vBufferLevel(i8 nBufferLevel);
    i8           nBufferLevel(void);
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
    Entry*       pAppendEntry(EntWrk& xEntWrk,EntryVec& xPriorVec);
    bool         bAppendEntry(Entry* pRowEntry, Entry** ppNewEntry);
    Entry*       pAppendRow(i8 iTxId, Stz* pRow, i8 iRowLength,
                            EntryBuffer** ppLastRowBuffer=nullptr);
    void         vBufferCount(i8& nBufferCount);
    void         vCompact(void);
    i8           iDecile(void);
    void         vDeleteEntry(Entry* pEntry);
    void         vDeleteEntry(Key* aKey);
    void         vDump(void);
    Entry*       pFirstEntry(void);
    Entry*       pGetEntryNo(i8 iEntryNo);
    bool         bHasSpace(i8 nBytes);
    bool         bInsert(EntWrk& xEntWrk);
    Entry*       pInsertKey(i8 iTxId, Id xIdx, Key* pKey, i8 iKeyLength,
                            EntryBuffer** ppRootKeyBuffer,
                            i8 iRefBufNo=0);
    static Entry* pInsertKeyTree_c(i8 iTxId, Id xIdx,
                                   Key* pKey, i8 iKeyLength,
                                   EntryBuffer** ppRootKeyBuffer);
    Entry*       pInsertKeyUpwards_c(i8 iTxId, Id xIdx,
                                     Key* pKey, i8 iKeyLength,
                                     i8 nOffsets,
                                     EntryBuffer** ppRootKeyBuffer,
                                     i8 iRefBufNo=0);
    Entry*       pInsertKeyBuffer_c(i8 iTxId, Id xIdx,
                                    Key* pKey, i8 iKeyLength,
                                    i8 nOffsets,
                                    EntryBuffer** ppRootKeyBuffer,
                                    i8 iRefBufNo=0);
    bool         bInsertRow(Entry* pOldRow, Entry** ppRow);
    EntryBuffer* pNewHigherBuffer(void);
    EntryBuffer* pNewInsertBuffer(Key* aInsKey, i8 nEntryPlusOffsets,
                                  EntryBuffer** ppRootKeyBuffer);
    EntryBuffer* pNewNextBuffer(void);
    Entry*       pNextEntry(Entry* pEntry, i8 nOffsetNo);
  
    EntryBuffer* pPositionInsert(Key* aInsKey);
    Entry*       pPositionQuery(Key* aGivenKey, eRO eRelOp);
    Entry*       pPositionQuery(Key* aGivenKey, i8 nKeyCols, eJC eJuxtaCompare);
    EntryBuffer* pPositionLevel(Key* aGivenKey, i8 nKeyCols, eJC eJuxtaCompare, i8 iLevel);
    
    void         vPriorVecHead(EntryVec& xPriorVec);
    void         vRestore(void);
    void         vShow(void);
    void         vShowShort(void);
    void         vShowTree(bool bDumpBuffer=false);
    EntryBuffer* pSplit(EntryBuffer** ppRootKeyBuffer);
    void         vTruncate(void);
    bool         bVerify(void);
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    private:
    
    Entry*       pAllotEntry(i8 nBytes);
    NEO*         aAllotNEO(i8 nBytes);
    void         vDeleteUpwards(Key* aDeleteKey,
                                EntryBuffer** ppRootKeyBuffer=nullptr);
    void         vDumpPriorVec(Entry* pPriorVec[]);
    Item*        pFirstItem(void);
    void         vFreeChains(void);
    Entry*       pGetEntryOffset(i8 iOffset);
    Entry*       pGetEntrySplit(i8 iSplitEntryNo);
    i4           iGetFreeEntry(void);
    void         vHeadEntryNew(void);
    void         vHeadEntryRestore(void);
    EntryBuffer* pInsertableBuffer(Key* aInsKey, i8 nEntryPlusOffsets,
                                   EntryBuffer** ppRootKeyBuffer);
    void         vItemCountRestore(void);
    bool         bKeyBeforeBuffer(Key* aKey);
    Item*        pNextItem(Item* pItem);
    Entry*       pPositionPrior(Key* aFindKey);
    void         vPositionPriorL0(Key* aFindKey, Entry** pPriorVec,
                                Entry* pFindEntry=nullptr);
    void         vPositionPriorLn(Key* aFindKey, Entry** pPriorVec,
                                  i8 iBufferNo=0);
    void         vPositionPriorLnAfter(Key* aFindKey, Entry** pPriorVec,
                                       i8 iBufferNo);
    void         vReplaceKey(Entry* pWasEntry, Entry* pWithEntry,i8 iBufferNo,
                             EntryBuffer** ppRootKeyBuffer=nullptr);
  
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
