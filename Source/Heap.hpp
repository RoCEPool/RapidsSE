/*============================================================================*/
/* Heap.hpp - heap allocator                                                  */
/*============================================================================*/

#pragma once

#include "Pool.h"

namespace Pool
{
  class Heap : public Buffer
  {
  private:
    i8          m_iObjectSize;   // size of each object (e.g. 1KB or 16KB)
    Lock        m_zLock;         // lock over all segments and free chain
    i8          m_iAllocCount;   // number of allocated objects
    i8          m_iBufferCount;  // number of buffers in the heap
    HeapBuffer* m_pFirstBuffer;  // first buffer in the heap
    HeapBuffer* m_pLastBuffer;   // last buffer in the heap
    i8          m_iFreeCount;    // number of free chunks
    FreeChunk*  m_pFreeHead;     // head of free chunk chain
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Heap(eCN eClass, i8 iObjectSize);
    ~Heap(void);
    
    static void vDump(eCN eClass, Object* pObject, const char* pTab);
    
    Object* pAllocObject(eCN eClass);
    void    vAnalyse(i8 xCounts[]);
    i8      iAllocCount(void);
    i8      iBufferCount(void);
    void    vDropLock(void);
    HeapBuffer* pFirstBuffer(void);
    HeapBuffer* pNextBuffer(HeapBuffer* pBuffer);
    i8      iFreeCount(void);
    void    vFreeObject(Object* pObject);
    void    vGrabLock(void);
    i8      iObjectSize(void);
        
  private:
    bool bExtendHeap(void);
    FreeChunk* pPopFree(void);
    void vPushFree(FreeChunk* pFreeChunk);
    void vVerify(void);
    
    Footer* pFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/