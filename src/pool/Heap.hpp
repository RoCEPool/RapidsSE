/*============================================================================*/
/* Heap.hpp -- Heap Allocator                                                 */
/*============================================================================*/
#pragma once

namespace Resources
{
  class Heap : public PoolBuffer
  {
  friend class HeapMem;
  
  private:
    i8          m_nObjectSize;    // size of each object (e.g. 1KB or 16KB)
    bool        m_bFixedHeap;     // fixed size heap or able to extend?
  
    Lock        m_okLock;         // lock over the following...
    i8          m_nCurrAllocated; // number of currently allocated objects
    i8          m_nFreeChunks;    // number of free chunks
    FreeChunk*  m_pFreeHead;      // head of free chunk chain
    i8          m_nBufferCount;   // number of buffers in the heap
    HeapBuffer* m_pFirstBuffer;   // first buffer in the heap
    HeapBuffer* m_pLastBuffer;    // last buffer in the heap
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Heap(eCN eClass, i8 iObjectSize);
    ~Heap(void) noexcept(false);
    
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
  
    Object* pAllocObject(eCN eClass);
    void    vAnalyse(i8 xCounts[]);
    i8      nBufferCount(void);
    i8      nCurrAllocated(void);
    void    vDropLock(void);
    HeapBuffer* pFirstBuffer(void);
    void    vFixedHeap(bool bFixed);
    i8      nFreeChunks(void);
    void    vFreeObject(Object* pObject);
    void    vGrabLock(void);
    void    vMonitorPortrait(s48K& sPortrait);
    HeapBuffer* pNextBuffer(HeapBuffer* pBuffer);
    i8      nObjectSize(void);
    void    vReport(String* pReportString);
    i8      nTotalHeapSize(void);
        
  private:
    bool bExtendHeap(void);
    FreeChunk* pPopFree(void);
    void vPushFree(FreeChunk* aFreeChunk, eCN eWasClass);
    void vRemoveBuffer(HeapBuffer* pHeapBuffer);
    void vVerify(void);
    void vValidFree(FreeChunk* aFreeChunk);
    
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
