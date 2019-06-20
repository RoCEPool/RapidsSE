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
BufferCursor::BufferCursor(IntBuffer* pIntBuffer, i8* aElement)
: Object(CN_BUFFERCURSOR)
{
  m_pIntBuffer = pIntBuffer;
  m_aElement   = aElement;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
BufferCursor::BufferCursor(BufferCursor* pBufferCursor)
: Object(CN_BUFFERCURSOR)
{
  m_pIntBuffer = pBufferCursor->m_pIntBuffer;
  m_aElement   = pBufferCursor->m_aElement;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
BufferCursor::BufferCursor(BufferCursor* pLowCursor, BufferCursor* pHighCursor)
: Object(CN_BUFFERCURSOR)
{
  m_pIntBuffer = pLowCursor->m_pIntBuffer;
  m_aElement     = pLowCursor->m_aElement + (pHighCursor->m_aElement - pLowCursor->m_aElement) / 2;
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
  m_aElement--;
}

/*----------------------------------------------------------------------------*/
/* Move to the next entry                                                     */
/*----------------------------------------------------------------------------*/
void BufferCursor::vMoveRight(void)
{
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
