/*============================================================================*/
/* BufferPool.cpp - buffer allocator                                          */
/*============================================================================*/

#include "BASE.h"

extern BufferPool* gpBufferPool;

SubSysFn __BufferPool("BufferPool"_Stz,(fpSELFFN*)BufferPool::vSubSystem,
                      ""_Stz,nullptr);

void* C_ADR_REGION   = (void*)0x204800000000;

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* BufferPool::operator new(size_t, void* pRegion)
{
  return pRegion;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void BufferPool::operator delete(void*)
{
}

/*----------------------------------------------------------------------------*/
/* constructor (new region)                                                   */
/*----------------------------------------------------------------------------*/
BufferPool::BufferPool(Stz* aRegionName, i8 iRegionSize,
                       i8 iNonPageableSize,
                       bool bTouch, bool bProgress,
                       bool bDontFree, bool bFreeSpecific)
: Object(CN_BUFFERPOOL)
, m_okLock(this)
{
  gpBufferPool = this;
  
  m_nBuffers = DIV(iRegionSize,C_SIZE_POOL_BUFFER);
  m_nNonPageableBuffers = DIV(iNonPageableSize, C_SIZE_POOL_BUFFER);
  
  m_pBase  = reinterpret_cast<PoolBuffer*>(this);
  m_pLimit = (PoolBuffer*)aAdd<void*>(m_pBase,iRegionSize);
  m_bDontFree = bDontFree;
  m_bFreeSpecific = bFreeSpecific;
  
  m_sShmNm = aRegionName;
  m_iShmFD   = 0;
  
  m_nAllocCount  = 0;
  m_nFreeCount   = 0;
  m_iFreeHead    = C_NIL;
 
  m_ixHeapBuffers.vCheckInit();
  m_ixOtherBuffers.vCheckInit();

  vInitialise(bTouch,bProgress);
  
  m_pTakenBuffers = nullptr;
  m_nTakenBytes   = 0;

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
/* the subsystem function                                                     */
/*----------------------------------------------------------------------------*/
void BufferPool::vSubSystem(TokenVec* pTokenVec)
{
  _static_
  
  RASSERT(pTokenVec);

  vTrace("BufferPool::vSubSystem\n"_Stz);
  
  BufferPool::vReport();
}

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
/* get current number of allocated buffers                                    */
/*----------------------------------------------------------------------------*/
i8 BufferPool::nAllocCount(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUFFERPOOL,_FOOTER_);
  
  return m_nAllocCount;
}

/*----------------------------------------------------------------------------*/
/* allocate a buffer for use as a HeapBuffer from free chain in pool          */
/*----------------------------------------------------------------------------*/
PoolBuffer* BufferPool::pAllocHeapBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUFFERPOOL,_FOOTER_);
  
  FreePoolBuffer* pHeapBuffer = pPopFreePoolBuffer();
  
  ++m_ixHeapBuffers;
  
  return _pPoolBuffer(pHeapBuffer);
}

/*----------------------------------------------------------------------------*/
/* allocate a longer buffer from free chain in pool                           */
/*----------------------------------------------------------------------------*/
PoolBuffer* BufferPool::pAllocLongerBuffer(i8 nBuffers)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUFFERPOOL,_FOOTER_);
  
  FreePoolBuffer* pLongerBuffer = pPopFreePoolBuffer();
  
  ++m_ixOtherBuffers;
  nBuffers--;
  
  while (nBuffers--) {
    FreePoolBuffer* pAdditionalBuffer = pPopFreePoolBuffer();
    UNUSED(pAdditionalBuffer);
    ++m_ixOtherBuffers;
  }

  return _pPoolBuffer(pLongerBuffer);
}

/*----------------------------------------------------------------------------*/
/* allocate a buffer for some other use from free chain in pool               */
/*----------------------------------------------------------------------------*/
PoolBuffer* BufferPool::pAllocOtherBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUFFERPOOL,_FOOTER_);
  
  FreePoolBuffer* pOtherBuffer = pPopFreePoolBuffer();
  
  ++m_ixOtherBuffers;

  return _pPoolBuffer(pOtherBuffer);
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
void BufferPool::vCheckAddress(const void* aAddress, const char* azFunc)
{
  ASSERT(m_pBase);
  ASSERT(m_pLimit);
  
  if (aAddress < m_pBase) {
    vTrace("aAddress(%p) < m_pBase(%p) : func %s\n"_Stz,
           aAddress,m_pBase,azFunc);
    vThreadCallStackDump();
  }
  ASSERT(aAddress >= m_pBase);
  
  if (aAddress >= m_pLimit) {
    vTrace("aAddress(%p) >= m_pLimit(%p) : func %s\n"_Stz,
           aAddress,m_pLimit,azFunc);
    vThreadCallStackDump();
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

  vPushFreePoolBuffer(pBuffer);
  
  --m_ixHeapBuffers;
}

/*----------------------------------------------------------------------------*/
/* free buffer back into free chain                                           */
/*----------------------------------------------------------------------------*/
void BufferPool::vFreeOtherBuffer(PoolBuffer* pBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUFFERPOOL,_FOOTER_);

  vPushFreePoolBuffer(pBuffer);
  
  --m_ixOtherBuffers;
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
bool BufferPool::bFreeSpecific(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUFFERPOOL,_FOOTER_);
  
  return m_bFreeSpecific;
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
/* give buffers                                                               */
/*----------------------------------------------------------------------------*/
void BufferPool::vGiveBuffers(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUFFERPOOL,_FOOTER_);
  
  while (m_pTakenBuffers) {
    
    PoolBuffer* pBuffer     = m_pTakenBuffers;
    PoolBuffer* pNextBuffer = *(PoolBuffer**)pBuffer->aEmbeddedBuffer();

    m_pTakenBuffers = pNextBuffer;
    
    gpBufferPool->vFreeOtherBuffer(pBuffer);
  }
  
  m_nTakenBytes = 0;
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
/* initialise all pool buffers in the pool                                    */
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
//  for (FreePoolBuffer* pFreePoolBuffer : *this) {
//    pFreePoolBuffer->vBufferNo(nBuffer);
//    nBuffer++;
//    pFreePoolBuffer->vNextFree(nBuffer);
//  }

  i8 iBufferMod;

  i8 i_PoolBuffer = iBufferFirst();
  while (C_NIL != i_PoolBuffer) {

    if (bProgress) {
      iBufferMod = i_PoolBuffer % 1000;
      if (! iBufferMod) {
        fflush(stdout);
      }
    }
    
    FreePoolBuffer* pFreePoolBuffer = pToFreePoolBuffer(i_PoolBuffer);

    pFreePoolBuffer->vSetMagic();
    pFreePoolBuffer->vSetClassNo(CN_FREEPOOLBUFFER);
    
    //pFreePoolBuffer->aFooter()->vSetFooter();
    if (bTouch)
      pFreePoolBuffer->vZeroData();

    pFreePoolBuffer->vSetFooter();
    pFreePoolBuffer->vBufferNo(i_PoolBuffer);
    i_PoolBuffer = iBufferNext(i_PoolBuffer);
    pFreePoolBuffer->vNextFree(i_PoolBuffer);
    
    m_nFreeCount++;
  }

  FreePoolBuffer* pLastBuffer = gpBufferPool->pToFreePoolBuffer(gpBufferPool->iBufferLast());

  pLastBuffer->vSetMagic();
  pLastBuffer->vSetClassNo(CN_FREEPOOLBUFFER);
  //pLastBuffer->aFooter()->vSetFooter();
  if (bTouch)
    pLastBuffer->vZeroData();

  pLastBuffer->vSetFooter();
  pLastBuffer->vBufferNo(gpBufferPool->iBufferLast());
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
    
  if (nFreeCount() >= C_LOW_BUFFERS)
    return false;
    
  return true;
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
  
  //  for (FreePoolBuffer* pFreePoolBuffer : *this)
  //    vTrace("%p: BufferNo %lld NextFree %lld\n"_Stz,
  //           pFreePoolBuffer,
  //           *pFreePoolBuffer->nBufferNo(),
  //           *pFreePoolBuffer->nNextFree());
  
  i8 i_PoolBuffer = m_iFreeHead;
  while (C_NIL != i_PoolBuffer) {
    FreePoolBuffer* pFreePoolBuffer = pToFreePoolBuffer(i_PoolBuffer);
    vTrace("%p: BufferNo %lld NextFree %lld\n"_Stz,
           pFreePoolBuffer,
           pFreePoolBuffer->iBufferNo(),
           pFreePoolBuffer->iNextFree());
    //nBuffer = pFreePoolBuffer((nBuffer);
    i_PoolBuffer = pFreePoolBuffer->iNextFree();
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
/* take buffers                                                               */
/*----------------------------------------------------------------------------*/
void BufferPool::vTakeBuffers(i8 nSpareBuffers)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUFFERPOOL,_FOOTER_);
  
  while (nFreeCount() >= C_LOW_BUFFERS + nSpareBuffers) {

    PoolBuffer* pBufferChain  = m_pTakenBuffers;
    
    PoolBuffer* pTakenBuffer  = gpBufferPool->pAllocOtherBuffer();
    PoolBuffer** ppNextBuffer = (PoolBuffer**)pTakenBuffer->aEmbeddedBuffer();
    *ppNextBuffer = pBufferChain;
    
    m_pTakenBuffers = pTakenBuffer;
    m_nTakenBytes += C_SIZE_POOL_BUFFER;
  }

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
/* transalate pool buffer number to free buffer                               */
/*----------------------------------------------------------------------------*/
FreePoolBuffer* BufferPool::pToFreePoolBuffer(i8 i_PoolBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUFFERPOOL,_FOOTER_);
  
  FreePoolBuffer* pFreePoolBuffer =
    static_cast<FreePoolBuffer*>(FileMap::pToPoolBuffer(i_PoolBuffer));
  
  return pPoolCheck<FreePoolBuffer*>(pFreePoolBuffer, __func__);
}

/*----------------------------------------------------------------------------*/
/* transalate pool buffer address to free buffer                              */
/*----------------------------------------------------------------------------*/
FreePoolBuffer* BufferPool::pToFreePoolBuffer(PoolBuffer* pBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUFFERPOOL,_FOOTER_);
  
  FreePoolBuffer* pFreePoolBuffer = static_cast<FreePoolBuffer*>(pBuffer);
  
  return pPoolCheck<FreePoolBuffer*>(pFreePoolBuffer, __func__);
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
  i8 i_PoolBuffer = m_iFreeHead;
  
  RASSERT(i_PoolBuffer < m_nBuffers);
  
  while (C_NIL != i_PoolBuffer) {
    
    FreePoolBuffer* pBufferVerify = pToFreePoolBuffer(i_PoolBuffer);
    
    RASSERT(pBufferVerify->bIsMagicValid());
    RASSERT(CN_FREEPOOLBUFFER == pBufferVerify->eGetClassNo());

    i_PoolBuffer = pBufferVerify->iNextFree();
    RASSERT(i_PoolBuffer < m_nBuffers);
  }

}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* pop a free buffer from the free chain                                      */
/*----------------------------------------------------------------------------*/
FreePoolBuffer* BufferPool::pPopFreePoolBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUFFERPOOL,_FOOTER_);
  
  m_okLock.vGrabWrite();                                                  // 🔴

  //vVerifyPool();
  
  // test for empty free chain
  if (C_NIL == m_iFreeHead) {
    ASSERT( ! m_nFreeCount );
    vTrace("NODE%lld: RUNNING OUT OF MEMORY!!!\n"_Stz, AM::NODE::iThisNodeId());
    vTrace("BufferPool::pAllocBuffer: zero free buffers\n"_Stz);
    m_okLock.vDropWrite();                                                // 🔵
    return nullptr;
  }

  // allocate buffer by popping head off free chain
  FreePoolBuffer* pAllocatedBuffer = pToFreePoolBuffer(m_iFreeHead);
  m_iFreeHead = pAllocatedBuffer->iNextFree();
  
  // adjust counts
  m_nFreeCount--;
  m_nAllocCount++;
  
  //vVerifyPool();
  
  m_okLock.vDropWrite();                                                  // 🔵
  
  return pPoolCheck<FreePoolBuffer*>(pAllocatedBuffer, __func__);
}

/*----------------------------------------------------------------------------*/
/* allocate a buffer from free chain in pool                                  */
/*----------------------------------------------------------------------------*/
void BufferPool::vPrintProgress(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUFFERPOOL,_FOOTER_);
  
  static s32 sMagTiny;   MagLib::vByteMag(C_SIZE_TINY_OBJECT,  sMagTiny);
  static s32 sMagSmall;  MagLib::vByteMag(C_SIZE_SMALL_OBJECT, sMagSmall);
  static s32 sMagMedium; MagLib::vByteMag(C_SIZE_MEDIUM_OBJECT,sMagMedium);
  static s32 sMagLarge;  MagLib::vByteMag(C_SIZE_LARGE_OBJECT, sMagLarge);
  static s32 sMagData;   MagLib::vByteMag(C_SIZE_LARGE_OBJECT, sMagData);

  m_okLock.vGrabWrite();
  
  vTrace("BufferPool::nAllocs: %lld; nBuffers: %lld; AllocCount: %lld; nFreeHead: %p; iFreeCount: %lld\n"_Stz,
        m_nAllocCount, m_nBuffers, m_nAllocCount, m_iFreeHead, m_nFreeCount);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  vTrace("   Transient TinyHeap (%4s) buffers %-6lld alloc %-6lld free %-6lld\n"_Stz,
         sMagSmall.azChar(),
         AM::pHeaps()->Transient.m_pTiny->nBufferCount(),
         AM::pHeaps()->Transient.m_pTiny->nCurrAllocated(),
         AM::pHeaps()->Transient.m_pTiny->nFreeChunks());

  vTrace("   Transient SmallHeap (%4s) buffers %-6lld alloc %-6lld free %-6lld\n"_Stz,
         sMagSmall.azChar(),
         AM::pHeaps()->Transient.m_pSmall->nBufferCount(),
         AM::pHeaps()->Transient.m_pSmall->nCurrAllocated(),
         AM::pHeaps()->Transient.m_pSmall->nFreeChunks());

  vTrace("  Transient MediumHeap (%4s) buffers %-6lld alloc %-6lld free %-6lld\n"_Stz,
         sMagMedium.azChar(),
         AM::pHeaps()->Transient.m_pMedium->nBufferCount(),
         AM::pHeaps()->Transient.m_pMedium->nCurrAllocated(),
         AM::pHeaps()->Transient.m_pMedium->nFreeChunks());

  vTrace("   Transient LargeHeap (%4s) buffers %-6lld alloc %-6lld free %-6lld\n"_Stz,
         sMagLarge.azChar(),
         AM::pHeaps()->Transient.m_pLarge->nBufferCount(),
         AM::pHeaps()->Transient.m_pLarge->nCurrAllocated(),
         AM::pHeaps()->Transient.m_pLarge->nFreeChunks());

  vTrace("   Transient DataHeap (%4s) buffers %-6lld alloc %-6lld free %-6lld\n"_Stz,
         sMagData.azChar(),
         AM::pHeaps()->Transient.m_pData->nBufferCount(),
         AM::pHeaps()->Transient.m_pData->nCurrAllocated(),
         AM::pHeaps()->Transient.m_pData->nFreeChunks());

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  vTrace("   Persistent SmallHeap (%4s) buffers %-6lld alloc %-6lld free %-6lld\n"_Stz,
         sMagSmall.azChar(),
         AM::pHeaps()->Persistent.m_pSmall->nBufferCount(),
         AM::pHeaps()->Persistent.m_pSmall->nCurrAllocated(),
         AM::pHeaps()->Persistent.m_pSmall->nFreeChunks());

  vTrace("  Persistent MediumHeap (%4s) buffers %-6lld alloc %-6lld free %-6lld\n"_Stz,
         sMagMedium.azChar(),
         AM::pHeaps()->Persistent.m_pMedium->nBufferCount(),
         AM::pHeaps()->Persistent.m_pMedium->nCurrAllocated(),
         AM::pHeaps()->Persistent.m_pMedium->nFreeChunks());

  vTrace("   Persistent LargeHeap (%4s) buffers %-6lld alloc %-6lld free %-6lld\n"_Stz,
         sMagLarge.azChar(),
         AM::pHeaps()->Persistent.m_pLarge->nBufferCount(),
         AM::pHeaps()->Persistent.m_pLarge->nCurrAllocated(),
         AM::pHeaps()->Persistent.m_pLarge->nFreeChunks());

  //vVerifyPool();
  
  
  // allocate buffer by popping head off free chain
  FreePoolBuffer* pAllocatedBuffer = pToFreePoolBuffer(m_iFreeHead);
  m_iFreeHead = pAllocatedBuffer->iNextFree();
  
  // adjust counts
  m_nFreeCount--;
  m_nAllocCount++;
  
  //vVerifyPool();
  
  m_okLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* free buffer back into free chain                                           */
/*----------------------------------------------------------------------------*/
void BufferPool::vPushFreePoolBuffer(PoolBuffer* pBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUFFERPOOL,_FOOTER_);
  
  FreePoolBuffer* pFreePoolBuffer = pToFreePoolBuffer(pBuffer);
  
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
  pFreePoolBuffer->vSetClassNo(CN_FREEPOOLBUFFER);
  pFreePoolBuffer->vNextFree(m_iFreeHead);
  m_iFreeHead = pBuffer->iBufferNo();
  
  // adjust counts
  m_nFreeCount++;
  m_nAllocCount--;
  
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
void BASE::vCheckAddress(void *aAddress, const char* azFunc)
{
  if (gpBufferPool)
    gpBufferPool->vCheckAddress(aAddress,azFunc);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
