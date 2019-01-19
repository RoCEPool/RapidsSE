/*============================================================================*/
/* BufferPool.hpp - a pool of buffers                                         */
/*============================================================================*/
#pragma once

namespace Pool
{
  static const i8 C_MIN_REGION = 1024 * C_1MB;
  static const i8 C_MEMORY_PERCENT = 95;
  static const void* C_ADDR_POOL = (void*)0x204800000000;

  class Buffer;

  class BufferPool : public Object
  {
    friend class BPA;
    
  private:
    i8      m_nBuffers;  // number of buffers in pool
    Buffer* m_pBase;     // starting address of pool (large section)
    Buffer* m_pJumbo;    // starting address of jumbo section
    Buffer* m_pLimit;    // address after pool
    bool    m_bDontFree; // dont actually free objects back to heap
    bool    m_bFreeRB;   // but actually free RowBuffers back to heap

    s32  m_sShmName;    // shared memory region name
    int  m_iShmFD;      // shared memory file descriptor
    
    // free chain
    Lock m_zFreeLock;    // lock over free chain
    i8   m_nAllocCount;  //   number of buffers currently in use
    i8   m_nFreeCount;   //   number of buffers in free chain
    bn   m_nFreeHead;    //   head of free chain

    Atomic  m_izHeapBuffers; //
    Atomic  m_izOtherBuffers; //

    // must be last
    Footer m_xFooter;
    
  public:        
    void* operator new(size_t, void* pRegion);
    void  operator delete(void* pRegion);
    
    BufferPool(Stz* aRegionName, i8 iRegionSize, i8 iJumboTook,
               bool bTouch, bool bDontFree, bool bFreeRB);
    ~BufferPool(void) noexcept(false);
  
    static void vReport(void);
    
    Buffer* pAllocAttachMapBuffer(void);
    i8      nAllocCount(void);
    Buffer* pAllocHeapBuffer(void);
    Buffer* pAllocOtherBuffer(void);
    i8   nBufferCount(void);
    bn   nBufferFirst(void);
    bn   nBufferLast(void);
    bn   nBufferNext(bn nBuffer);
    bn   nBufferPrev(bn nBuffer);
    i8   iBufferSize(void);
    void vCheckAddress(const void* aAddress);
    bool bDetachPool(void);
    bool bDontFree(void);
    void vFree(Buffer* pBuffer);
    i8   nFreeCount(void);
    i8   iFreePercent(void);
    bool bFreeRB(void);
    i8   iFreeSpace(void);
    bool bHasDataSpace(void);
    void vInitialise(bool bTouch);
    bool bLowMemory(void);
    void vMonitorPortrait(s48K& sPortrait);
    Buffer* pNextJumbo(Buffer* pPrev);
    void vPoolDump(void);
    Buffer* pToBuffer(bn nBuffer);
    FreeBuffer* pToFreeBuffer(bn nBuffer);
    FreeBuffer* pToFreeBuffer(Buffer* pBuffer);
    bool bValidAddress(const void* aAddress);
    bool bVerifyBoundaries(bool bTrace);
    void vVerifyPool(void);
  
  private:
    FreeBuffer* pPopFreeBuffer(void);
    FreeBuffer* pPopFreeBuffer_NL(void);
    void vPrintProgress(void);

    Footer* aFooter(void);
   };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
