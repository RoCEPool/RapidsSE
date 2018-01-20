/*============================================================================*/
/* PerformanceBuffer.cpp -- Performance Buffer                                */
/*============================================================================*/

#include "DBobjects.h"

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
PerformanceBuffer* DBobjects::_pPerformanceBuffer(void* aAddr)
{
  PerformanceBuffer* pPerformanceBuffer = reinterpret_cast<PerformanceBuffer*>(aAddr);
  
  RASSERT( !pPerformanceBuffer || CN_PERFORMANCEBUFFER == pPerformanceBuffer->eClass());
  
  return pPerformanceBuffer;
}

/*----------------------------------------------------------------------------*/
/* fetch a new performance buffer from buffer pool                            */
/*----------------------------------------------------------------------------*/
void* PerformanceBuffer::operator new(size_t iSize)
{
  RASSERT(iSize);
  Buffer* pBuffer = (Buffer*)Heap::pAllocate(CN_PERFORMANCEBUFFER);
  
  return pBuffer;
}

/*----------------------------------------------------------------------------*/
/* return a performance buffer to the buffer pool                             */
/*----------------------------------------------------------------------------*/
void PerformanceBuffer::operator delete(void* pBufferAddress)
{
  Buffer* pBuffer = static_cast<Buffer*>(pBufferAddress);
  
  Heap::vFree(pBuffer);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
PerformanceBuffer::PerformanceBuffer(void)
: ItemBuffer(CN_PERFORMANCEBUFFER)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PERFORMANCEBUFFER,_FOOTER_);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_PERFORMANCEBUFFER,_FOOTER_);
  
  _checkInitialised_(CN_PERFORMANCEBUFFER);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
PerformanceBuffer::~PerformanceBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PERFORMANCEBUFFER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* PerformanceBuffer::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PERFORMANCEBUFFER);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
