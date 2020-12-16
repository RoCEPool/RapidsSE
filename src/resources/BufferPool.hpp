/*============================================================================*/
/* BufferPool.hpp - a pool of buffers                                         */
/*============================================================================*/
#pragma once

namespace Resources
{
  static const void* C_ADR_REGION   = (void*)0x204800000000;

  static const i8 C_MIN_SHM_REGION  = 1024 * C_1MB;
  static const i8 C_MAX_SHM_PERCENT = 95;

  class BufferPool : public Object
  {
  friend class Utilities::BPA;
  friend class Utilities::PoolHelper;
  private:
    i8      m_nBuffers;   // number of buffers in pool
    PoolBuffer* m_pBase;  // starting address of pool (large section)
    PoolBuffer* m_pLimit; // address after pool
    bool    m_bDontFree;  // dont actually free objects back to heap
    bool    m_bFreeCB;    // but actually free CrwBuffers back to heap

    s32  m_sShmNm;        // shared memory region name
    int  m_iShmFD;        // shared memory file descriptor
    
    // free chain
    Lock m_okLock;        // lock over free chain
    i8   m_nAllocCount;   //   number of buffers currently in use
    i8   m_nFreeCount;    //   number of buffers in free chain
    i8   m_iFreeHead;     //   head of free chain

    Atomic  m_ixHeapBuffers;  //
    Atomic  m_ixOtherBuffers; //

    // must be last
    Footer m_xFooter;
    
  public:        
    void* operator new(size_t, void* pRegion);
    void  operator delete(void* pRegion);
    
    BufferPool(Stz* aRegionName, i8 iRegionSize,
               bool bTouch, bool bProgress,
               bool bDontFree, bool bFreeCB);
   ~BufferPool(void) noexcept(false);
  
    static void vReport(void);
    
    PoolBuffer* pAllocAttachMapBuffer(void);
    i8          nAllocCount(void);
    PoolBuffer* pAllocHeapBuffer(void);
    PoolBuffer* pAllocOtherBuffer(void);
    PoolBuffer* pBase(void);
    i8   nBufferCount(void);
    i8   iBufferFirst(void);
    i8   iBufferLast(void);
    i8   iBufferNext(i8 iBuffer);
    i8   iBufferPrev(i8 iBuffer);
    i8   iBufferSize(void);
    void vCheckAddress(const void* aAddress);
    bool bDetachPool(void);
    bool bDontFree(void);
    void vFree(PoolBuffer* pBuffer);
    void vFreeHeapBuffer(PoolBuffer* pPoolBuffer);
    i8   nFreeCount(void);
    i8   iFreePercent(void);
    bool bFreeCB(void);
    i8   iFreeSpace(void);
    bool bHasDataSpace(void);
    bool bInBufferPool(Object* pObject);
    void vInitialise(bool bTouch, bool bProgress);
    PoolBuffer* pLimit(void);
    bool bLowMemory(void);
    void vMonitorPortrait(s48K& sPortrait);
    void vPoolDump(void);
    Stz* szShmNm(void);
    i8   iTotalSpace(void);
    PoolBuffer* pToPoolBuffer(i8 i_Buffer);
    FreeBuffer* pToFreeBuffer(i8 i_Buffer);
    FreeBuffer* pToFreeBuffer(PoolBuffer* pBuffer);
    i8   iUsedSpace(void);
    bool bValidAddress(const void* aAddress);
    bool bVerifyBoundaries(void);
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
