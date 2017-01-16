/*============================================================================*/
/* BufferPool.cpp - buffer allocator                                          */
/*============================================================================*/

#include "Pool.h"

#include "AttachMap.hpp"

extern AttachMap* gpAttachMap;
extern Arguments  gxArguments;

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
BufferPool::BufferPool(void)
: Header(CN_BUFFERPOOL)
{
  gpBufferPool = this;
  
  m_iRegionSize  = gxArguments.m_iRegionSize;
  m_iBufferCount = DIV(m_iRegionSize,C_SIZE_POOL_BUFFER) - gxArguments.m_nJumboTook;
  
  m_sShmName = gxArguments.m_sRegionName;
  m_iShmFD   = 0;

  m_pBase  = reinterpret_cast<Buffer*>(this);
  m_pLimit = reinterpret_cast<Buffer*>(pvAdd(m_pBase,m_iRegionSize));
  m_pJumbo = reinterpret_cast<Buffer*>(
                                       pvAdd(m_pLimit,-(gxArguments.m_nJumboTook * C_SIZE_POOL_BUFFER)));
  
  //m_zFreeLock
  m_iAllocCount   = 0;
  m_iFreeCount    = 0;
  m_nFreeHead     = bn::nNil();
  
  vInitialise();
  
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
  BufferFree* pAllocatedBuffer = pToBufferFree(m_nFreeHead);
  m_nFreeHead = pAllocatedBuffer->nNextFree();
  
  // adjust counts
  m_iFreeCount--;
  m_iAllocCount++;
  
  //vVerifyPool();
 
  m_zFreeLock.vDropWrite();
  
  return pPoolCheck<BufferFree*>(pAllocatedBuffer);
}

/*----------------------------------------------------------------------------*/
/* get buffer count                                                           */
/*----------------------------------------------------------------------------*/
i8 BufferPool::iBufferCount(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUFFERPOOL,_FOOTER_);
  
  return m_iBufferCount;
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
  
  return m_iBufferCount - 1;
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
void BufferPool::vCheckAddress(const void* pAddress)
{
  ASSERT(pAddress >= m_pBase);
  ASSERT(pAddress <  m_pLimit);
}

/*----------------------------------------------------------------------------*/
/* detach from buffer pool                                                    */
/*----------------------------------------------------------------------------*/
bool BufferPool::bDetachPool(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUFFERPOOL,_FOOTER_);
  
  SysCall xSysCall;
  
  bool bResult = Shm::bClose(m_iShmFD,xSysCall);
  
  bResult = Shm::bUnmap(m_pBase,m_iRegionSize,xSysCall);
  
  return bResult;
}

/*----------------------------------------------------------------------------*/
/* dump buffer pool                                                           */
/*----------------------------------------------------------------------------*/
void BufferPool::vDump(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUFFERPOOL,_FOOTER_);
  
  vTrace("RegionSize %lld BufferSize %lld BufferCount %lld Base %p Limit %p\n"
         "InUse %lld FreeCount %lld FreeHead %lld\n",
         m_iRegionSize,
         C_SIZE_POOL_BUFFER,
         m_iBufferCount,
         m_pBase,
         m_pLimit,
         m_iAllocCount,
         m_iFreeCount,
         *m_nFreeHead
         );
  
//  for (BufferFree* pFreeBuffer : *this)
//    vTrace("%p: BufferNo %lld NextFree %lld\n",
//           pFreeBuffer,
//           *pFreeBuffer->nBufferNo(),
//           *pFreeBuffer->nNextFree());

  bn nBuffer = m_nFreeHead;
  while (bn::nNil() != nBuffer) {
    BufferFree* pFreeBuffer = pToBufferFree(nBuffer);
    vTrace("%p: BufferNo %lld NextFree %lld\n",
           pFreeBuffer,
           *pFreeBuffer->nBufferNo(),
           *pFreeBuffer->nNextFree());
    //nBuffer = pToBufferFree((nBuffer);
    nBuffer = pFreeBuffer->nNextFree();
  }
  
}

/*----------------------------------------------------------------------------*/
/* free buffer back into free chain                                           */
/*----------------------------------------------------------------------------*/
void BufferPool::vFree(Buffer* pBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUFFERPOOL,_FOOTER_);
  
  BufferFree* pFreeBuffer = pToBufferFree(pBuffer);
  
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
  pFreeBuffer->vClass(CN_BUFFER);
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
  
  i8 iFreePercent = DIV(iFreeSpace() * 100,iRegionSize());
  
  return iFreePercent;
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
void BufferPool::vInitialise(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUFFERPOOL,_FOOTER_);
  
  m_iAllocCount = 0;
  //m_iFreeCount  = m_iBufferCount;
  m_iFreeCount  = 0;
  m_nFreeHead   = nBufferFirst();
  
  //vTrace("FirstBuffer %lld LastBuffer %lld\n",*nBufferFirst(),*nBufferLast());
  
//  bn nBuffer = nBufferFirst();
//  for (BufferFree* pFreeBuffer : *this) {
//    pFreeBuffer->vBufferNo(nBuffer);
//    nBuffer++;
//    pFreeBuffer->vNextFree(nBuffer);
//  }

  i8 iBufferMod;
  
  bn nBuffer = nBufferFirst();
  while (bn::nNil() != nBuffer) {
    
    if (gxArguments.m_bProgress) {
      iBufferMod = *nBuffer % 1000;
      if (! iBufferMod) {
        printf("buffer %lld of %lld             \r",*nBuffer,*nBufferLast());
        fflush(stdout);
      }
    }
    
    BufferFree* pFreeBuffer = pToBufferFree(nBuffer);
    
    pFreeBuffer->vSetMagic();
    pFreeBuffer->vClass(CN_BUFFER);
    //pFreeBuffer->pFooter()->vSetFooter();
    if (gxArguments.m_bTouch)
      pFreeBuffer->vZeroData();
    pFreeBuffer->vSetFooter();
    pFreeBuffer->vBufferNo(nBuffer);
    nBuffer = nBufferNext(nBuffer);
    pFreeBuffer->vNextFree(nBuffer);
    
    m_iFreeCount++;
}
  
  BufferFree* pLastBuffer = pToBufferFree(nBufferLast());
  pLastBuffer->vSetMagic();
  pLastBuffer->vClass(CN_BUFFER);
  //pLastBuffer->pFooter()->vSetFooter();
  if (gxArguments.m_bTouch)
    pLastBuffer->vZeroData();
  pLastBuffer->vSetFooter();
  pLastBuffer->vBufferNo(nBufferLast());
  pLastBuffer->vNextFree(bn::nNil());
  
  if (gxArguments.m_bProgress)
    printf("\n");
  
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
    pJumbo = reinterpret_cast<Buffer*>(
               pvAdd(pPrev,C_SIZE_POOL_BUFFER*C_JUMBO_RATIO));
    if (pJumbo >= m_pLimit)
      pJumbo = nullptr;
  }
  
  //vTrace("Jumbo %p %lld\n",pJumbo,iSub(pJumbo,pPrev));
  return pJumbo;
}

/*----------------------------------------------------------------------------*/
/* get size in bytes of pool                                                  */
/*----------------------------------------------------------------------------*/
i8 BufferPool::iRegionSize(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUFFERPOOL,_FOOTER_);
  
  return m_iRegionSize;
}

/*----------------------------------------------------------------------------*/
/* transalate buffer number to allocated buffer                               */
/*----------------------------------------------------------------------------*/
Buffer* BufferPool::pToBuffer(bn nBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUFFERPOOL,_FOOTER_);
  
  i8 iOffset = C_SIZE_POOL_BUFFER * *nBuffer;
  
  Buffer* pBuffer = static_cast<Buffer*>(Memory::pAddBytes(m_pBase,iOffset));
  
  return pPoolCheck<Buffer*>(pBuffer);
}

/*----------------------------------------------------------------------------*/
/* transalate buffer number to free buffer                                    */
/*----------------------------------------------------------------------------*/
BufferFree* BufferPool::pToBufferFree(bn nBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUFFERPOOL,_FOOTER_);
  
  BufferFree* pBufferFree = static_cast<BufferFree*>(pToBuffer(nBuffer));
  
  return pPoolCheck<BufferFree*>(pBufferFree);
}

/*----------------------------------------------------------------------------*/
/* transalate buffer number to free buffer                                    */
/*----------------------------------------------------------------------------*/
BufferFree* BufferPool::pToBufferFree(Buffer* pBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUFFERPOOL,_FOOTER_);
  
  BufferFree* pBufferFree = static_cast<BufferFree*>(pBuffer);
  
  return pPoolCheck<BufferFree*>(pBufferFree);
}

/*----------------------------------------------------------------------------*/
/* is address in pool?                                                        */
/*----------------------------------------------------------------------------*/
bool BufferPool::bValidAddress(const void* pAddress)
{
  if ((pAddress >= m_pBase) && (pAddress <  m_pLimit))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* assert buffer pool is valid                                                */
/*----------------------------------------------------------------------------*/
void BufferPool::vVerifyPool(void)
{
  bn nBuffer = m_nFreeHead;
  
  while (bn::nNil() != nBuffer) {
    
    BufferFree* pBufferVerify = pToBufferFree(nBuffer);
    
    RASSERT(MAGIC == pBufferVerify->iMagic());
    RASSERT(CN_BUFFER == pBufferVerify->eClass());
    
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

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
