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
    
    i8           nBufferLevel(void);
    void         vDump(void);
    i8           nEntries(void);
    bool         bHasSpace(i8 nBytes);
    Entry*       aHead(void);
    void         vHigherLevel(IndexBuffer* pHigherLevel);
    IndexBuffer* pHigherLevel(void);
    Index*       pIndex(void);
    bool         bInsert(KeyWsp& xKeyWsp, Id xId);
    IndexBuffer* pNextBuffer(void);
    Entry*       aNextEntry(Entry* aEntry);
    IndexBuffer* pPositionLevel(KeyWsp& xKeyWsp, eJC eJuxtaCompare, i8 iLevel);
    Entry*       aPosition(KeyWsp& xKeyWsp, eJC eJuxtaCompare);
    bool         bRemove(Key* aKey);
    void         vShow(void);
    IndexBuffer* pSplitBuffer(void);
    bool         bVerify(void);
    
  private:
    void         vCopyEntries(IndexBuffer* pFromBuffer, Entry* aFromEntry);
    void         vDumpRevise(Entry** ppReviseVec);
    void         vFixHigherLevel(Entry* aNewEntry);
    void         vFixRevise(Entry** ppReviseVec, Entry* aNewEntry);
    Entry*       pGetSplitEntry(i8 iSplitAtEntry);
       i8        iLevel(void);
    void         vMoveLeft(void* aDst, i8 nBytes);
    void         vMoveRight(void* aSrc, i8 nBytes);
    Entry*       aAllotEntry(i8 nBytes, i8 nLevels, Entry* aAfterEntry);
    void         vAllotHead(void);
    Entry*       aPosition(Key* aGivenKey, eJC eJuxtaCompare, Entry** aaReviseVec);
    void         vReplace(KeyWsp& xOldKeyWsp, KeyWsp& xNewKeyWsp);
    Entry*       aRightEntry(Entry* aEntry, u8 uLevel);
    Entry*       aToEntry(i8 iOffset);
    i8           iToEndOffset(Entry* aEntry);
    i8           iToOffsetAbs(Entry* aEntry);
    
  };
  
  IndexBuffer* _pIndexBuffer(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
