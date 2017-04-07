/*============================================================================*/
/* BufferPool.cpp - buffer allocator                                          */
/*============================================================================*/

#include "Pool.h"

#include "StorageEngine.h"

extern AttachMap*  gpAttachMap;
extern BufferPool* gpBufferPool;
extern bool        gbProgress;

/*----------------------------------------------------------------------------*/
/* new operator                                                               */
/*----------------------------------------------------------------------------*/
void* BufferPool::operator new(size_t iSize, void* pRegion)
{
  RASSERT(iSize);
  
  return pRegion;
}

/*----------------------------------------------------------------------------*/
/* delete operator                                                            */
/*----------------------------------------------------------------------------*/
void BufferPool::operator delete(void* pRegion)
{
  RASSERT(pRegion);
}

/*----------------------------------------------------------------------------*/
/* constructor (new region)                                                   */
/*----------------------------------------------------------------------------*/
BufferPool::BufferPool(Str* aRegionName, i8 iRegionSize, i8 iJumboTook,
                       bool bTouch, bool bDontFree, bool bFreeRB)
: Object(CN_BUFFERPOOL)
{
  gpBufferPool = this;
  
  m_nBuffers = DIV(iRegionSize,C_SIZE_POOL_BUFFER)
                 - iJumboTook;
  

  m_pBase  = reinterpret_cast<Buffer*>(this);
  m_pLimit = pAdd<Buffer*>(m_pBase,iRegionSize);
  m_pJumbo = pAdd<Buffer*>(m_pLimit,-(iJumboTook * C_SIZE_POOL_BUFFER));
  m_bDontFree = bDontFree;
  m_bFreeRB = bFreeRB;
  
  m_sShmName = aRegionName;
  m_iShmFD   = 0;
  
  m_zFreeLock.vInit("BufferPool::m_zFreeLock"_Str);
  
  m_iAllocCount   = 0;
  m_iFreeCount    = 0;
  m_nFreeHead     = bn::nNil();
  
  vInitialise(bTouch);
  
  _checkInitialised_(CN_BUFFERPOOL);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_BUFFERPOOL,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
BufferPool::~BufferPool(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUFFERPOOL,_FOOTER_);
  
  bDetachPool();
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* allocate a buffer from free chain in pool                                  */
/*----------------------------------------------------------------------------*/
Buffer* BufferPool::pAllocBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUFFERPOOL,_FOOTER_);
  
  m_zFreeLock.vGrabWrite();
  
  //vVerifyPool();
  
  // test for empty free chain
  if (bn::nNil() == m_nFreeHead) {
    ASSERT( ! m_iFreeCount );
    m_zFreeLock.vDropWrite();
    return nullptr;
  }
  
  // allocate buffer by popping head off free chain
  FreeBuffer* pAllocatedBuffer = pToFreeBuffer(m_nFreeHead);
  m_nFreeHead = pAllocatedBuffer->nNextFree();
  
  // adjust counts
  m_iFreeCount--;
  m_iAllocCount++;
  
  //vVerifyPool();
 
  m_zFreeLock.vDropWrite();
  
  return pPoolCheck<FreeBuffer*>(pAllocatedBuffer);
}

/*----------------------------------------------------------------------------*/
/* get buffer count                                                           */
/*----------------------------------------------------------------------------*/
i8 BufferPool::nBuffers(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUFFERPOOL,_FOOTER_);
  
  return m_nBuffers;
}

/*----------------------------------------------------------------------------*/
/* get buffer number of first usable buffer in pool                           */
/*----------------------------------------------------------------------------*/
bn BufferPool::nBufferFirst(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUFFERPOOL,_FOOTER_);
  
  return 1_bn;
}

/*----------------------------------------------------------------------------*/
/* get buffer number of last buffer in pool                                   */
/*----------------------------------------------------------------------------*/
bn BufferPool::nBufferLast(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUFFERPOOL,_FOOTER_);
  
  return m_nBuffers - 1;
}

/*----------------------------------------------------------------------------*/
/* calculate next buffer number from given buffer number                      */
/*----------------------------------------------------------------------------*/
bn BufferPool::nBufferNext(bn nBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUFFERPOOL,_FOOTER_);
  
  // ALKA:
  //if (nBuffer + 1 == nBufferLast())
  if (nBuffer == nBufferLast())
    return bn::nNil();
  
  return nBuffer + 1;
}

/*----------------------------------------------------------------------------*/
/* calculate previous buffer number from given buffer number                  */
/*----------------------------------------------------------------------------*/
bn BufferPool::nBufferPrev(bn nBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUFFERPOOL,_FOOTER_);
  
  // ALKA:
  //if (nBuffer - 1 == nBufferFirst())
  if (nBuffer == nBufferFirst())
    return bn::nNil();
  
  return nBuffer - 1;
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
    vTrace("aAddress(%p) < m_pBase(%p) \n"_Str, aAddress, m_pBase);
  }
  ASSERT(aAddress >= m_pBase);
  
  if (aAddress >= m_pLimit) {
    vTrace("aAddress(%p) >= m_pLimit(%p) \n"_Str, aAddress, m_pLimit);
  }
  ASSERT(aAddress <  m_pLimit);
}

/*----------------------------------------------------------------------------*/
/* detach from buffer pool                                                    */
/*----------------------------------------------------------------------------*/
bool BufferPool::bDetachPool(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUFFERPOOL,_FOOTER_);
  
  SysCall xSysCall;
  
  bool bResult = Shm::bClose(m_iShmFD,xSysCall);
  
  bResult = Shm::bUnmap(m_pBase,gpAttachMap->m_iRegionSize,xSysCall);
  
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
void BufferPool::vFree(Buffer* pBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUFFERPOOL,_FOOTER_);
  
  FreeBuffer* pFreeBuffer = pToFreeBuffer(pBuffer);
  
  m_zFreeLock.vGrabWrite();
  
  //vVerifyPool();
  
  // test for existing free list
  if (bn::nNil() != m_nFreeHead) {
    ASSERT( m_iFreeCount > 0 );
  }
  else {
    ASSERT( ! m_iFreeCount );
  }
  
  // free buffer by pushing onto free chain
  pFreeBuffer->vClass(CN_FREEBUFFER);
  pFreeBuffer->vNextFree(m_nFreeHead);
  m_nFreeHead = pBuffer->nBufferNo();
  
  // adjust counts
  m_iFreeCount++;
  m_iAllocCount--;
  
  //vVerifyPool();
  
  m_zFreeLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* return number of free buffers                                              */
/*----------------------------------------------------------------------------*/
i8 BufferPool::iFreeCount(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUFFERPOOL,_FOOTER_);
  
  return m_iFreeCount;
}

/*----------------------------------------------------------------------------*/
/* return free space percentage                                               */
/*----------------------------------------------------------------------------*/
i8 BufferPool::iFreePercent(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUFFERPOOL,_FOOTER_);
  
  i8 iFreePercent = DIV(iFreeSpace() * 100,gpAttachMap->m_iRegionSize);
  
  return iFreePercent;
}

/*----------------------------------------------------------------------------*/
/* free RowBuffers back to heap?                                              */
/*----------------------------------------------------------------------------*/
bool BufferPool::bFreeRB(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUFFERPOOL,_FOOTER_);
  
  return m_bFreeRB;
}

/*----------------------------------------------------------------------------*/
/* return free space in bytes                                                 */
/*----------------------------------------------------------------------------*/
i8 BufferPool::iFreeSpace(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUFFERPOOL,_FOOTER_);
  
  i8 iFreeSpace = m_iFreeCount * C_SIZE_POOL_BUFFER;
  
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
/* initialise all buffers in the pool                                         */
/*----------------------------------------------------------------------------*/
void BufferPool::vInitialise(bool bTouch)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUFFERPOOL,_FOOTER_);
  
  m_iAllocCount = 0;
  //m_iFreeCount  = m_nBuffers;
  m_iFreeCount  = 0;
  m_nFreeHead   = nBufferFirst();
  
  //vTrace("FirstBuffer %lld LastBuffer %lld\n"_Str,*nBufferFirst(),*nBufferLast());
  
//  bn nBuffer = nBufferFirst();
//  for (FreeBuffer* pFreeBuffer : *this) {
//    pFreeBuffer->vBufferNo(nBuffer);
//    nBuffer++;
//    pFreeBuffer->vNextFree(nBuffer);
//  }

  i8 iBufferMod;
  
  bn nBuffer = nBufferFirst();
  while (bn::nNil() != nBuffer) {
    
    if (gbProgress) {
      iBufferMod = *nBuffer % 1000;
      if (! iBufferMod) {
        vInfo("buffer %lld of %lld             \r"_Str,*nBuffer,*nBufferLast());
        fflush(stdout);
      }
    }
    
    FreeBuffer* pFreeBuffer = pToFreeBuffer(nBuffer);
    
    pFreeBuffer->vSetMagic();
    pFreeBuffer->vClass(CN_FREEBUFFER);
    //pFreeBuffer->pFooter()->vSetFooter();
    if (bTouch)
      pFreeBuffer->vZeroData();
    pFreeBuffer->vSetFooter();
    pFreeBuffer->vBufferNo(nBuffer);
    nBuffer = nBufferNext(nBuffer);
    pFreeBuffer->vNextFree(nBuffer);
    
    m_iFreeCount++;
}
  
  FreeBuffer* pLastBuffer = pToFreeBuffer(nBufferLast());
  pLastBuffer->vSetMagic();
  pLastBuffer->vClass(CN_FREEBUFFER);
  //pLastBuffer->pFooter()->vSetFooter();
  if (bTouch)
    pLastBuffer->vZeroData();
  pLastBuffer->vSetFooter();
  pLastBuffer->vBufferNo(nBufferLast());
  pLastBuffer->vNextFree(bn::nNil());
  
  if (gbProgress)
    vInfo("\n"_Str);
  
  //vDump();
}

/*----------------------------------------------------------------------------*/
/* next jumbo pointer                                                         */
/*----------------------------------------------------------------------------*/
Buffer* BufferPool::pNextJumbo(Buffer* pPrev)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUFFERPOOL,_FOOTER_);
  
  Buffer* pJumbo;
  
  if ( ! pPrev ) {
    pJumbo = m_pJumbo;
  }
  else {
    pJumbo = pAdd<Buffer*>(pPrev,C_SIZE_POOL_BUFFER*C_JUMBO_RATIO);
    if (pJumbo >= m_pLimit)
      pJumbo = nullptr;
  }
  
  //vTrace("Jumbo %p %lld\n"_Str,pJumbo,iSub(pJumbo,pPrev));
  return pJumbo;
}

/*----------------------------------------------------------------------------*/
/* dump buffer pool                                                           */
/*----------------------------------------------------------------------------*/
void BufferPool::vPoolDump(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUFFERPOOL,_FOOTER_);
  
  vTrace("RegionSize %lld BufferSize %lld BufferCount %lld Base %p Limit %p\n"_Str
         "InUse %lld FreeCount %lld FreeHead %lld\n"_Str,
         gpAttachMap->m_iRegionSize,
         C_SIZE_POOL_BUFFER,
         m_nBuffers,
         m_pBase,
         m_pLimit,
         m_iAllocCount,
         m_iFreeCount,
         *m_nFreeHead
         );
  
  //  for (FreeBuffer* pFreeBuffer : *this)
  //    vTrace("%p: BufferNo %lld NextFree %lld\n"_Str,
  //           pFreeBuffer,
  //           *pFreeBuffer->nBufferNo(),
  //           *pFreeBuffer->nNextFree());
  
  bn nBuffer = m_nFreeHead;
  while (bn::nNil() != nBuffer) {
    FreeBuffer* pFreeBuffer = pToFreeBuffer(nBuffer);
    vTrace("%p: BufferNo %lld NextFree %lld\n"_Str,
           pFreeBuffer,
           *pFreeBuffer->nBufferNo(),
           *pFreeBuffer->nNextFree());
    //nBuffer = pToFreeBuffer((nBuffer);
    nBuffer = pFreeBuffer->nNextFree();
  }
  
}

/*----------------------------------------------------------------------------*/
/* transalate buffer number to allocated buffer                               */
/*----------------------------------------------------------------------------*/
Buffer* BufferPool::pToBuffer(bn nBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUFFERPOOL,_FOOTER_);
  
  i8 iOffset = C_SIZE_POOL_BUFFER * *nBuffer;
  
  Buffer* pBuffer = static_cast<Buffer*>(MemLib::pAddBytes(m_pBase,iOffset));
  
  return pPoolCheck<Buffer*>(pBuffer);
}

/*----------------------------------------------------------------------------*/
/* transalate buffer number to free buffer                                    */
/*----------------------------------------------------------------------------*/
FreeBuffer* BufferPool::pToFreeBuffer(bn nBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUFFERPOOL,_FOOTER_);
  
  FreeBuffer* pBufferFree = static_cast<FreeBuffer*>(pToBuffer(nBuffer));
  
  return pPoolCheck<FreeBuffer*>(pBufferFree);
}

/*----------------------------------------------------------------------------*/
/* transalate buffer number to free buffer                                    */
/*----------------------------------------------------------------------------*/
FreeBuffer* BufferPool::pToFreeBuffer(Buffer* pBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUFFERPOOL,_FOOTER_);
  
  FreeBuffer* pBufferFree = static_cast<FreeBuffer*>(pBuffer);
  
  return pPoolCheck<FreeBuffer*>(pBufferFree);
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
bool BufferPool::bVerifyBoundaries(bool bTrace)
{
  if (bTrace)
    vTrace("Base=%p Limit=%p\n"_Str, m_pBase, m_pLimit);
  
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
  bn nBuffer = m_nFreeHead;
  
  while (bn::nNil() != nBuffer) {
    
    FreeBuffer* pBufferVerify = pToFreeBuffer(nBuffer);
    
    RASSERT(MAGIC == pBufferVerify->iMagic());
    RASSERT(CN_FREEBUFFER == pBufferVerify->eClass());
    
    nBuffer = pBufferVerify->nNextFree();
  }

}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* BufferPool::pFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUFFERPOOL);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- CheckAddress ------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
void Base::vCheckAddress(void *aAddress)
{
  gpBufferPool->vCheckAddress(aAddress);
}
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
