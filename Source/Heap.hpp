/*============================================================================*/
/* Heap.hpp - heap allocator                                                  */
/*============================================================================*/

#pragma once

#include "MemoryManagement.h"

namespace MemoryManagement
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
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Heap(eCN eClass, i8 iObjectSize);
    ~Heap(void);
    
    Object* pAllocObject(void);
    void    vAnalyse(i8 xCounts[]);
    void    vDump(void);
    i8      iAllocCount(void);
    i8      iBufferCount(void);
    i8      iFreeCount(void);
    void    vFreeObject(Object* pObject);
    i8      iObjectSize(void);
    
  private:
    bool bExtendHeap(void);
    FreeChunk* pPopFree(void);
    void vPushFree(FreeChunk* pFreeChunk);
    void vVerify(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/