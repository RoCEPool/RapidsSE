/*============================================================================*/
/* IndexBuffer.hpp -- Index Buffer (skip list variant)                        */
/*============================================================================*/
#pragma once

namespace DBobjects
{
  class Index;
  
  class IndexBuffer : public Buffer
  {
    
  private:
    Index*       m_pIndex;       // index this buffer belongs to
    i8           m_nBufferLevel; // index buffer level
    i8           m_nEntries;     // number of entries in this buffer
    IndexBuffer* m_pPrevBuffer;  // prev index buffer at this level
    IndexBuffer* m_pNextBuffer;  // next index buffer at this level
    IndexBuffer* m_pHigherLevel; // higher level buffer that points to this buffer
    
    // need to add skip list head
    // head would be array of [FLAT_LEVELS - 1]
    // array of byte offsets, not addresses
    // will need a C_NIL say -1
    // entries are in contigious key order
    // so we dont keep L1, L1 is implied
    // insert is memmove to right
    // remove is memmove to left
    //
  
    WspFxd<C_SIZE_LARGE_OBJECT - C_1KB> m_xWsp;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    IndexBuffer(Index* pIndex, i8 nLevel);
    ~IndexBuffer(void);
    
    void         vAnalyze(s48K& sAnalysis, bool bDetailed, i8& iBuffers, i8& iEntries);
    i8           nBufferLevel(void);
    void         vBufferCount(i8& nBufferCount);
    i8           iDecile(void);
    void         vDump(void);
    i8           nEntries(void);
    Entry*       pFirstEntry(void);
    bool         bHasSpace(i8 nBytes);
    Entry*       pHead(void);
    void         vHigherLevel(IndexBuffer* pHigherLevel);
    IndexBuffer* pHigherLevel(void);
    Index*       pIndex(void);
    bool         bInsert(EntWsp& xEntWsp);
    IndexBuffer* pNextBuffer(void);
    Entry*       pNextEntry(Entry* pEntry);
    IndexBuffer* pPositionLevel(KeyWsp& xKeyWsp, eJC eJuxtaCompare, i8 iLevel);
    Entry*       pPosition(KeyWsp& xKeyWsp, eJC eJuxtaCompare);
    bool         bRemove(Key* aKey);
    void         vShow(void);
    IndexBuffer* pSplitBuffer(void);
    void         vTruncate(void);
    bool         bVerify(void);
    
  private:
    void         vAdjustOffsets(Entry* pEntry, i8 nAdjustAmount);
    Entry*       pAllotEntry(i8 nBytes, i8 nLevels, Entry* aAfterEntry);
    void         vAllotHead(void);
    i8           iCopyEntries(IndexBuffer* pFromBuffer, Entry* aFromEntry);
    void         vDumpPrior(Entry* aPriorVec[]);
    void         vFixHigherLevel(Entry* aNewEntry);
    void         vFixPriorOffset(EntryVec* pPriorVec, Entry* aNewEntry);
    Entry*       pGetEntry(i8 iEntryNo);
    i8           iLevel(void);
    void         vMoveLeft(void* aDst, i8 nBytes);
    void         vMoveRight(void* aSrc, i8 nBytes);
    u8           uOffsetBetween(Entry* pEntryA, Entry* pEntryB);
    Entry*       pPosition(Key* aGivenKey, eJC eJuxtaCompare, EntryVec* pPriorVec);
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
  
  IndexBuffer* _pIndexBuffer(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
