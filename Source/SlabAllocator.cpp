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
  
  Slab* pSlab_tbd = m_pFirstSlab;
  while (pSlab_tbd) {
    Slab* pSlabNxt = pSlab_tbd->pNextSlab();
    delete pSlab_tbd;
    pSlab_tbd = pSlabNxt;
  }
  
  m_pFirstSlab = nullptr;
}

/*----------------------------------------------------------------------------*/
/* allot given number of bytes                                                */
/*----------------------------------------------------------------------------*/
byte* SlabAllocator::ayAllotBytes(i8 nBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SLABALLOCATOR,_FOOTER_);
  
  if (! m_pFirstSlab)
    m_pFirstSlab = new Slab;
  
  byte* ayBytes = m_pFirstSlab->ayAllotBytes(nBytes);
  if (ayBytes)
    return ayBytes;
  
  Slab* pNewFirstSlab = new Slab;
  pNewFirstSlab->vNextSlab(m_pFirstSlab);
  RASSERT(pNewFirstSlab);
  
  m_pFirstSlab = pNewFirstSlab;
  
  ayBytes = m_pFirstSlab->ayAllotBytes(nBytes);
  ASSERT(ayBytes);
  
  return ayBytes;
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
