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
  
  class EntryBuffer : public Object
  {
  protected:
  
    //BitArray m_xBitArray;
  
    eBD   m_eDurability; // buffer durability
    bool  m_bIsLast;
    i8    m_nEntries;    // number of entries in this buffer
  
    EntryBuffer* m_pPrevBuffer;  // prev buffer
    EntryBuffer* m_pNextBuffer;  // next buffer
    EntryBuffer* m_pHigherLevel; // higher level buffer that has entries
                                 // that point to this buffer
  
    Index*       m_pIndex;       // index this buffer belongs to
    i8           m_nBufferLevel; // index buffer level
    i8           m_nPartitionNo; // partition no this buffer belongs to
  
    WspFxd<C_SIZE_LARGE_OBJECT - C_1KB*4> m_xWsp;
  
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

  
    eBD  eDurability(void);
    void vDurability(eBD eDurability);
  
    i8   nEntries(void);
  
    EntryBuffer* pNextBuffer(void);
    void         vNextBuffer(EntryBuffer* pNextBuffer);
  
    EntryBuffer* pPrevBuffer(void);
    void         vPrevBuffer(EntryBuffer* pPrevBuffer);
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // what was RowBufer
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
    
    Entry*     pAllotEntry(i8 nBytes);
    byte*      aDataArea(void);
    Entry*     pFirstRow(void);
    i8         nFreeBytes(void);
    bool       bInsertRow(EntWrk& xEntRow, Entry** ppRow);
    bool       bInsertRow(Entry* pOldRow, Entry** ppRow);

    void       vIsLast(bool bIsLast);
    bool       bIsLast(void);
    Entry*     pNextRow(Entry* pCurrRow);
    void       vPartitionNo(i8 nPartitionNo);
    i8         nPartitionNo(void);
    void       vRowCount(void);
    i8         nUsedBytes(void);
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // what was IndexBuffer
  
    void         vAnalyze(s48K& sAnalysis, bool bDetailed, i8& iBuffers, i8& iEntries);
    Entry*       pAppendEntry(EntWrk& xEntWrk,EntryVec& xPriorVec);
    bool         bAppendEntry(Entry* pRowEntry, Entry** ppNewEntry);
    bool         bAppendEntry(i8 iTxId, Stz* pRow, i8 iLength);
    bool         bAppendRow(i8 iTxId, Stz* pRow, i8 iLength);
    i8           nBufferLevel(void);
    void         vBufferCount(i8& nBufferCount);
    i8           iDecile(void);
    void         vDump(void);
    Entry*       pFirstEntry(void);
    Entry*       pFirstEntryActual(void);
    bool         bHasSpace(i8 nBytes);
    Entry*       pHead(void);
    void         vHigherLevel(EntryBuffer* pHigherLevel);
    EntryBuffer* pHigherLevel(void);
    void         vIndex(Index* pIndex);
    Index*       pIndex(void);
    bool         bInsertEntry(i8 iTxId, Stz* pRow, i8 iLength);
    bool         bInsert(EntWrk& xEntWrk);
    EntryBuffer* pNewNextBuffer(void);
    Entry*       pNextEntry(Entry* pEntry);
    EntryBuffer* pPositionLevel(KeyWrk& xKeyWrk, i8 nKeyCols, eJC eJuxtaCompare, i8 iLevel);
    Entry*       pPosition(KeyWrk& xKeyWrk, i8 nKeyCols, eJC eJuxtaCompare);
    void         vPriorVecHead(EntryVec& xPriorVec);
    bool         bRemove(Key* aKey);
    void         vShow(void);
    EntryBuffer* pSplitBuffer(void);
    void         vTruncate(void);
    bool         bVerify(void);
    
    Entry*       pAllotEntryBytes(i8 nBytes);
  
    private:
    
    void         vAdjustOffsets(Entry* pEntry, i8 nAdjustAmount);
    Entry*       pAllotEntry(i8 nBytes, i8 nLevels, Entry* aAfterEntry);
    void         vAllotHead(void);
    i8           iCopyEntries(EntryBuffer* pFromBuffer, Entry* aFromEntry);
    void         vDumpPrior(Entry* aPriorVec[]);
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
  
    Footer* aFooter(void);
  };
  
  EntryBuffer* _pEntryBuffer(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
