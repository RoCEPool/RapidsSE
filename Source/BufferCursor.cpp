/*============================================================================*/
/* BufferCursor.cpp -- Buffer Cursor                                          */
/*============================================================================*/

#include "SortBuffers.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
BufferCursor* SortBuffers::_pBufferCursor(void* aAddr)
{
  BufferCursor* pBufferCursor = reinterpret_cast<BufferCursor*>(aAddr);
  
  RASSERT_TYPE(pBufferCursor,CN_BUFFERCURSOR);
  
  return pBufferCursor;
}

/*----------------------------------------------------------------------------*/
/* allocate a new row buffer from heap                                        */
/*----------------------------------------------------------------------------*/
void* BufferCursor::operator new(size_t)
{
  BufferCursor* pBufferCursor =
  static_cast<BufferCursor*>(HeapMem::pAllocate(CN_BUFFERCURSOR));
  
  return pBufferCursor;
}

/*----------------------------------------------------------------------------*/
/* perform new of row buffer for given address                                */
/*----------------------------------------------------------------------------*/
void* BufferCursor::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free a row buffer back to the heap                                         */
/*----------------------------------------------------------------------------*/
void BufferCursor::operator delete(void* aAddress)
{
  IntBuffer* pIntBuffer = static_cast<IntBuffer*>(aAddress);
  
  HeapMem::vFree(pIntBuffer);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
BufferCursor::BufferCursor(SrtBuffer* pSrtBuffer, i8 iBufferNo, i8 iElement)
: Object(CN_BUFFERCURSOR)
{
  m_pSrtBuffer = pSrtBuffer;
  m_iBufferNo  = iBufferNo;
  m_iElement   = iElement;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
BufferCursor::BufferCursor(BufferCursor* pBufferCursor)
: Object(CN_BUFFERCURSOR)
{
  m_pSrtBuffer = pBufferCursor->m_pSrtBuffer;
  m_iBufferNo  = pBufferCursor->m_iBufferNo;
  m_iElement   = pBufferCursor->m_iElement;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
BufferCursor::BufferCursor(BufferCursor* pLowCursor, BufferCursor* pHighCursor)
: Object(CN_BUFFERCURSOR)
{
  
  if (pLowCursor->m_pSrtBuffer == pHighCursor->m_pSrtBuffer) {
    m_pSrtBuffer = pLowCursor->m_pSrtBuffer;
    m_iBufferNo = pLowCursor->m_iBufferNo;
    m_iElement   = pLowCursor->m_iElement + (pHighCursor->m_iElement - pLowCursor->m_iElement) / 2;
    return;
  }
  
  vTrace("Low %lld High %lld\n"_Stz,
         pLowCursor->m_iElement,
         pHighCursor->m_iElement);
  
  RASSERT(0);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
BufferCursor::~BufferCursor(void) noexcept(false)
{
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Move to the previous entry                                                 */
/*----------------------------------------------------------------------------*/
bool BufferCursor::bMoveLeft(void)
{
  if (m_iElement == 0) {
    // move to a previous buffer
      if (m_pSrtBuffer->pPrevBuffer()) {
      m_pSrtBuffer = m_pSrtBuffer->pPrevBuffer();
      m_iBufferNo--;
      m_iElement = m_pSrtBuffer->nItems() -1;
      return true;
      }
    else
      return false;
  }
  
  // move left
  m_iElement--;
  return true;
}

/*----------------------------------------------------------------------------*/
/* Move to the next entry                                                     */
/*----------------------------------------------------------------------------*/
bool BufferCursor::bMoveRight(void)
{
  if (m_iElement == m_pSrtBuffer->nItems() -1) {
    
    if (m_pSrtBuffer->pNextBuffer()) {
      // move to a next buffer
      m_pSrtBuffer = m_pSrtBuffer->pNextBuffer();
      m_iBufferNo++;
      m_iElement = 0;
      return true;
    }
    else
      return false;
  }
  
  // move right
  m_iElement++;
  return true;
}

/*----------------------------------------------------------------------------*/
/* get sort set                                                               */
/*----------------------------------------------------------------------------*/
SrtSet* BufferCursor::aSrtSet(void)
{
  return m_pSrtBuffer->aSrtSet(m_iElement);
}

/*----------------------------------------------------------------------------*/
/* swap elements of the cursor                                                */
/*----------------------------------------------------------------------------*/
void BufferCursor::vSwap(BufferCursor* pBufferCursor)
{
  m_pSrtBuffer->vSwap(m_iElement, pBufferCursor->m_pSrtBuffer, pBufferCursor->m_iElement);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
