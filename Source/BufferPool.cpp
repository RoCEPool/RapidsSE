/*============================================================================*/
/* BufferPool.cpp - buffer allocator                                          */
/*============================================================================*/

#include "Pool.h"



extern BufferPool* gpBufferPool;

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* new operator                                                               */
/*----------------------------------------------------------------------------*/
void* BufferPool::operator new(size_t, void* pRegion)
{
  return pRegion;
}

/*----------------------------------------------------------------------------*/
/* delete operator                                                            */
/*----------------------------------------------------------------------------*/
void BufferPool::operator delete(void*)
{
}

/*----------------------------------------------------------------------------*/
/* constructor (new region)                                                   */
/*----------------------------------------------------------------------------*/
BufferPool::BufferPool(Stz* aRegionName, i8 iRegionSize,
                       bool bTouch, bool bProgress,
                       bool bDontFree, bool bFreeCB)
: Object(CN_BUFFERPOOL)
, m_okLock(LK_BUFFER_POOL,this)
{
  gpBufferPool = this;
  
  m_nBuffers = DIV(iRegionSize,C_SIZE_POOL_BUFFER);
  
  m_pBase  = reinterpret_cast<PoolBuffer*>(this);
  m_pLimit = (PoolBuffer*)aAdd<void*>(m_pBase,iRegionSize);
  m_bDontFree = bDontFree;
  m_bFreeCB = bFreeCB;
  
  m_sShmNm = aRegionName;
  m_iShmFD   = 0;
  
  m_nAllocCount  = 0;
  m_nFreeCount   = 0;
  m_iFreeHead    = C_NIL;
 
  m_ixHeapBuffers.vCheckInit();
  m_ixOtherBuffers.vCheckInit();

  vInitialise(bTouch,bProgress);
  
  _checkInitialised_(CN_BUFFERPOOL);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_BUFFERPOOL,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
BufferPool::~BufferPool(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUFFERPOOL,_FOOTER_);
  
  bDetachPool();
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* report on shm region                                                       */
/*----------------------------------------------------------------------------*/
void BufferPool::vReport(void)
{
  // get MagLib of the various buffer pools
  s32 sRegionMag;
  MagLib::vByteMag(AM::iRegionSize(), sRegionMag);
  
  i8  iSum = C_SIZE_POOL_BUFFER
             +
             gpBufferPool->m_nAllocCount * C_SIZE_POOL_BUFFER
             +
             gpBufferPool->m_nFreeCount  * C_SIZE_POOL_BUFFER;

  s32 sSum;
  MagLib::vByteMag(iSum, sSum);

  vTrace("Buffer Pool %p: name %s size %s"
         " buffers %lld alloc %lld free %lld (sum=%s)\n"_Stz,
         
         gpBufferPool,
         
         gpBufferPool->m_sShmNm.szStz(),
         sRegionMag.azChar(),
         
         gpBufferPool->m_nBuffers,
         gpBufferPool->m_nAllocCount,
         gpBufferPool->m_nFreeCount,
         sSum.azChar()
         );
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* allocate the one attachMap buffer from free chain in pool                  */
/*----------------------------------------------------------------------------*/
PoolBuffer* BufferPool::pAllocAttachMapBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUFFERPOOL,_FOOTER_);
  
  FreeBuffer* pAllocatedBuffer = pPopFreeBuffer_NL();
  
  ++m_ixOtherBuffers;
  
  return _pPoolBuffer(pAllocatedBuffer);
}

/*----------------------------------------------------------------------------*/
/* get current number of allocated buffers                                    */
/*----------------------------------------------------------------------------*/
i8 BufferPool::nAllocCount(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUFFERPOOL,_FOOTER_);
  
  return m_nAllocCount;
}

/*----------------------------------------------------------------------------*/
/* allocate a buffer for a HeapBuffer from free chain in pool                 */
/*----------------------------------------------------------------------------*/
PoolBuffer* BufferPool::pAllocHeapBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUFFERPOOL,_FOOTER_);
  
  FreeBuffer* pAllocatedBuffer = pPopFreeBuffer();
  
  ++m_ixHeapBuffers;
  
  return _pPoolBuffer(pAllocatedBuffer);
}

/*----------------------------------------------------------------------------*/
/* allocate a buffer from free chain in pool                                  */
/*----------------------------------------------------------------------------*/
PoolBuffer* BufferPool::pAllocOtherBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUFFERPOOL,_FOOTER_);
  
  FreeBuffer* pAllocatedBuffer = pPopFreeBuffer();
  
  ++m_ixOtherBuffers;

  return _pPoolBuffer(pAllocatedBuffer);
}

/*----------------------------------------------------------------------------*/
/* get base address                                                           */
/*----------------------------------------------------------------------------*/
PoolBuffer* BufferPool::pBase(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUFFERPOOL,_FOOTER_);
  
  return m_pBase;
}

/*----------------------------------------------------------------------------*/
/* get number of buffers in pool                                              */
/*----------------------------------------------------------------------------*/
i8 BufferPool::nBufferCount(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUFFERPOOL,_FOOTER_);
  
  return m_nBuffers;
}

/*----------------------------------------------------------------------------*/
/* get buffer number of first usable buffer in pool                           */
/*----------------------------------------------------------------------------*/
i8 BufferPool::iBufferFirst(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUFFERPOOL,_FOOTER_);
  
  return 1;
}

/*----------------------------------------------------------------------------*/
/* get buffer number of last buffer in pool                                   */
/*----------------------------------------------------------------------------*/
i8 BufferPool::iBufferLast(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUFFERPOOL,_FOOTER_);
  
  return m_nBuffers - 1;
}

/*----------------------------------------------------------------------------*/
/* calculate next buffer number from given buffer number                      */
/*----------------------------------------------------------------------------*/
i8 BufferPool::iBufferNext(i8 iBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUFFERPOOL,_FOOTER_);
  
  // ALKA:
  //if (nBuffer + 1 == iBufferLast())
  if (iBuffer == iBufferLast())
    return C_NIL;
  
  return iBuffer + 1;
}

/*----------------------------------------------------------------------------*/
/* calculate previous buffer number from given buffer number                  */
/*----------------------------------------------------------------------------*/
i8 BufferPool::iBufferPrev(i8 iBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUFFERPOOL,_FOOTER_);
  
  // ALKA:
  //if (nBuffer - 1 == iBufferFirst())
  if (iBuffer == iBufferFirst())
    return C_NIL;
  
  return iBuffer - 1;
}

/*----------------------------------------------------------------------------*/
/* get size in bytes of a buffer                                              */
/*----------------------------------------------------------------------------*/
i8 BufferPool::iBufferSize(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUFFERPOOL,_FOOTER_);
  
  return C_SIZE_POOL_BUFFER;
}

/*----------------------------------------------------------------------------*/
/* check address is in pool                                                   */
/*----------------------------------------------------------------------------*/
void BufferPool::vCheckAddress(const void* aAddress)
{
  ASSERT(m_pBase);
  ASSERT(m_pLimit);
  
  if (aAddress < m_pBase) {
    vTrace("aAddress(%p) < m_pBase(%p) \n"_Stz, aAddress, m_pBase);
  }
  ASSERT(aAddress >= m_pBase);
  
  if (aAddress >= m_pLimit) {
    vTrace("aAddress(%p) >= m_pLimit(%p) \n"_Stz, aAddress, m_pLimit);
  }
  ASSERT(aAddress <  m_pLimit);
}

/*----------------------------------------------------------------------------*/
/* detach from buffer pool                                                    */
/*----------------------------------------------------------------------------*/
bool BufferPool::bDetachPool(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUFFERPOOL,_FOOTER_);
  
  SysCall oSysCall;
  
  bool bResult = ShmLib::bClose(m_iShmFD,oSysCall);
  
  bResult = ShmLib::bUnmap(m_pBase,AM::iRegionSize(),oSysCall);
  
  return bResult;
}

/*----------------------------------------------------------------------------*/
/* dont actually free object?                                                 */
/*----------------------------------------------------------------------------*/
bool BufferPool::bDontFree(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUFFERPOOL,_FOOTER_);
  
  return m_bDontFree;
}

/*----------------------------------------------------------------------------*/
/* free buffer back into free chain                                           */
/*----------------------------------------------------------------------------*/
void BufferPool::vFree(PoolBuffer* pBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUFFERPOOL,_FOOTER_);
  
  FreeBuffer* pFreeBuffer = pToFreeBuffer(pBuffer);
  
  m_okLock.vGrabWrite();
  
  //vVerifyPool();
  
  // test for existing free list
  if (C_NIL != m_iFreeHead) {
    ASSERT( m_nFreeCount > 0 );
  }
  else {
    ASSERT( ! m_nFreeCount );
  }
  
  // free buffer by pushing onto free chain
  pFreeBuffer->vClass(CN_FREEBUFFER);
  pFreeBuffer->vNextFree(m_iFreeHead);
  m_iFreeHead = pBuffer->iBufferNo();
  
  // adjust counts
  m_nFreeCount++;
  m_nAllocCount--;
  
  //vVerifyPool();
  
  m_okLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* get number of currently free buffers                                       */
/*----------------------------------------------------------------------------*/
i8 BufferPool::nFreeCount(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUFFERPOOL,_FOOTER_);
  
  return m_nFreeCount;
}

/*----------------------------------------------------------------------------*/
/* free buffer back into free chain                                           */
/*----------------------------------------------------------------------------*/
void BufferPool::vFreeHeapBuffer(PoolBuffer* pBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUFFERPOOL,_FOOTER_);
  
  vFree(pBuffer);
  
  --m_ixHeapBuffers;
}

/*----------------------------------------------------------------------------*/
/* return free space percentage                                               */
/*----------------------------------------------------------------------------*/
i8 BufferPool::iFreePercent(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUFFERPOOL,_FOOTER_);
  
  i8 iFreeSize = iFreeSpace();
  i8 iShmSize  = AM::iRegionSize();
  
  i8 iFreePercent = DIV(iFreeSize * 100,iShmSize);
  
  return iFreePercent;
}

/*----------------------------------------------------------------------------*/
/* free CrwBuffers back to heap?                                              */
/*----------------------------------------------------------------------------*/
bool BufferPool::bFreeCB(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUFFERPOOL,_FOOTER_);
  
  return m_bFreeCB;
}

/*----------------------------------------------------------------------------*/
/* return free space in bytes                                                 */
/*----------------------------------------------------------------------------*/
i8 BufferPool::iFreeSpace(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUFFERPOOL,_FOOTER_);
  
  i8 iFreeSpace = m_nFreeCount * C_SIZE_POOL_BUFFER;
  
  return iFreeSpace;
}

/*----------------------------------------------------------------------------*/
/* does the pool have room for more data (row or entries) ?                   */
/*----------------------------------------------------------------------------*/
bool BufferPool::bHasDataSpace(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUFFERPOOL,_FOOTER_);
  
  i8   iPercent = iFreePercent();
  
  bool bHasSpace = iPercent > 19 ? true : false;
  
  return bHasSpace;
}

/*----------------------------------------------------------------------------*/
/* check address is in pool                                                   */
/*----------------------------------------------------------------------------*/
bool BufferPool::bInBufferPool(Object* pObject)
{
  RASSERT(m_pBase);
  RASSERT(m_pLimit);
  
  if (pObject < m_pBase)
    return false;
  
  if (pObject >= m_pLimit)
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* initialise all buffers in the pool                                         */
/*----------------------------------------------------------------------------*/
void BufferPool::vInitialise(bool bTouch, bool bProgress)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUFFERPOOL,_FOOTER_);
  
  m_nAllocCount = 0;
  //m_nFreeCount  = m_nBuffers;
  m_nFreeCount  = 0;
  m_iFreeHead   = iBufferFirst();
  
  //vTrace("FirstBuffer %lld LastBuffer %lld\n"_Stz,*iBufferFirst(),*iBufferLast());
  
//  bn nBuffer = iBufferFirst();
//  for (FreeBuffer* pFreeBuffer : *this) {
//    pFreeBuffer->vBufferNo(nBuffer);
//    nBuffer++;
//    pFreeBuffer->vNextFree(nBuffer);
//  }

  i8 iBufferMod;
  
  i8 i_Buffer = iBufferFirst();
  while (C_NIL != i_Buffer) {
    
    if (bProgress) {
      iBufferMod = i_Buffer % 1000;
      if (! iBufferMod) {
        fflush(stdout);
      }
    }
    
    FreeBuffer* pFreeBuffer = pToFreeBuffer(i_Buffer);
    
    pFreeBuffer->vMagic();
    pFreeBuffer->vClass(CN_FREEBUFFER);
    
    //pFreeBuffer->aFooter()->vSetFooter();
    if (bTouch)
      pFreeBuffer->vZeroData();
    
    pFreeBuffer->vSetFooter();
    pFreeBuffer->vBufferNo(i_Buffer);
    i_Buffer = iBufferNext(i_Buffer);
    pFreeBuffer->vNextFree(i_Buffer);
    
    m_nFreeCount++;
}
  
  FreeBuffer* pLastBuffer = pToFreeBuffer(iBufferLast());
  
  pLastBuffer->vMagic();
  pLastBuffer->vClass(CN_FREEBUFFER);
  //pLastBuffer->aFooter()->vSetFooter();
  if (bTouch)
    pLastBuffer->vZeroData();

  pLastBuffer->vSetFooter();
  pLastBuffer->vBufferNo(iBufferLast());
  pLastBuffer->vNextFree(C_NIL);

    
  //vDump();
}

/*----------------------------------------------------------------------------*/
/* get limit address                                                          */
/*----------------------------------------------------------------------------*/
PoolBuffer* BufferPool::pLimit(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUFFERPOOL,_FOOTER_);
  
  return m_pLimit;
}

/*----------------------------------------------------------------------------*/
/* test for low memory condition                                              */
/*----------------------------------------------------------------------------*/
bool BufferPool::bLowMemory(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUFFERPOOL,_FOOTER_);
  
  bool bLow = nFreeCount() < C_LOW_BUFFERS ? true : false;
  
  return bLow;
}

/*----------------------------------------------------------------------------*/
/* monitor portrait                                                           */
/*----------------------------------------------------------------------------*/
void BufferPool::vMonitorPortrait(s48K& sPortrait)
{
  sPortrait.vSprint("Pool: buffers %lld allocated %lld (heap %lld other %lld) "
                    "free %lld taken %lld\n"_Stz,
                    m_nBuffers,
                    m_nAllocCount,
                      *m_ixHeapBuffers,
                      *m_ixOtherBuffers,
                    m_nFreeCount,
                    AM::nTakenBytes()
                    );
}

/*----------------------------------------------------------------------------*/
/* dump buffer pool                                                           */
/*----------------------------------------------------------------------------*/
void BufferPool::vPoolDump(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUFFERPOOL,_FOOTER_);
  
  vTrace("RegionSize %lld BufferSize %lld BufferCount %lld Base %p Limit %p\n"_Stz
         "InUse %lld FreeCount %lld FreeHead %lld\n"_Stz,
         AM::iRegionSize(),
         C_SIZE_POOL_BUFFER,
         m_nBuffers,
         m_pBase,
         m_pLimit,
         m_nAllocCount,
         m_nFreeCount,
         m_iFreeHead
         );
  
  //  for (FreeBuffer* pFreeBuffer : *this)
  //    vTrace("%p: BufferNo %lld NextFree %lld\n"_Stz,
  //           pFreeBuffer,
  //           *pFreeBuffer->nBufferNo(),
  //           *pFreeBuffer->nNextFree());
  
  i8 i_Buffer = m_iFreeHead;
  while (C_NIL != i_Buffer) {
    FreeBuffer* pFreeBuffer = pToFreeBuffer(i_Buffer);
    vTrace("%p: BufferNo %lld NextFree %lld\n"_Stz,
           pFreeBuffer,
           pFreeBuffer->iBufferNo(),
           pFreeBuffer->iNextFree());
    //nBuffer = pToFreeBuffer((nBuffer);
    i_Buffer = pFreeBuffer->iNextFree();
  }
  
}

/*----------------------------------------------------------------------------*/
/* return free space in bytes                                                 */
/*----------------------------------------------------------------------------*/
Stz* BufferPool::szShmNm(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUFFERPOOL,_FOOTER_);
  
  return m_sShmNm;
}

/*----------------------------------------------------------------------------*/
/* return free space in bytes                                                 */
/*----------------------------------------------------------------------------*/
i8 BufferPool::iTotalSpace(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUFFERPOOL,_FOOTER_);
  
  i8 iTotalSpace = m_nBuffers * C_SIZE_POOL_BUFFER;
  
  return iTotalSpace;
}

/*----------------------------------------------------------------------------*/
/* translate buffer number to allocated buffer                                */
/*----------------------------------------------------------------------------*/
PoolBuffer* BufferPool::pToPoolBuffer(i8 i_Buffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUFFERPOOL,_FOOTER_);
  
  i8 iOffset = C_SIZE_POOL_BUFFER * i_Buffer;
  
  PoolBuffer* pBuffer = static_cast<PoolBuffer*>(MemLib::aAddBytes(m_pBase,iOffset));
  
  return pPoolCheck<PoolBuffer*>(pBuffer);
}

/*----------------------------------------------------------------------------*/
/* transalate buffer number to free buffer                                    */
/*----------------------------------------------------------------------------*/
FreeBuffer* BufferPool::pToFreeBuffer(i8 i_Buffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUFFERPOOL,_FOOTER_);
  
  FreeBuffer* pBufferFree =
    static_cast<FreeBuffer*>(pToPoolBuffer(i_Buffer));
  
  return pPoolCheck<FreeBuffer*>(pBufferFree);
}

/*----------------------------------------------------------------------------*/
/* transalate buffer number to free buffer                                    */
/*----------------------------------------------------------------------------*/
FreeBuffer* BufferPool::pToFreeBuffer(PoolBuffer* pBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUFFERPOOL,_FOOTER_);
  
  FreeBuffer* pBufferFree = static_cast<FreeBuffer*>(pBuffer);
  
  return pPoolCheck<FreeBuffer*>(pBufferFree);
}

/*----------------------------------------------------------------------------*/
/* return free space in bytes                                                 */
/*----------------------------------------------------------------------------*/
i8 BufferPool::iUsedSpace(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUFFERPOOL,_FOOTER_);
  
  i8 iUsedSpace = m_nAllocCount * C_SIZE_POOL_BUFFER;
  
  return iUsedSpace;
}

/*----------------------------------------------------------------------------*/
/* is address in pool?                                                        */
/*----------------------------------------------------------------------------*/
bool BufferPool::bValidAddress(const void* aAddress)
{
  if ((aAddress >= m_pBase) && (aAddress <  m_pLimit))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* assert buffer pool is valid                                                */
/*----------------------------------------------------------------------------*/
bool BufferPool::bVerifyBoundaries(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUFFERPOOL,_FOOTER_);
  
  if ( ! m_pLimit )
    return false;
  
  if ( ! m_pBase )
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* assert buffer pool is valid                                                */
/*----------------------------------------------------------------------------*/
void BufferPool::vVerifyPool(void)
{
  i8 i_Buffer = m_iFreeHead;
  
  RASSERT(i_Buffer < m_nBuffers);
  
  while (C_NIL != i_Buffer) {
    
    FreeBuffer* pBufferVerify = pToFreeBuffer(i_Buffer);
    
    RASSERT(pBufferVerify->bValidMagic());
    RASSERT(CN_FREEBUFFER == pBufferVerify->eClass());
    
    i_Buffer = pBufferVerify->iNextFree();
    RASSERT(i_Buffer < m_nBuffers);
  }

}

/*----------------------------------------------------------------------------*/
/* pop a free buffer from the free chain                                      */
/*----------------------------------------------------------------------------*/
FreeBuffer* BufferPool::pPopFreeBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUFFERPOOL,_FOOTER_);
  
  m_okLock.vGrabWrite();                                                  // 🔴

  //vVerifyPool();
  
  // test for empty free chain
  if (C_NIL == m_iFreeHead) {
    ASSERT( ! m_nFreeCount );
    vTrace("NODE%lld: RUNNING OUT OF MEMORY!!!\n"_Stz, AM::iThisNodeNo());
    vWarn("BufferPool::pAllocBuffer: zero free buffers\n"_Stz);
    m_okLock.vDropWrite();                                                // 🔵
    return nullptr;
  }

  // allocate buffer by popping head off free chain
  FreeBuffer* pAllocatedBuffer = pToFreeBuffer(m_iFreeHead);
  m_iFreeHead = pAllocatedBuffer->iNextFree();
  
  // adjust counts
  m_nFreeCount--;
  m_nAllocCount++;
  
  //vVerifyPool();
  
  m_okLock.vDropWrite();                                                  // 🔵
  
  return pPoolCheck<FreeBuffer*>(pAllocatedBuffer);
}

/*----------------------------------------------------------------------------*/
/* pop a free buffer from the free chain                                      */
/*----------------------------------------------------------------------------*/
FreeBuffer* BufferPool::pPopFreeBuffer_NL(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUFFERPOOL,_FOOTER_);
  
  // allocate buffer by popping head off free chain
  FreeBuffer* pAllocatedBuffer = pToFreeBuffer(m_iFreeHead);
  m_iFreeHead = pAllocatedBuffer->iNextFree();
  
  // adjust counts
  m_nFreeCount--;
  m_nAllocCount++;
  
  return pPoolCheck<FreeBuffer*>(pAllocatedBuffer);
}

/*----------------------------------------------------------------------------*/
/* allocate a buffer from free chain in pool                                  */
/*----------------------------------------------------------------------------*/
void BufferPool::vPrintProgress(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUFFERPOOL,_FOOTER_);
  
  static s32 sMagSmall;  MagLib::vByteMag(C_SIZE_SMALL_OBJECT, sMagSmall);
  static s32 sMagMedium; MagLib::vByteMag(C_SIZE_MEDIUM_OBJECT,sMagMedium);
  static s32 sMagLarge;  MagLib::vByteMag(C_SIZE_LARGE_OBJECT, sMagLarge);
  
  m_okLock.vGrabWrite();
  
  vTrace("BufferPool::nAllocs: %lld; nBuffers: %lld; AllocCount: %lld; nFreeHead: %p; iFreeCount: %lld\n"_Stz,
        m_nAllocCount, m_nBuffers, m_nAllocCount, m_iFreeHead, m_nFreeCount);

  vTrace("   Transient SmallHeap (%4s) buffers %-6lld alloc %-6lld free %-6lld\n"_Stz,
         sMagSmall.azChar(),
         AM::pHeapMem()->Transient.m_pSmall->nBufferCount(),
         AM::pHeapMem()->Transient.m_pSmall->nCurrAllocated(),
         AM::pHeapMem()->Transient.m_pSmall->nFreeChunks());

  vTrace("  Transient MediumHeap (%4s) buffers %-6lld alloc %-6lld free %-6lld\n"_Stz,
         sMagMedium.azChar(),
         AM::pHeapMem()->Transient.m_pMedium->nBufferCount(),
         AM::pHeapMem()->Transient.m_pMedium->nCurrAllocated(),
         AM::pHeapMem()->Transient.m_pMedium->nFreeChunks());

  vTrace("   Transient LargeHeap (%4s) buffers %-6lld alloc %-6lld free %-6lld\n"_Stz,
         sMagLarge.azChar(),
         AM::pHeapMem()->Transient.m_pLarge->nBufferCount(),
         AM::pHeapMem()->Transient.m_pLarge->nCurrAllocated(),
         AM::pHeapMem()->Transient.m_pLarge->nFreeChunks());

  vTrace("   Persistent SmallHeap (%4s) buffers %-6lld alloc %-6lld free %-6lld\n"_Stz,
         sMagSmall.azChar(),
         AM::pHeapMem()->Persistent.m_pSmall->nBufferCount(),
         AM::pHeapMem()->Persistent.m_pSmall->nCurrAllocated(),
         AM::pHeapMem()->Persistent.m_pSmall->nFreeChunks());

  vTrace("  Persistent MediumHeap (%4s) buffers %-6lld alloc %-6lld free %-6lld\n"_Stz,
         sMagMedium.azChar(),
         AM::pHeapMem()->Persistent.m_pMedium->nBufferCount(),
         AM::pHeapMem()->Persistent.m_pMedium->nCurrAllocated(),
         AM::pHeapMem()->Persistent.m_pMedium->nFreeChunks());

  vTrace("   Persistent LargeHeap (%4s) buffers %-6lld alloc %-6lld free %-6lld\n"_Stz,
         sMagLarge.azChar(),
         AM::pHeapMem()->Persistent.m_pLarge->nBufferCount(),
         AM::pHeapMem()->Persistent.m_pLarge->nCurrAllocated(),
         AM::pHeapMem()->Persistent.m_pLarge->nFreeChunks());

  //vVerifyPool();
  
  
  // allocate buffer by popping head off free chain
  FreeBuffer* pAllocatedBuffer = pToFreeBuffer(m_iFreeHead);
  m_iFreeHead = pAllocatedBuffer->iNextFree();
  
  // adjust counts
  m_nFreeCount--;
  m_nAllocCount++;
  
  //vVerifyPool();
  
  m_okLock.vDropWrite();
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* BufferPool::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUFFERPOOL);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- CheckAddress ------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
void Correctness::vCheckAddress(void *aAddress)
{
  if (gpBufferPool)
    gpBufferPool->vCheckAddress(aAddress);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
