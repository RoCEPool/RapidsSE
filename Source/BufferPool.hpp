/*============================================================================*/
/* BufferPool.hpp - a pool of buffers                                         */
/*============================================================================*/
#pragma once

#include "Pool.h"

using namespace Common;

namespace Pool
{
  class Buffer;
  class BufferPoolIter;

  class BufferPool : public mn,
                     public cn
  {
    
    friend class BufferPoolAnalyzer;
    
  private:
    
    // buffer pool characteristics
    i8   m_iRegionSize;    // size of the pool (e.g. 1_TB)
    i8   m_iBufferSize;  // size of each buffer (e.g. 1_MB)
    i8   m_iBufferCount; // number of buffers in pool
    
    s256 m_sShmName;     // shared memory region name
    int  m_iShmDescriptor; // shared memory file descriptor

    Buffer* m_pBase;     // starting address of pool
    Buffer* m_pLimit;    // address after pool
    
    // free chain
    i8   m_zFreeLock;    // lock over free chain
    i8   m_iAllocCount;  // number of buffers currently in use
    i8   m_iFreeCount;   // number of buffers in free chain
    bn   m_nFreeHead;    // head of free chain

  public:    
    static Buffer* pAttachRegion(s& sRegionName, int& iDescriptor);
    static Buffer* pAttachRegion(s& sRegionName,
                                 i8 iRegionSize, i8 iBufferSize, int& iDescriptor);
    static Buffer* pCreateRegion(s& sRegionName,
                                 i8 iRegionSize, i8 iBufferSize, int& iDescriptor);

    BufferPool(s& sRegionName);
    BufferPool(s& sRegionName, i8 iRegionSize, i8 iBufferSize);
    
    void* operator new(size_t);
    //----
    
    void* operator new(size_t, void* pRegion);
    void  operator delete(void* pRegion);
    BufferPool(void);
    ~BufferPool(void);
    
    Buffer* pAllocBuffer(void);
    bn   nBufferFirst(void) const;
    bn   nBufferLast(void) const;
    bn   nBufferNext(bn nBuffer) const;
    bn   nBufferPrev(bn nBuffer) const;
    i8   iBufferSize(void) const;
    void vCheckAddress(const void* pAddress) const;
    bool bDetachPool(void);
    void vDump(void);
    void vFree(Buffer* pBuffer);
    i8   iFreePercent(void);
    i8   iFreeSpace(void);
    bool bHasDataSpace(void);
    void vInitialise(void);
    i8   iRegionSize(void) const;
    Buffer* pToBuffer(bn nBuffer) const;
    BufferFree* pToBufferFree(bn nBuffer) const;
    BufferFree* pToBufferFree(Buffer* pBuffer) const;
 
    BufferPoolIter begin(void) const;
    BufferPoolIter end(void) const;
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/