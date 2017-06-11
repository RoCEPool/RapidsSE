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
  
  static Str* aRD[]
  {
  /* 0 */ "NONE"_Str,
  /* 1 */ "TEMPORARY"_Str,
  /* 2 */ "PERMANENT"_Str,
  /* 3 */ "ENUMS"_Str,
  };
  
  static const i8 C_BUFFER_THRESHOLD = 100;
  
  static const EntryBuffer* C_ROWBUFFER_MARKER = (EntryBuffer*)0xFFFFFFFFFFFFFFFF;
  
  class EntryBuffer : public Buffer
  {
  protected:
    eBD   m_eDurability; // buffer durability
    i8    m_nEntries;    // number of entries in this buffer
  
    EntryBuffer* m_pPrevBuffer;  // prev buffer
    EntryBuffer* m_pNextBuffer;  // next buffer
    EntryBuffer* m_pHigherLevel; // higher level buffer that has entries
                                 // that point to this buffer
  
    Index*       m_pIndex;       // index this buffer belongs to
    i8           m_nBufferLevel; // index buffer level
  
    WspFxd<C_SIZE_LARGE_OBJECT - C_1KB> m_xWsp;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    EntryBuffer(void);
    EntryBuffer(i8 iDataBytes);
  
    EntryBuffer(eCN eClassNo);
    EntryBuffer(Index* pIndex, i8 nLevel);
    ~EntryBuffer(void);
  
    static EntryBuffer* pNewEntryBuffer(eCN eClassNo);
  
    eBD  eDurability(void);
    void vDurability(eBD eDurability);
  
    i8   nEntries(void);
  
    EntryBuffer* pNextBuffer(void);
    void         vNextBuffer(EntryBuffer* pNextBuffer);
  
    EntryBuffer* pPrevBuffer(void);
    void         vPrevBuffer(EntryBuffer* pPrevBuffer);
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // what was RowBufer
  
    static void vDump(eCN eClass, Object* pObject, Str* aTab);
    
    Entry*       pAllocRow(i8 nDataBytes);
    void       vAppendBuffer(EntryBuffer* pSrcBuffer);
    void       vCopyBuffer(EntryBuffer* pSrcBuffer);
    byte*      aDataArea(void);
    Entry*       pFirstRow(void);
    i8         nFreeBytes(void);
    bool       bInsertRow(TxRef* pTxRef, Str* pRecord, i8 iLength, Entry** ppRow);
    Entry*       pNextRow(Entry* pCurrRow);
    void       vRowCount(void);
    i8         nUsedBytes(void);
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // what was IndexBuffer
  
    void         vAnalyze(s48K& sAnalysis, bool bDetailed, i8& iBuffers, i8& iEntries);
    i8           nBufferLevel(void);
    void         vBufferCount(i8& nBufferCount);
    i8           iDecile(void);
    void         vDump(void);
    Entry*       pFirstEntry(void);
    bool         bHasSpace(i8 nBytes);
    Entry*       pHead(void);
    void         vHigherLevel(EntryBuffer* pHigherLevel);
    EntryBuffer* pHigherLevel(void);
    Index*       pIndex(void);
    bool         bInsert(EntWrk& xEntWrk);
    Entry*       pNextEntry(Entry* pEntry);
    EntryBuffer* pPositionLevel(KeyWsp& xKeyWsp, eJC eJuxtaCompare, i8 iLevel);
    Entry*       pPosition(KeyWsp& xKeyWsp, eJC eJuxtaCompare);
    bool         bRemove(Key* aKey);
    void         vShow(void);
    EntryBuffer* pSplitBuffer(void);
    void         vTruncate(void);
    bool         bVerify(void);
    
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
    Entry*       pPosition(Key* aGivenKey, eJC eJuxtaCompare, EntryVec* pPriorVec, bool& bIsEqual);
    void         vPriorVec(Entry* pEntry, EntryVec* pPriorVec);
    void         vReplace(KeyWsp& xOldKeyWsp, KeyWsp& xNewKeyWsp);
    void         vReplaceEqualKey(KeyWsp& xOldKeyWsp, KeyWsp& xNewKeyWsp);
    void         vReplaceLargerKey(KeyWsp& xOldKeyWsp, KeyWsp& xNewKeyWsp);
    void         vReplaceSmallereKey(KeyWsp& xOldKeyWsp, KeyWsp& xNewKeyWsp);
    Entry*       pRightEntry(Entry* pEntry, ln nLevelNo);
    i8           iToEndOffsetAbs(Entry* pEntry);
    i8           iToOffsetAbs(Entry* pEntry);
    i8           iToRightOffsetAbs(Entry* pEntry, on nOffsetNo);
  
  };
  
  EntryBuffer* _pEntryBuffer(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
