/*============================================================================*/
/* IndexBuffer.hpp -- Index Buffer                                            */
/*============================================================================*/
#pragma once

namespace DBobjects
{
  
  class IndexBuffer : public Buffer
  {
  public:
    static const i8 C_FLAT_LEVELS = 3;
    
  private:
    static const i8 C_NIL = 0xFFFFFFFFFFFFFFFF;
    
  private:
    i8           m_nEntries;    // number of entries in this buffer
    IndexBuffer* m_pPrevBuffer; // prev buffer
    IndexBuffer* m_pNextBuffer; // next buffer
    i8           m_nSkipHead[C_FLAT_LEVELS - 1];  // skip list head
    
    
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
    
    static void vDump(eCN eClass, Object* pObject, Str* aTab);
    
    void  vAppendBuffer(IndexBuffer* pArgBuffer);
    void  vCopyBuffer(IndexBuffer* pFromBuffer);
    i8    nEntries(void);
    void  vEntryCount(void);
    void  vInsertEntry(EntWsp& xEntWsp);
    bool  bInsertEntry(EntWsp& xEntWsp);
    IndexBuffer* pFindBuffer(EntWsp& xEntWsp);
    void  vShow(void);
    void  vSplitBuffer(void);
    void  vVerify(void);
  
    Entry* pFirstEntry();
    Entry* pNextEntry(Entry* pCurr);
    
    IndexBuffer* pNextBuffer(void);
    void         vNextBuffer(IndexBuffer* pNextBuffer);
    
    IndexBuffer* pPrevBuffer(void);
    void         vPrevBuffer(IndexBuffer* pPrevBuffer);
  };
  
  IndexBuffer* _pIndexBuffer(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
