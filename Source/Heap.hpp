/*============================================================================*/
/* Heap.hpp -- Heap Allocator                                                 */
/*============================================================================*/
#pragma once

namespace Pool
{
  enum eHT // Heap type
  {
    /*  0 */ HT_INVALID=0,
    /*  1 */ HT_PERSISTENT_SMALL,
    /*  2 */ HT_PERSISTENT_MEDIUM,
    /*  3 */ HT_PERSISTENT_LARGE,
    /*  4 */ HT_PERSISTENT_JUMBO,
    /*  5 */ HT_TRANSIENT_SMALL,
    /*  6 */ HT_TRANSIENT_MEDIUM,
    /*  7 */ HT_TRANSIENT_LARGE,
    /*  8 */ HT_TRANSIENT_JUMBO,
    /*  9 */ HT_ENUMS
  };
  
  static Stz* aHT[]
  {
    /*  0 */ "INVALID"_Stz,
    /*  1 */ "PERSISTENT_SMALL"_Stz,
    /*  2 */ "PERSISTENT_MEDIUM"_Stz,
    /*  3 */ "PERSISTENT_LARGE"_Stz,
    /*  4 */ "PERSISTENT_JUMBO"_Stz,
    /*  5 */ "TRANSIENT_SMALL"_Stz,
    /*  6 */ "TRANSIENT_MEDIUM"_Stz,
    /*  7 */ "TRANSIENT_LARGE"_Stz,
    /*  8 */ "TRANSIENT_JUMBO"_Stz,
    /*  9 */ "ENUMS"_Stz
  };
  
  class Heap : public PoolBuffer
  {
  friend class HeapMem;
  
  private:
    i8          m_iObjectSize;   // size of each object (e.g. 1KB or 16KB)
    Lock        m_okLock;         // lock over all segments and free chain
    i8          m_iAllocCount;   // number of allocated objects
    i8          m_iBufferCount;  // number of buffers in the heap
    HeapBuffer* m_pFirstBuffer;  // first buffer in the heap
    HeapBuffer* m_pLastBuffer;   // last buffer in the heap
    i8          m_iFreeCount;    // number of free chunks
    FreeChunk*  m_pFreeHead;     // head of free chunk chain
    bool        m_bFixedHeap;    // fixed size heap or able to extend?
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Heap(eCN eClass, i8 iObjectSize);
    ~Heap(void) noexcept(false);
    
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
  
    void    vAddFree(FreeChunk* aFreeChunk);
    i8      iAllocCount(void);
    Object* pAllocObject(eCN eClass);
    Object* pAllocObject(eCN eClass, i8 iMaxSize);
    void    vAnalyse(i8 xCounts[]);
    i8      iBufferCount(void);
    void    vDropLock(void);
    HeapBuffer* pFirstBuffer(void);
    void    vFixedHeap(bool bFixed);
    i8      iFreeCount(void);
    void    vFreeObject(Object* pObject);
    void    vGrabLock(void);
    void    vMonitorPortrait(s48K& sPortrait);
    HeapBuffer* pNextBuffer(HeapBuffer* pBuffer);
    i8      iObjectSize(void);
        
  private:
    bool bExtendHeap(void);
    FreeChunk* pPopFree(void);
    void vPushFree(FreeChunk* aFreeChunk, eCN eWasClass);
    void vVerify(void);
    void vValidFree(FreeChunk* aFreeChunk);
    
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
