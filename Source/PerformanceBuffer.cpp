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
  Buffer* pBuffer = reinterpret_cast<Buffer*>(pBufferAddress);
  
  Heap::vFree(pBuffer);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
PerformanceBuffer::PerformanceBuffer(void)
: ItemBuffer(CN_PERFORMANCEBUFFER)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PERFORMANCEBUFFER,_FOOTER_);
  
  m_pNextBuffer = nullptr;
  m_pPrevBuffer = nullptr;
  
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
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* allot new performance entry                                                */
/*----------------------------------------------------------------------------*/
PerformanceEntry* PerformanceBuffer::pAllotEntry(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PERFORMANCEBUFFER,_FOOTER_);
  
  PerformanceEntry* pEntry =
    reinterpret_cast<PerformanceEntry*>(pAllotItem(sizeof(PerformanceEntry)));
  
  if ( ! pEntry )
    return nullptr;
  
  new(pEntry) PerformanceEntry;
  
  return pEntry;
}

/*----------------------------------------------------------------------------*/
/* get next buffer                                                            */
/*----------------------------------------------------------------------------*/
PerformanceBuffer* PerformanceBuffer::pNextBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PERFORMANCEBUFFER,_FOOTER_);
  
  return m_pNextBuffer;
}

/*----------------------------------------------------------------------------*/
/* set next buffer                                                            */
/*----------------------------------------------------------------------------*/
void PerformanceBuffer::vNextBuffer(PerformanceBuffer* pNextBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PERFORMANCEBUFFER,_FOOTER_);
  
  m_pNextBuffer = pNextBuffer;
}

/*----------------------------------------------------------------------------*/
/* get prev buffer                                                            */
/*----------------------------------------------------------------------------*/
PerformanceBuffer* PerformanceBuffer::pPrevBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PERFORMANCEBUFFER,_FOOTER_);
  
  return m_pPrevBuffer;
}

/*----------------------------------------------------------------------------*/
/* set prev buffer                                                            */
/*----------------------------------------------------------------------------*/
void PerformanceBuffer::vPrevBuffer(PerformanceBuffer* pPrevBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PERFORMANCEBUFFER,_FOOTER_);
  
  m_pPrevBuffer = pPrevBuffer;
}

/*----------------------------------------------------------------------------*/
/* get first performance entry                                                */
/*----------------------------------------------------------------------------*/
PerformanceEntry* PerformanceBuffer::pFirstEntry(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  PerformanceEntry* pFirst = reinterpret_cast<PerformanceEntry*>(pFirstItem());
  
  return pFirst;
}

/*----------------------------------------------------------------------------*/
/* get next performance entry                                                 */
/*----------------------------------------------------------------------------*/
PerformanceEntry* PerformanceBuffer::pNextEntry(PerformanceEntry* pCurrentEntry)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PERFORMANCEBUFFER,_FOOTER_);
  
  PerformanceEntry* pNext = reinterpret_cast<PerformanceEntry*>(pNextItem(pCurrentEntry));
  
  return pNext;
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
