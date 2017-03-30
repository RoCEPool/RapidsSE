/*============================================================================*/
/* IndexBufferSeq.hpp -- Index Buffer (sequential variant)                    */
/*============================================================================*/
#pragma once

namespace DBobjects
{
  class IndexBufferSeq : public Buffer
  {
  public:
  private:
    i8           m_nEntries;    // number of entries in this buffer
    IndexBufferSeq* m_pPrevBuffer; // prev buffer
    IndexBufferSeq* m_pNextBuffer; // next buffer
    i8           m_nSkipHead[C_FLAT_LEVELS];  // skip list head
  
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
  
  };
  
  IndexBufferSeq* _pIndexBufferSeq(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
