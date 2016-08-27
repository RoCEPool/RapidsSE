/*============================================================================*/
/* BufferPool.cpp - buffer allocator                                          */
/*============================================================================*/

#include "Pool.h"

#include "AttachMap.hpp"

extern AttachMap* gpAttachMap;
extern Arguments  gxArguments;

extern void* C_ADDR_POOL;

/*----------------------------------------------------------------------------*/
/* attach to an existing shared buffer pool                                   */
/*----------------------------------------------------------------------------*/
Buffer* BufferPool::pAttachRegion(s& sRegionName, int& iDescriptor)
{
  SysCall xSysCall;
  
  vTrace("Attaching region %s\n",sRegionName.a());
  
  // open a shared memory region
  bool bResult = Shm::bAttach(sRegionName.a(),iDescriptor,xSysCall);
  if ( ! bResult ) {
    vTrace("BufferPool::pAttachRegion Shared Memory Region failed to open region: %s\n",
           sRegionName.a());
    exit(EXIT_FAILURE);
  }
  
  // map the opened shared memory region to memory
  void* pRegion = Shm::pMap(iDescriptor,C_ADDR_POOL,10_MB,true,xSysCall);
  if ( ! pRegion )  {
    vTrace("BufferPool::pAttachRegion failed to map!\n");
    Shm::bClose(iDescriptor,xSysCall);
    exit(EXIT_FAILURE);
  }
  
  vTrace("Shm mapped at %p\n",pRegion);
  
  return reinterpret_cast<Buffer*>(pRegion);
}

/*----------------------------------------------------------------------------*/
/* attach to an existing shared buffer pool                                   */
/*----------------------------------------------------------------------------*/
Buffer* BufferPool::pAttachRegion(s& sRegionName,
                                  i8 iRegionSize, i8 iBufferSize, int& iDescriptor)
{
  SysCall xSysCall;
  
  vTrace("Attaching region %s\n",sRegionName.a());
  
  // open a shared memory region
  bool bResult = Shm::bAttach(sRegionName.a(),iDescriptor,xSysCall);
  if ( ! bResult ) {
    vTrace("BufferPool::pAttachRegion Shared Memory Region failed to open region: %s\n",
           sRegionName.a());
    exit(EXIT_FAILURE);
  }
  
  // map the opened shared memory region to memory
  void* pRegion = Shm::pMap(iDescriptor,C_ADDR_POOL,iRegionSize,true,xSysCall);
  if ( ! pRegion )  {
    vTrace("BufferPool::pAttachRegion failed to map!\n");
    Shm::bClose(iDescriptor,xSysCall);
    exit(EXIT_FAILURE);
  }
  
  vTrace("Shm mapped at %p\n",pRegion);
  
  return reinterpret_cast<Buffer*>(pRegion);
}

/*----------------------------------------------------------------------------*/
/* create a new shared buffer pool                                            */
/*----------------------------------------------------------------------------*/
Buffer* BufferPool::pCreateRegion(s& sRegionName,
                                  i8 iRegionSize, i8 iBufferSize, int& iDescriptor)
{
  SysCall xSysCall;
  
  vTrace("Creating region %s\n",sRegionName.a());
  
  // open a shared memory region
  bool bResult = Shm::bCreate(sRegionName.a(),iRegionSize,iDescriptor,xSysCall);
  if ( ! bResult ) {
    vTrace("BufferPool::pCreateRegion Shared Memory Region failed to open region %s\n",
           sRegionName.a());
    exit(EXIT_FAILURE);
  }
  // map the opened shared memory region to memory
  void* pRegion = Shm::pMap(iDescriptor,C_ADDR_POOL,iRegionSize,true,xSysCall);
  if ( ! pRegion )  {
    vTrace("BufferPool::pCreateRegion failed to map!\n");
    Shm::bClose(iDescriptor,xSysCall);
    exit(EXIT_FAILURE);
  }
  
  vTrace("Shm mapped at %p\n",pRegion);
  
  return reinterpret_cast<Buffer*>(pRegion);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* attach to an existing buffer pool                                          */
/*----------------------------------------------------------------------------*/
BufferPool::BufferPool(s& sRegionName)
{
  vClass(CN_BUFFERPOOL);
  
  m_iRegionSize    = 10_MB;
  m_iBufferSize  = 1_MB;
  m_sShmName     = sRegionName;
  
  m_zFreeLock    = 0;
  m_iAllocCount  = 0;
  m_iFreeCount   = 0;
  
  m_pBase = pAttachRegion(sRegionName,m_iShmDescriptor);
  
  gpAttachMap = reinterpret_cast<AttachMap*>(pcAdd(m_pBase,C_SIZE_LARGE_BUFFER));
  
  m_iRegionSize   = gpAttachMap->m_iRegionSize;
  m_iBufferSize = C_SIZE_LARGE_BUFFER;
  
  bool bResult = bDetachPool();
  ASSERT(bResult);

  m_pBase  = pAttachRegion(sRegionName,m_iRegionSize,m_iBufferSize,m_iShmDescriptor);
  m_pLimit = reinterpret_cast<Buffer*>(pvAdd(m_pBase,m_iRegionSize));
  m_iBufferCount = DIV(m_iRegionSize,m_iBufferSize);
  
  // do not initialise
}

/*----------------------------------------------------------------------------*/
/* create a new buffer pool                                                   */
/*----------------------------------------------------------------------------*/
BufferPool::BufferPool(s& sRegionName, i8 iRegionSize, i8 iBufferSize)
{
  vClass(CN_BUFFERPOOL);
  
  gpBufferPool   = this;
  m_iRegionSize    = iRegionSize;
  m_iBufferSize  = iBufferSize;
  m_iBufferCount = DIV(m_iRegionSize,m_iBufferSize);
  m_sShmName     = sRegionName;
  
  m_zFreeLock    = 0;
  m_iAllocCount  = 0;
  m_iFreeCount   = 0;
  
  m_nFreeHead    = bn::nNil();
  m_pBase        = pCreateRegion(m_sShmName,m_iRegionSize,m_iBufferSize,m_iShmDescriptor);
  m_pLimit       = reinterpret_cast<Buffer*>(pvAdd(m_pBase,m_iRegionSize));
  
  vInitialise();
}

/*----------------------------------------------------------------------------*/
/* new operator                                                               */
/*----------------------------------------------------------------------------*/
void* BufferPool::operator new(size_t)
{
  void* pBufferPool = malloc(sizeof(BufferPool));
  
  return pBufferPool;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
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
void BufferPool::operator delete(void* pRegion)
{
}

/*----------------------------------------------------------------------------*/
/* initialise a new buffer pool                                               */
/*----------------------------------------------------------------------------*/
BufferPool::BufferPool(void)
{
  vClass(CN_BUFFERPOOL);
  
  gpBufferPool   = this;
  m_iRegionSize    = gxArguments.m_iRegionSize;
  m_iBufferSize  = C_SIZE_LARGE_BUFFER;
  m_iBufferCount = DIV(m_iRegionSize,C_SIZE_LARGE_BUFFER);
  m_sShmName     = gxArguments.m_sRegionName;
  
  m_zFreeLock    = 0;
  m_iAllocCount  = 0;
  m_iFreeCount   = 0;
  
  m_nFreeHead    = bn::nNil();
  m_pBase        = reinterpret_cast<Buffer*>(this);
  m_pLimit       = reinterpret_cast<Buffer*>(pvAdd(m_pBase,m_iRegionSize));
  
  vInitialise();
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
BufferPool::~BufferPool(void)
{
  _class_(CN_BUFFERPOOL);
  
  bDetachPool();
}

/*----------------------------------------------------------------------------*/
/* allocate a buffer from free chain in pool                                  */
/*----------------------------------------------------------------------------*/
Buffer* BufferPool::pAllocBuffer(void)
{
  _class_(CN_BUFFERPOOL);
  
  // test for empty free chain
  if (bn::nNil() == m_nFreeHead) {
    ASSERT( ! m_iFreeCount );
    return nullptr;
  }
  
  // allocate buffer by popping head off free chain
  BufferFree* pAllocatedBuffer = pToBufferFree(m_nFreeHead);
  m_nFreeHead = pAllocatedBuffer->nNextFree();
  
  // adjust counts
  m_iFreeCount--;
  m_iAllocCount++;
  
  return pPoolCheck<BufferFree*>(pAllocatedBuffer);
}

/*----------------------------------------------------------------------------*/
/* check address is in pool                                                   */
/*----------------------------------------------------------------------------*/
void BufferPool::vCheckAddress(const void* pAddress) const
{
  ASSERT(pAddress >= m_pBase);
  ASSERT(pAddress <  m_pLimit);
}

/*----------------------------------------------------------------------------*/
/* get buffer number of first usable buffer in pool                           */
/*----------------------------------------------------------------------------*/
bn BufferPool::nBufferFirst(void) const
{
  _class_(CN_BUFFERPOOL);
  
  return 1_bn;
}

/*----------------------------------------------------------------------------*/
/* get buffer number of last buffer in pool                                   */
/*----------------------------------------------------------------------------*/
bn BufferPool::nBufferLast(void) const
{
  _class_(CN_BUFFERPOOL);
  
  return m_iBufferCount - 1;
}

/*----------------------------------------------------------------------------*/
/* calculate next buffer number from given buffer number                      */
/*----------------------------------------------------------------------------*/
bn BufferPool::nBufferNext(bn nBuffer) const
{
  _class_(CN_BUFFERPOOL);
  
  // ALKA:
  //if (nBuffer + 1 == nBufferLast())
  if (nBuffer == nBufferLast())
    return bn::nNil();
  
  return nBuffer + 1;
}

/*----------------------------------------------------------------------------*/
/* calculate previous buffer number from given buffer number                  */
/*----------------------------------------------------------------------------*/
bn BufferPool::nBufferPrev(bn nBuffer) const
{
  _class_(CN_BUFFERPOOL);
  
  // ALKA:
  //if (nBuffer - 1 == nBufferFirst())
  if (nBuffer == nBufferFirst())
    return bn::nNil();
  
  return nBuffer - 1;
}

/*----------------------------------------------------------------------------*/
/* get size in bytes of a buffer                                              */
/*----------------------------------------------------------------------------*/
i8 BufferPool::iBufferSize(void) const
{
  _class_(CN_BUFFERPOOL);
  
  return m_iBufferSize;
}

/*----------------------------------------------------------------------------*/
/* detach from buffer pool                                                    */
/*----------------------------------------------------------------------------*/
bool BufferPool::bDetachPool(void)
{
  _class_(CN_BUFFERPOOL);
  
  SysCall xSysCall;
  
  bool bResult = Shm::bClose(m_iShmDescriptor,xSysCall);
  
  bResult = Shm::bUnmap(m_pBase,m_iRegionSize,xSysCall);
  
  return bResult;
}

/*----------------------------------------------------------------------------*/
/* dump buffer pool                                                           */
/*----------------------------------------------------------------------------*/
void BufferPool::vDump(void)
{
  _class_(CN_BUFFERPOOL);
  
  vTrace("RegionSize %lld BufferSize %lld BufferCount %lld Base %p Limit %p\n"
         "InUse %lld FreeCount %lld FreeHead %lld\n",
         m_iRegionSize,
         m_iBufferSize,
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
  _class_(CN_BUFFERPOOL);
  
  BufferFree* pFreeBuffer = pToBufferFree(pBuffer);
  
  // test for existing free list
  if (bn::nNil() != m_nFreeHead) {
    ASSERT( m_iFreeCount > 0 );
  }
  else {
    ASSERT( ! m_iFreeCount );
  }
  
  // free buffer by pushing onto free chain
  pFreeBuffer->vNextFree(m_nFreeHead);
  m_nFreeHead = pBuffer->nBufferNo();
  
  // adjust counts
  m_iFreeCount++;
  m_iAllocCount--;
}

/*----------------------------------------------------------------------------*/
/* return free space percentage                                               */
/*----------------------------------------------------------------------------*/
i8 BufferPool::iFreePercent(void)
{
  _class_(CN_BUFFERPOOL);
  
  i8 iFreePercent = DIV(iFreeSpace() * 100,iRegionSize());
  
  return iFreePercent;
}

/*----------------------------------------------------------------------------*/
/* return free space in bytes                                                 */
/*----------------------------------------------------------------------------*/
i8 BufferPool::iFreeSpace(void)
{
  _class_(CN_BUFFERPOOL);
  
  i8 iFreeSpace = m_iFreeCount * m_iBufferSize;
  
  return iFreeSpace;
}

/*----------------------------------------------------------------------------*/
/* does the pool have room for more data (row or entries) ?                   */
/*----------------------------------------------------------------------------*/
bool BufferPool::bHasDataSpace(void)
{
  _class_(CN_BUFFERPOOL);
  
  i8   iPercent = iFreePercent();
  
  bool bHasSpace = iPercent > 19 ? true : false;
  
  return bHasSpace;
}

/*----------------------------------------------------------------------------*/
/* initialise all buffers in the pool                                         */
/*----------------------------------------------------------------------------*/
void BufferPool::vInitialise(void)
{
  _class_(CN_BUFFERPOOL);
  
  m_iAllocCount = 0;
  m_iFreeCount  = m_iBufferCount;
  m_nFreeHead   = nBufferFirst();
  
  //vTrace("FirstBuffer %lld LastBuffer %lld\n",*nBufferFirst(),*nBufferLast());
  
//  bn nBuffer = nBufferFirst();
//  for (BufferFree* pFreeBuffer : *this) {
//    pFreeBuffer->vBufferNo(nBuffer);
//    nBuffer++;
//    pFreeBuffer->vNextFree(nBuffer);
//  }

  bn nBuffer = nBufferFirst();
  while (bn::nNil() != nBuffer) {
    BufferFree* pFreeBuffer = pToBufferFree(nBuffer);
    
    pFreeBuffer->vSetMagic();
    pFreeBuffer->vClass(CN_BUFFER);
    pFreeBuffer->vBufferNo(nBuffer);
    nBuffer = nBufferNext(nBuffer);
    pFreeBuffer->vNextFree(nBuffer);
}
  
  BufferFree* pLastBuffer = pToBufferFree(nBufferLast());
  pLastBuffer->vSetMagic();
  pLastBuffer->vClass(CN_BUFFER);
  pLastBuffer->vBufferNo(nBufferLast());
  pLastBuffer->vNextFree(bn::nNil());
  
  //vDump();
}

/*----------------------------------------------------------------------------*/
/* get size in bytes of pool                                                  */
/*----------------------------------------------------------------------------*/
i8 BufferPool::iRegionSize(void) const
{
  _class_(CN_BUFFERPOOL);
  
  return m_iRegionSize;
}

/*----------------------------------------------------------------------------*/
/* transalate buffer number to allocated buffer                               */
/*----------------------------------------------------------------------------*/
Buffer* BufferPool::pToBuffer(bn nBuffer) const
{
  _class_(CN_BUFFERPOOL);
  
  i8 iOffset = m_iBufferSize * *nBuffer;
  
  Buffer* pBuffer = static_cast<Buffer*>(Memory::pAddBytes(m_pBase,iOffset));
  
  return pPoolCheck<Buffer*>(pBuffer);
}

/*----------------------------------------------------------------------------*/
/* transalate buffer number to free buffer                                    */
/*----------------------------------------------------------------------------*/
BufferFree* BufferPool::pToBufferFree(bn nBuffer) const
{
  _class_(CN_BUFFERPOOL);
  
  BufferFree* pBufferFree = static_cast<BufferFree*>(pToBuffer(nBuffer));
  
  return pPoolCheck<BufferFree*>(pBufferFree);
}

/*----------------------------------------------------------------------------*/
/* transalate buffer number to free buffer                                    */
/*----------------------------------------------------------------------------*/
BufferFree* BufferPool::pToBufferFree(Buffer* pBuffer) const
{
  _class_(CN_BUFFERPOOL);
  
  BufferFree* pBufferFree = static_cast<BufferFree*>(pBuffer);
  
  return pPoolCheck<BufferFree*>(pBufferFree);
}

/*----------------------------------------------------------------------------*/
/* get first iteration                                                        */
/*----------------------------------------------------------------------------*/
BufferPoolIter BufferPool::begin(void) const
{
  _class_(CN_BUFFERPOOL);
  
  return BufferPoolIter(this,static_cast<BufferFree*>(m_pBase));
}

/*----------------------------------------------------------------------------*/
/* get last iteration limit                                                   */
/*----------------------------------------------------------------------------*/
BufferPoolIter BufferPool::end(void) const
{
  _class_(CN_BUFFERPOOL);
  
  return BufferPoolIter(this,static_cast<BufferFree*>(m_pLimit));
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/