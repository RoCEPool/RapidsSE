/*============================================================================*/
/* EntryBuffer.hpp -- Entry Buffer                                            */
/*============================================================================*/
#pragma once

namespace DBobjects
{
  class Index;
  
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
  
  static const i8 C_BUFFER_THRESHOLD = 100;
  
  static const EntryBuffer* C_ENTRYBUFFER_MARKER = (EntryBuffer*)0xFFFFFFFFFFFFFFFF;
  
  class EntryBuffer : public Object,
                      public ItemBuffer
  {
  protected:
  
    //BitArray m_xBitArray;
  
    eBD   m_eDurability; // buffer durability
    bool  m_bIsLast;
  
    EntryBuffer* m_pHigherLevel; // higher level buffer that has entries
                                 // that point to this buffer
    EntryBuffer* m_pNextBuffer;  // next buffer
    EntryBuffer* m_pPrevBuffer;  // prev buffer
  
    Index*       m_pIndex;       // index this buffer belongs to
    i8           m_nBufferLevel; // index buffer level
    i8           m_nPartitionNo; // partition no this buffer belongs to
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    EntryBuffer(void);
    EntryBuffer(i8 iDataBytes);
  
    EntryBuffer(eCN eClassNo);
    EntryBuffer(Index* pIndex, i8 nLevel, i8 nPartitionNo);
    ~EntryBuffer(void);

    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
  
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // simple member functions
  
    eBD  eDurability(void);
    void vDurability(eBD eDurability);
  
    void         vIsLast(bool bIsLast);
    bool         bIsLast(void);
  
    void         vHigherLevel(EntryBuffer* pHigherLevel);
    EntryBuffer* pHigherLevel(void);
  
    EntryBuffer* pNextBuffer(void);
    void         vNextBuffer(EntryBuffer* pNextBuffer);
  
    EntryBuffer* pPrevBuffer(void);
    void         vPrevBuffer(EntryBuffer* pPrevBuffer);

    i8           nBufferLevel(void);
  
    void         vIndex(Index* pIndex);
    Index*       pIndex(void);
  
    void         vPartitionNo(i8 nPartitionNo);
    i8           nPartitionNo(void);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // complex functions
  
    Entry*       pAllotEntry(i8 nBytes);
    Entry*       pAllotEntryBytes(i8 nBytes);
    void         vAnalyze(s48K& sAnalysis, bool bDetailed, i8& iBuffers, i8& iEntries);
    Entry*       pAppendEntry(EntWrk& xEntWrk,EntryVec& xPriorVec);
    bool         bAppendEntry(Entry* pRowEntry, Entry** ppNewEntry);
    bool         bAppendEntry(i8 iTxId, Stz* pRow, i8 iLength);
    bool         bAppendRow(i8 iTxId, Stz* pRow, i8 iLength);
    void         vBufferCount(i8& nBufferCount);
    i8           iDecile(void);
    void         vDump(void);
    Entry*       pFirstEntry(void);
    bool         bHasSpace(i8 nBytes);
    Entry*       pHeadEntry(void);
    bool         bInsert(EntWrk& xEntWrk);
    bool         bInsertEntry(i8 iTxId, Stz* pRow, i8 iLength);
    bool         bInsertRow(EntWrk& xEntRow, Entry** ppRow);
    bool         bInsertRow(Entry* pOldRow, Entry** ppRow);
    EntryBuffer* pNewNextBuffer(void);
    Entry*       pNextEntry(Entry* pEntry);
    Entry*       pPosition(KeyWrk& xKeyWrk, i8 nKeyCols, eJC eJuxtaCompare);
    Entry*       pPosition(KeyWrk &xKeyWrk, eRO eRelOp);
    EntryBuffer* pPositionLevel(KeyWrk& xKeyWrk, i8 nKeyCols, eJC eJuxtaCompare, i8 iLevel);
    void         vPriorVecHead(EntryVec& xPriorVec);
    bool         bRemove(Key* aKey);
    void         vRowCount(void);
    void         vShow(void);
    EntryBuffer* pSplitBuffer(void);
    void         vTruncate(void);
    bool         bVerify(void);
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    private:
    
    void         vAdjustOffsets(Entry* pEntry, i8 nAdjustAmount);
    Entry*       pAllotEntryAfter(i8 nBytes, i8 nLevels, Entry* aAfterEntry);
    void         vAllotHeadEntry(void);
    i8           iCopyEntries(EntryBuffer* pFromBuffer, Entry* aFromEntry);
    void         vDumpPriorVec(Entry* aPriorVec[]);
    void         vFixHigherLevel(Entry* aNewEntry);
    void         vFixPriorOffset(EntryVec* pPriorVec, Entry* aNewEntry);
    Entry*       pGetEntry(i8 iEntryNo);
    i8           iLevel(void);
    void         vMoveLeft(void* aDst, i8 nBytes);
    void         vMoveRight(void* aSrc, i8 nBytes);
    u8           uOffsetBetween(Entry* pEntryA, Entry* pEntryB);
    Entry*       pPosition(Key* aGivenKey, i8 nKeyCols,
                           eJC eJuxtaCompare, EntryVec* pPriorVec,
                           bool& bIsEqual, bool bIsQuery);
    void         vPriorVec(Entry* pEntry, EntryVec* pPriorVec);
    void         vReplace(KeyWrk& xOldKeyWrk, KeyWrk& xNewKeyWrk);
    void         vReplaceEqualKey(KeyWrk& xOldKeyWrk, KeyWrk& xNewKeyWrk);
    void         vReplaceLargerKey(KeyWrk& xOldKeyWrk, KeyWrk& xNewKeyWrk);
    void         vReplaceSmallereKey(KeyWrk& xOldKeyWrk, KeyWrk& xNewKeyWrk);
    Entry*       pRightEntry(Entry* pEntry, ln nLevelNo);
    i8           iToEndOffsetAbs(Entry* pEntry);
    i8           iToOffsetAbs(Entry* pEntry);
    i8           iToRightOffsetAbs(Entry* pEntry, on nOffsetNo);
  
    public:
    Footer* aFooter(void);
  };
  
  EntryBuffer* _pEntryBuffer(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
