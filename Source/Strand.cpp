/*============================================================================*/
/* Strand.cpp - a strand of segment buffers                                   */
/*============================================================================*/

#include "MemoryManagement.h"

extern Heap* gpSmallHeap;

/*----------------------------------------------------------------------------*/
/* allocate a strand from small object heap                                   */
/*----------------------------------------------------------------------------*/
void* Strand::operator new(size_t)
{
  Strand* pStrand = static_cast<Strand*>(gpSmallHeap->pAllocObject());
  
  return pStrand;
}

/*----------------------------------------------------------------------------*/
/* release a strand back to the small object heap                             */
/*----------------------------------------------------------------------------*/
void Strand::operator delete(void* pStrandAddress)
{
  Strand* pStrand = static_cast<Strand*>(pStrandAddress);
  
  gpSmallHeap->vFreeObject(pStrand);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Strand::Strand(void)
: Object(CN_STRAND)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STRAND);
  
  m_xLock.vCheckInit();
  
  m_pPrevStrand = nullptr;
  m_pNextStrand = nullptr;
  
  m_pHeadBuffer = nullptr;
  m_pTailBuffer = nullptr;
  
  pFooter()->vSetFooter();
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Strand::~Strand(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STRAND,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* append segment buffer to this strand                                       */
/*----------------------------------------------------------------------------*/
void Strand::vAppendBuffer(SegmentBuffer* pBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STRAND,_FOOTER_);
  
  m_xLock.vGrabWrite();
  
  if (! m_pHeadBuffer) {
    ASSERT(! m_iCountBuffer);
    ASSERT(! m_pTailBuffer);
    
    m_pHeadBuffer = pBuffer;
    m_pTailBuffer = pBuffer;
    
    m_iCountBuffer = 1;
  }
  else {
    ASSERT(m_iCountBuffer);
    ASSERT(m_pTailBuffer);
    
    m_pTailBuffer->vNextBuffer(pBuffer);
    pBuffer->vPrevBuffer(m_pTailBuffer);
    
    m_pTailBuffer = pBuffer;
    
    m_iCountBuffer++;
  }
  
  m_xLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* first buffer                                                               */
/*----------------------------------------------------------------------------*/
SegmentBuffer* Strand::pFirstBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEGMENTBUFFER,_FOOTER_);

  return m_pHeadBuffer;
}

/*----------------------------------------------------------------------------*/
/* pop buffer                                                                 */
/*----------------------------------------------------------------------------*/
SegmentBuffer* Strand::pPopBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEGMENTBUFFER,_FOOTER_);
  
  SegmentBuffer* pBuffer = nullptr;
  
  m_xLock.vGrabWrite();
  
  if (! m_pHeadBuffer) {
    m_xLock.vDropWrite();
    return nullptr;
  }
  
  pBuffer = m_pHeadBuffer;
  m_pHeadBuffer = m_pHeadBuffer->pNextBuffer();
  if (m_pHeadBuffer)
    m_pHeadBuffer->vPrevBuffer(nullptr);
  else
    m_pTailBuffer = nullptr;
  
  m_iCountBuffer--;
  
  m_xLock.vDropWrite();

  return pBuffer;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* calculate location of footer                                               */
/*----------------------------------------------------------------------------*/
Footer* Strand::pFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEGMENTBUFFER);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/