/*============================================================================*/
/* IndexBuffer.hpp -- Index Buffer (skip list variant)                        */
/*============================================================================*/
#pragma once

namespace DBobjects
{
  
  class IndexBuffer : public Buffer
  {
  public:
    static const i8 C_FLAT_LEVELS = 3;
    static const i8 C_FLAT_FREQ = 2;
    static const i8 C_NIL = 0xFFFFFFFFFFFFFFFF;
    
  private:
    i8           m_nEntries;    // number of entries in this buffer
    IndexBuffer* m_pPrevBuffer; // prev buffer
    IndexBuffer* m_pNextBuffer; // next buffer
  
    
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
    
    IndexBuffer(void);
    ~IndexBuffer(void);
    
    static IndexBuffer* pNewIndexBuffer(void);
  
    bool   bInsert(Key* aKey, RowId xRowId);
    bool   bRemove(Key* aKey);
  
  private:
    Entry*  aHeadEntry(void);
    i8      iLevel(void);
    void    vMoveLeft(void* aDst, i8 nBytes);
    void    vMoveRight(void* aSrc, i8 nBytes);
    Entry*  aNewEntry(i8 nBytes, i8 nLevels, Entry* aAfterEntry);
    void    vNewHead(void);
    Entry*  aNextEntry(Entry* aEntry);
    Entry*  aPosition(Key* aKey, bool& bEqualTo, Entry** aReviseVec);
    Entry*  aRightEntry(Entry* aEntry, i8 iLevel);
  };
  
  IndexBuffer* _pIndexBuffer(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
