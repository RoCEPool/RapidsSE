/*============================================================================*/
/* IndexBufferSeq.hpp -- Index Buffer (sequential)                            */
/*============================================================================*/
#pragma once

namespace DBobjects
{
  class IndexBufferSeq : public Buffer
  {
  public:
    static const i8 C_FLAT_LEVELS = 3;
    static const i8 C_FLAT_FREQ = 2;
    static const i8 C_NIL = 0xFFFFFFFFFFFFFFFF;
    
  private:
    i8           m_nEntries;    // number of entries in this buffer
    IndexBufferSeq* m_pPrevBuffer; // prev buffer
    IndexBufferSeq* m_pNextBuffer; // next buffer
    i8           m_nSkipHead[C_FLAT_LEVELS];  // skip list head
    
    
    // need to add skip list head
    // head would be array of [FLAT_LEVELS - 1]
    // array of byte offsets, not addresses
    // will need a C_NIL say -1
    // entries are in contigious key order
    // so we dont keep L1, L1 is implied
    // insert is memmove to right
    // remove is memmove to left
    //
    // need to have verify & show fn's
  
    WspFxd<C_SIZE_LARGE_OBJECT - C_1KB> m_xWsp;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    IndexBufferSeq(void);
    ~IndexBufferSeq(void);
    
    static IndexBufferSeq* pNewIndexBufferSeq(void);
    
    static void vDump(eCN eClass, Object* pObject, Str* aTab);
    
    void  vAppendBuffer(IndexBufferSeq* pArgBuffer);
    void  vCopyBuffer(IndexBufferSeq* pFromBuffer);
    i8    nEntries(void);
    void  vEntryCount(void);
    void  vInsertEntry(EntWsp& xEntWsp);
    bool  bInsertEntry(EntWsp& xEntWsp);
    IndexBufferSeq* pFindBuffer(EntWsp& xEntWsp);
    void  vShow(void);
    void  vSplitBuffer(void);
    void  vVerify(void);
  
    Entry* pFirstEntry();
    Entry* pNextEntry(Entry* pCurr);
    
    IndexBufferSeq* pNextBuffer(void);
    void         vNextBuffer(IndexBufferSeq* pNextBuffer);
    
    IndexBufferSeq* pPrevBuffer(void);
    void         vPrevBuffer(IndexBufferSeq* pPrevBuffer);
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - //
    // skip list support                                                     //
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - //

    bool   bInsert(Key* pKey, RowId xRowId);
    bool   bRemove(Key* pKey);
  
    Entry* pPosition(Key* pKey, bool& bEqualTo, Entry** pReviseVec);
    i8     iLevel(void);
  
    Entry* pHeadEntry(void);
    Entry* pNewEntry(Entry* pAfterEntry, i8 nBytes, i8 nLevels);
    Entry* pRightEntry(Entry* pEntry, i8 iLevel);
    Entry* pNextEntrySL(Entry* pEntry);

    void   vMoveLeft(void* aDst, void* aSrc);
    void   vMoveRight(void* aSrc, i8 nBytes);
  };
  
  IndexBufferSeq* _pIndexBufferSeq(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
