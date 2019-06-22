/*============================================================================*/
/* BufferCursor.cpp -- Buffer Cursor                                          */
/*============================================================================*/

#include "Data.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
BufferCursor* Data::_pBufferCursor(void* aAddr)
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
BufferCursor::BufferCursor(IntBuffer* pIntBuffer, i8 iBufferNo, i8* aElement)
: Object(CN_BUFFERCURSOR)
{
  m_pIntBuffer = pIntBuffer;
  m_iBufferNo  = iBufferNo;
  m_aElement   = aElement;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
BufferCursor::BufferCursor(BufferCursor* pBufferCursor)
: Object(CN_BUFFERCURSOR)
{
  m_pIntBuffer = pBufferCursor->m_pIntBuffer;
  m_iBufferNo  = pBufferCursor->m_iBufferNo;
  m_aElement   = pBufferCursor->m_aElement;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
BufferCursor::BufferCursor(BufferCursor* pLowCursor, BufferCursor* pHighCursor)
: Object(CN_BUFFERCURSOR)
{
  
  if (pLowCursor->m_pIntBuffer == pHighCursor->m_pIntBuffer) {
    m_pIntBuffer = pLowCursor->m_pIntBuffer;
    m_iBufferNo = pLowCursor->m_iBufferNo;
    m_aElement   = pLowCursor->m_aElement + (pHighCursor->m_aElement - pLowCursor->m_aElement) / 2;
    return;
  }
  
  vTrace("Low %lld High %lld\n"_Stz,
         *pLowCursor->m_aElement,
         *pHighCursor->m_aElement);
  
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
void BufferCursor::vMoveLeft(void)
{
  if (m_aElement == m_pIntBuffer->aGetElement(0)) {
    // move to a previous buffer
    m_pIntBuffer = m_pIntBuffer->pPrevBuffer();
    m_iBufferNo--;
    m_aElement = m_pIntBuffer->aGetElement(m_pIntBuffer->nCount() -1);
    return;
  }
  
  // move left
  m_aElement--;
  
}

/*----------------------------------------------------------------------------*/
/* Move to the next entry                                                     */
/*----------------------------------------------------------------------------*/
void BufferCursor::vMoveRight(void)
{
  if (m_aElement == m_pIntBuffer->aGetElement(m_pIntBuffer->nCount() -1)) {
    // move to a next buffer
    m_pIntBuffer = m_pIntBuffer->pNextBuffer();
    m_iBufferNo++;
    m_aElement = m_pIntBuffer->aGetElement(0);
    return;
  }
  
  // move right
  m_aElement++;
}

/*----------------------------------------------------------------------------*/
/* swap elements of the cursor                                                */
/*----------------------------------------------------------------------------*/
void BufferCursor::vSwap(BufferCursor* pBufferCursor)
{
  i8 iTemp = *pBufferCursor->m_aElement;
  *pBufferCursor->m_aElement = *m_aElement;
  *m_aElement = iTemp;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
