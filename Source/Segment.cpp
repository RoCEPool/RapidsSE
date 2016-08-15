/*============================================================================*/
/* Segment.cpp - segmented space allocator                                    */
/*============================================================================*/

#include "MemoryManagement.h"

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Segment::operator new(size_t)
{
  vFatal("fatal: Segment class cannot be new'd");
  
  //vExitFatal();
  
  exit(EXIT_FAILURE);
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void Segment::operator delete(void*)
{
  vFatal("fatal: Segment class cannot be deleted");
  
  //vExitFatal();
  
  exit(EXIT_FAILURE);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Segment::Segment(eCN eClass)
: Object(eClass)
{
  m_zLock.vCheckInit();
  m_iCount       = 0;
  
  m_pFirstBuffer = nullptr;
  m_pLastBuffer  = nullptr;

  m_pHeadStrand = nullptr;
  m_pTailStrand = nullptr;
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Segment::~Segment(void)
{
}

/*----------------------------------------------------------------------------*/
/* allocate an object in this segmented space                                 */
/*----------------------------------------------------------------------------*/
Object* Segment::pAllocObject(size_t nBytes)
{
  _poolAddress_(this);
  
  m_zLock.vGrabWrite();
  
  if ( ! m_pLastBuffer )
    vExtendSpace();

  Object* pObject = m_pLastBuffer->pAllocObject(nBytes);
  
  if ( ! pObject ) {
    vExtendSpace();
    pObject = m_pLastBuffer->pAllocObject(nBytes);
  }
  
  m_zLock.vDropWrite();

  return pObject;
}

/*----------------------------------------------------------------------------*/
/* extend this segmented space                                                */
/*----------------------------------------------------------------------------*/
void Segment::vExtendSpace(void)
{
  _poolAddress_(this);

  SegmentBuffer* pNewBuffer = reinterpret_cast<SegmentBuffer*>(new SegmentBuffer);
  
  //m_zLock.vGrabWrite();

  if (! m_pFirstBuffer) {
    ASSERT(! m_iCount);
    ASSERT(! m_pLastBuffer);
    
    m_pFirstBuffer = pNewBuffer;
    m_pLastBuffer  = pNewBuffer;
    
    m_iCount = 1;
  }
  else {
    ASSERT(m_iCount);
    ASSERT(m_pLastBuffer);

    m_pLastBuffer->vNextBuffer(pNewBuffer);
    pNewBuffer->vPrevBuffer(m_pLastBuffer);
    
    m_pLastBuffer = pNewBuffer;
    
    m_iCount++;
  }
  
  //m_zLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* get first buffer                                                           */
/*----------------------------------------------------------------------------*/
SegmentBuffer* Segment::pFirstBuffer(void)
{
  _poolAddress_(this);
  
  return m_pFirstBuffer;
}

/*----------------------------------------------------------------------------*/
/* get lasy buffer                                                           */
/*----------------------------------------------------------------------------*/
SegmentBuffer* Segment::pLastBuffer(void)
{
  _poolAddress_(this);
  
  return m_pLastBuffer;
}

/*----------------------------------------------------------------------------*/
/* get first object                                                           */
/*----------------------------------------------------------------------------*/
Object* Segment::pFirstObject(void)
{
  _poolAddress_(this);
  
  if (! m_pFirstBuffer )
    return nullptr;
  
  Object* pFirst = m_pFirstBuffer->pFirst();

  return pFirst;
}

/*----------------------------------------------------------------------------*/
/* get next object                                                            */
/*----------------------------------------------------------------------------*/
Object* Segment::pNextObject(Object* pCurr, i8 nBytes)
{
  _poolAddress_(this);
  
  SegmentBuffer* pBuffer = (SegmentBuffer*)( ((i8)pCurr / C_SIZE_LARGE_BUFFER) * C_SIZE_LARGE_BUFFER );
  
  Object* pNext = pBuffer->pNext(pCurr,nBytes);
  
  if (! pNext) {
    pBuffer = pBuffer->pNextBuffer();
    if (pBuffer)
      pNext = pBuffer->pFirst();
    else
      pNext = nullptr;
  }
  
  return pNext;
}

/*----------------------------------------------------------------------------*/
/* get buffer count                                                           */
/*----------------------------------------------------------------------------*/
i8 Segment::iBufferCount(void)
{
  _poolAddress_(this);
  
  return m_iCount;
}

/*----------------------------------------------------------------------------*/
/* release all segements                                                      */
/*----------------------------------------------------------------------------*/
void Segment::vReleaseAll(void)
{
  _poolAddress_(this);
  
  m_zLock.vGrabWrite();
  
  SegmentBuffer* pBuffer;
  SegmentBuffer* pNextBuffer;
  
  pBuffer = m_pFirstBuffer;
  while (pBuffer) {
    pNextBuffer = pBuffer->pNextBuffer();
    delete pBuffer;
    pBuffer = pNextBuffer;
  }
  
  m_iCount = 0;
  m_pFirstBuffer = nullptr;
  m_pLastBuffer  = nullptr;
  
  m_zLock.vDropWrite();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/