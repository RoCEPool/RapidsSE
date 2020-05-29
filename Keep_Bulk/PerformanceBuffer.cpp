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
PerformanceBuffer* Measurement::_pPerformanceBuffer(void* aAddr)
{
  PerformanceBuffer* pPerformanceBuffer = reinterpret_cast<PerformanceBuffer*>(aAddr);
  
  RASSERT_TYPE(pPerformanceBuffer,CN_PERFORMANCEBUFFER);
  
  return pPerformanceBuffer;
}

/*----------------------------------------------------------------------------*/
/* fetch a new performance buffer from buffer pool                            */
/*----------------------------------------------------------------------------*/
void* PerformanceBuffer::operator new(size_t)
{
  Buffer* pBuffer = (Buffer*)HeapMem::pAllocate(CN_PERFORMANCEBUFFER);
  
  return pBuffer;
}

/*----------------------------------------------------------------------------*/
/* return a performance buffer to the buffer pool                             */
/*----------------------------------------------------------------------------*/
void PerformanceBuffer::operator delete(void* pBufferAddress)
{
  Buffer* pBuffer = reinterpret_cast<Buffer*>(pBufferAddress);
  
  HeapMem::vFree(pBuffer);
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
PerformanceBuffer::~PerformanceBuffer(void) noexcept(false)
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
  
  PerformanceEntry* aEntry =
    reinterpret_cast<PerformanceEntry*>(aAllotItem(sizeof(PerformanceEntry)));
  
  if ( ! aEntry )
    return nullptr;
  
  new(aEntry) PerformanceEntry;
  
  return aEntry;
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
  
  PerformanceEntry* aFirst = reinterpret_cast<PerformanceEntry*>(aFirstItem());
  
  return aFirst;
}

/*----------------------------------------------------------------------------*/
/* get next performance entry                                                 */
/*----------------------------------------------------------------------------*/
PerformanceEntry* PerformanceBuffer::pNextEntry(PerformanceEntry* aCurrentEntry)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PERFORMANCEBUFFER,_FOOTER_);
  
  PerformanceEntry* aNext = reinterpret_cast<PerformanceEntry*>(aNextItem(aCurrentEntry));
  
  return aNext;
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
