/*============================================================================*/
/* SegmentBuffer.cpp - a segment bufferwithin a segmented space               */
/*============================================================================*/

#include "MemoryManagement.h"

extern BufferPool* gpBufferPool;

/*----------------------------------------------------------------------------*/
/* fetch a new buffer from buffer pool                                        */
/*----------------------------------------------------------------------------*/
void* SegmentBuffer::operator new(size_t)
{
  SegmentBuffer* pSegment = static_cast<SegmentBuffer*>(gpBufferPool->pAllocBuffer());
  
  return pSegment;
}

/*----------------------------------------------------------------------------*/
/* return a segment to the buffer pool                                        */
/*----------------------------------------------------------------------------*/
void SegmentBuffer::operator delete(void* pSegmentAddress)
{
  Buffer* pBuffer = static_cast<Buffer*>(pSegmentAddress);
  
  gpBufferPool->vFree(pBuffer);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
SegmentBuffer::SegmentBuffer(void)
: Buffer(CN_SEGMENTBUFFER)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEGMENTBUFFER);
  
  m_bHasSpace   = true;
  m_pFreeSpace  = pFirstObject();
  m_pPrevBuffer = nullptr;
  m_pNextBuffer = nullptr;
  
  pFooter()->vSetFooter();
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
SegmentBuffer::~SegmentBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEGMENTBUFFER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* allocate an object in this segmented space                                 */
/*----------------------------------------------------------------------------*/
Object* SegmentBuffer::pAllocObject(size_t nBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEGMENTBUFFER,_FOOTER_);
  
  // was exhausted on last call?
  if (bIsExhausted())
    return nullptr;
  
  // test  sufficient freespace
  if (iFreeSpace() < nBytes) {
    vSetExhausted();
    return nullptr;
  }
  
  Object* pObject = m_pFreeSpace;
  
  m_pFreeSpace = static_cast<Object*>(pvAdd(m_pFreeSpace,nBytes));
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* first object                                                               */
/*----------------------------------------------------------------------------*/
Object* SegmentBuffer::pFirst(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEGMENTBUFFER,_FOOTER_);
  
  Object* pFirst = pFirstObject();
  
  if (pFirst < m_pFreeSpace)
    return pFirst;
  else
    return nullptr;
}

/*----------------------------------------------------------------------------*/
/* next object                                                                */
/*----------------------------------------------------------------------------*/
Object* SegmentBuffer::pNext(Object* pCurr, i8 nBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEGMENTBUFFER,_FOOTER_);
  
  Object* pNext = static_cast<Object*>(pvAdd(pCurr,nBytes));
  
  if (pNext < m_pFreeSpace)
    return pNext;
  else
    return nullptr;
}

/*----------------------------------------------------------------------------*/
/* get next buffer                                                            */
/*----------------------------------------------------------------------------*/
SegmentBuffer* SegmentBuffer::pNextBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEGMENTBUFFER,_FOOTER_);
  
  return m_pNextBuffer;
}

/*----------------------------------------------------------------------------*/
/* set next buffer                                                            */
/*----------------------------------------------------------------------------*/
void SegmentBuffer::vNextBuffer(SegmentBuffer* pNextBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEGMENTBUFFER,_FOOTER_);
  
  m_pNextBuffer = pNextBuffer;
}

/*----------------------------------------------------------------------------*/
/* get prev buffer                                                            */
/*----------------------------------------------------------------------------*/
SegmentBuffer* SegmentBuffer::pPrevBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEGMENTBUFFER,_FOOTER_);
  
  return m_pPrevBuffer;
}

/*----------------------------------------------------------------------------*/
/* set prev buffer                                                            */
/*----------------------------------------------------------------------------*/
void SegmentBuffer::vPrevBuffer(SegmentBuffer* pPrevBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEGMENTBUFFER,_FOOTER_);
  
  m_pPrevBuffer = pPrevBuffer;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* first allocatable byte in segment                                          */
/*----------------------------------------------------------------------------*/
Object* SegmentBuffer::pFirstObject(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEGMENTBUFFER,_FOOTER_);
  
  Object* pStart = static_cast<Object*>(pvAdd(this,sizeof(SegmentBuffer)));
  
  return pStart;
}

/*----------------------------------------------------------------------------*/
/* calculate location of footer                                               */
/*----------------------------------------------------------------------------*/
Footer* SegmentBuffer::pFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEGMENTBUFFER);
  
  Footer* pFooter = static_cast<Footer*>(pvAdd(this,C_SIZE_LARGE_BUFFER-sizeof(Footer)));
  
  return pFooter;
}

/*----------------------------------------------------------------------------*/
/* how much free space (in bytes) left in segment?                            */
/*----------------------------------------------------------------------------*/
i8 SegmentBuffer::iFreeSpace(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEGMENTBUFFER,_FOOTER_);
  
  i8 nBytes = iSub(pLastByte(),m_pFreeSpace);
  
  return nBytes;
}

/*----------------------------------------------------------------------------*/
/* has this buffer exhausted its space?                                       */
/*----------------------------------------------------------------------------*/
bool SegmentBuffer::bIsExhausted(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEGMENTBUFFER,_FOOTER_);
  
  return m_bHasSpace ? false : true;
}

/*----------------------------------------------------------------------------*/
/* this buffer has exhausted its space                                        */
/*----------------------------------------------------------------------------*/
void SegmentBuffer::vSetExhausted(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEGMENTBUFFER,_FOOTER_);
  
  m_bHasSpace = false;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/