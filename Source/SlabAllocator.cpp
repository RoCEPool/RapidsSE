/*============================================================================*/
/* SlabAllocator.cpp - slab allocator                                         */
/*============================================================================*/

#include "Pool.h"

/*----------------------------------------------------------------------------*/
/* fetch a new buffer from buffer pool                                        */
/*----------------------------------------------------------------------------*/
void* SlabAllocator::operator new(size_t)
{
  PoolBuffer* pBuffer = (PoolBuffer*)HeapMem::pAllocate(CN_SLABALLOCATOR);
  
  return pBuffer;
}

/*----------------------------------------------------------------------------*/
/* return a row buffer to the buffer pool                                     */
/*----------------------------------------------------------------------------*/
void SlabAllocator::operator delete(void* pRowBufferAddress)
{
  PoolBuffer* pBuffer = static_cast<PoolBuffer*>(pRowBufferAddress);
  
  HeapMem::vFree(pBuffer);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
SlabAllocator::SlabAllocator(void)
: Object(CN_SLABALLOCATOR)
{
  _magic_();
  
  m_pFirstSlab = nullptr;
  
  _checkInitialised_(CN_SLABALLOCATOR);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
SlabAllocator::~SlabAllocator(void) noexcept(false)
{
  _magic_();
  
  Slab* pSlabTbd = m_pFirstSlab;
  while (pSlabTbd) {
    Slab* pSlabNxt = pSlabTbd->pNextSlab();
    delete pSlabTbd;
    pSlabTbd = pSlabNxt;
  }
  
  m_pFirstSlab = nullptr;
}

/*----------------------------------------------------------------------------*/
/* allot given number of bytes                                                */
/*----------------------------------------------------------------------------*/
char* SlabAllocator::aAllotBytes(i8 nBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SLABALLOCATOR,_FOOTER_);
  
  if (! m_pFirstSlab)
    m_pFirstSlab = new Slab;
  
  char* aBytes = m_pFirstSlab->aAllotBytes(nBytes);
  if (aBytes)
    return aBytes;
  
  Slab* pNewFirstSlab = new Slab;
  pNewFirstSlab->vNextSlab(m_pFirstSlab);
  RASSERT(pNewFirstSlab);
  
  m_pFirstSlab = pNewFirstSlab;
  
  aBytes = m_pFirstSlab->aAllotBytes(nBytes);
  ASSERT(aBytes);
  
  return aBytes;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* SlabAllocator::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SLABALLOCATOR);
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
