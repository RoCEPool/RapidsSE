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
  
  static const EntryBuffer_v1* C_ENTRYBUFFER_MARKER = (EntryBuffer_v1*)0xFFFFFFFFFFFFFFFF;
  
  class EntryBuffer_v1 : public ItemBuffer
  {
  protected:
  
    //BitArray m_xBitArray;
  
    eBD   m_eDurability; // buffer durability
    bool  m_bIsLast;
  
    EntryBuffer_v1* m_pHigherLevel; // higher level buffer that has entries
                                 // that point to this buffer
    EntryBuffer_v1* m_pNextBuffer;  // next buffer
    EntryBuffer_v1* m_pPrevBuffer;  // prev buffer
  
    Index*       m_pIndex;       // index this buffer belongs to
    i8           m_nBufferLevel; // index buffer level
    i8           m_nPartitionNo; // partition no this buffer belongs to
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    EntryBuffer_v1(void);
    EntryBuffer_v1(i8 iDataBytes);
  
    EntryBuffer_v1(eCN eClassNo);
    EntryBuffer_v1(Index* pIndex, i8 nLevel, i8 nPartitionNo);
    ~EntryBuffer_v1(void);

    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
  
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // simple member functions
  
    eBD  eDurability(void);
    void vDurability(eBD eDurability);
  
    void         vIsLast(bool bIsLast);
    bool         bIsLast(void);
  
    void         vHigherLevel(EntryBuffer_v1* pHigherLevel);
    EntryBuffer_v1* pHigherLevel(void);
  
    EntryBuffer_v1* pNextBuffer(void);
    void         vNextBuffer(EntryBuffer_v1* pNextBuffer);
  
    EntryBuffer_v1* pPrevBuffer(void);
    void         vPrevBuffer(EntryBuffer_v1* pPrevBuffer);
 
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
    Entry*       pAppendEntry(EntWrk_v1& xEntWrk,EntryVec& xPriorVec);
    bool         bAppendEntry(Entry* pRowEntry, Entry** ppNewEntry);
    bool         bAppendEntry(i8 iTxId, Stz* pRow, i8 iLength);
    bool         bAppendRow(i8 iTxId, Stz* pRow, i8 iLength);
    void         vBufferCount(i8& nBufferCount);
    i8           iDecile(void);
    void         vDump(void);
    Entry*       pFirstEntry(void);
    bool         bHasSpace(i8 nBytes);
    Entry*       pHeadEntry(void);
    bool         bInsert(EntWrk_v1& xEntWrk);
    bool         bInsertEntry(i8 iTxId, Stz* pRow, i8 iLength);
    bool         bInsertRow(EntWrk_v1& xEntRow, Entry** ppRow);
    bool         bInsertRow(Entry* pOldRow, Entry** ppRow);
    EntryBuffer_v1* pNewNextBuffer(void);
    Entry*       pNextEntry(Entry* pEntry);
    Entry*       pPosition(KeyWrk& xKeyWrk, i8 nKeyCols, eJC eJuxtaCompare);
    Entry*       pPosition(KeyWrk &xKeyWrk, eRO eRelOp);
    EntryBuffer_v1* pPositionLevel(KeyWrk& xKeyWrk, i8 nKeyCols, eJC eJuxtaCompare, i8 iLevel);
    void         vPriorVecHead(EntryVec& xPriorVec);
    bool         bRemove(Key* aKey);
    void         vRowCount(void);
    void         vShow(void);
    EntryBuffer_v1* pSplitBuffer(void);
    void         vTruncate(void);
    bool         bVerify(void);
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    private:
    
    void         vAdjustOffsets(Entry* pEntry, i8 nAdjustAmount);
    Entry*       pAllotEntryAfter(i8 nBytes, i8 nLevels, Entry* aAfterEntry);
    void         vAllotHeadEntry(void);
    i8           iCopyEntries(EntryBuffer_v1* pFromBuffer, Entry* aFromEntry);
    void         vDumpPriorVec(Entry* aPriorVec[]);
    void         vFixHigherLevel(Entry* aNewEntry);
    void         vFixPriorOffset(EntryVec* pPriorVec, Entry* aNewEntry);
    Entry*       pGetEntry(i8 iEntryNo);
    void         vMoveLeft(void* aDst, i8 nBytes);
    void         vMoveRight(void* aSrc, i8 nBytes);
    Entry*       pPosition(Key* aGivenKey, i8 nKeyCols,
                           eJC eJuxtaCompare, EntryVec* pPriorVec,
                           bool& bIsEqual, bool bIsQuery);
    void         vPositionPrior(Key* aFindKey, EntryVec* pPriorVec);
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
  
  EntryBuffer_v1* _pEntryBuffer_v1(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
