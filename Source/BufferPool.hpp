/*============================================================================*/
/* BufferPool.hpp - a pool of buffers                                         */
/*============================================================================*/
#pragma once

#include "Pool.h"

using namespace Common;

namespace Pool
{
  class Buffer;

  class BufferPool : public Header
  {
    
    friend class BPA;
    
  private:
    
    // buffer pool characteristics
    i8   m_iRegionSize;   // size of the pool (e.g. 1_TB)
    i8   m_iBufferCount;  // number of buffers in pool
    
    s256 m_sShmName;      // shared memory region name
    int  m_iShmFD;        // shared memory file descriptor

    Buffer* m_pBase;      // starting address of pool (large section)
    Buffer* m_pJumbo;     // starting address of jumbo section
    Buffer* m_pLimit;     // address after pool
    
    // free chain
    Lock m_zFreeLock;     // lock over free chain
    i8   m_iAllocCount;   // number of buffers currently in use
    i8   m_iFreeCount;    // number of buffers in free chain
    bn   m_nFreeHead;     // head of free chain
    
    // must be last
    Footer m_xFooter;
    
  public:        
    void* operator new(size_t, void* pRegion);
    void  operator delete(void* pRegion);
    
    BufferPool(void);
    ~BufferPool(void);
    
    Buffer* pAllocBuffer(void);
    i8   iBufferCount(void);
    bn   nBufferFirst(void);
    bn   nBufferLast(void);
    bn   nBufferNext(bn nBuffer);
    bn   nBufferPrev(bn nBuffer);
    i8   iBufferSize(void);
    void vCheckAddress(const void* pAddress);
    bool bDetachPool(void);
    void vDump(void);
    void vFree(Buffer* pBuffer);
    i8   iFreeCount(void);
    i8   iFreePercent(void);
    i8   iFreeSpace(void);
    bool bHasDataSpace(void);
    void vInitialise(void);
    Buffer* pNextJumbo(Buffer* pPrev);
    i8   iRegionSize(void);
    Buffer* pToBuffer(bn nBuffer);
    BufferFree* pToBufferFree(bn nBuffer);
    BufferFree* pToBufferFree(Buffer* pBuffer);
    bool bValidAddress(const void* pAddress);
    void vVerifyPool(void);
    
    Footer* pFooter(void);
   };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
