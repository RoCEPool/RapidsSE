/*============================================================================*/
/* Queue.cpp -- Queue of Values                                               */
/*============================================================================*/

#include "Resources.h"

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Queue* Resources::_pQueue(void* aAddr)
{
  Queue* pQueue = reinterpret_cast<Queue*>(aAddr);
  
  RASSERT_TYPE(pQueue,CN_QUEUE);
  
  return pQueue;
}

/*----------------------------------------------------------------------------*/
/* new operator                                                               */
/*----------------------------------------------------------------------------*/
void* Queue::operator new(size_t)
{
  void* pObject = HeapMem::pAllocate(CN_QUEUE);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* new operator                                                               */
/*----------------------------------------------------------------------------*/
void* Queue::operator new(size_t, Queue* pQueue)
{
  return pQueue;
}

/*----------------------------------------------------------------------------*/
/* delete operator                                                            */
/*----------------------------------------------------------------------------*/
void Queue::operator delete(void* pQueueAddress)
{
  Queue* pQueue = reinterpret_cast<Queue*>(pQueueAddress);
  
  HeapMem::vFree(pQueue);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Queue::Queue(void)
: Object(CN_QUEUE)
, m_okLock(LK_QUEUE,this)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUE,_FOOTER_);
  
  m_iCount = 0;
  m_pTopBuffer = nullptr;
  m_pEndBuffer = nullptr;

  _checkInitialised_(CN_QUEUE);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_QUEUE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Queue::~Queue(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUE,_FOOTER_);
  
  while (m_pTopBuffer)
    vDiscard();
    
  ASSERT( ! m_pEndBuffer );
  ASSERT( ! m_pTopBuffer );
}

/*----------------------------------------------------------------------------*/
/* append object pointer to this queue                                        */
/*----------------------------------------------------------------------------*/
void Queue::vAppend(Object* pObject)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUE,_FOOTER_);
  
  m_okLock.vGrabWrite();
  
  if ( ! m_pEndBuffer ) {
    ASSERT(0 == m_iCount);
    // need to add first buffer
    vExpand();
  }
  
  // atempt to add into
  bool bAppended = m_pEndBuffer->bAppend(pObject);
  
  if ( ! bAppended ) {
    vExpand();
    bool b = m_pEndBuffer->bAppend(pObject);
    ASSERT(b);
  }
  
  m_iCount++;
  
  m_okLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* get number of values in queue                                              */
/*----------------------------------------------------------------------------*/
i8 Queue::iCount(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUE,_FOOTER_);
  
  return m_iCount;
}

/*----------------------------------------------------------------------------*/
/* dump the queue                                                             */
/*----------------------------------------------------------------------------*/
void Queue::vContentDump(Stz* aTab)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUE,_FOOTER_);
  
  vTrace("%sQueue: %p\n"_Stz, aTab, this);
  vTrace("%s  Count %lld\n"_Stz, aTab, m_iCount);
  vTrace("%s  TopBuffer %p\n"_Stz, aTab, m_pTopBuffer);
  vTrace("%s  EndBuffer %p\n"_Stz, aTab, m_pEndBuffer);
  
  for (QueueBuffer* pQbuffer = m_pTopBuffer; pQbuffer; pQbuffer = pQbuffer->pNext())
    pQbuffer->vContentDump(aTab);
}

/*----------------------------------------------------------------------------*/
/* dump grid                                                                  */
/*----------------------------------------------------------------------------*/
void Queue::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Queue* pQueue = _pQueue(pObject);
  
  vTrace("count %lld top %p end %p\n"_Stz,
         pQueue->m_iCount,
         pQueue->m_pTopBuffer,
         pQueue->m_pEndBuffer);
}

/*----------------------------------------------------------------------------*/
/* pop value from this queue (there may not be any)                           */
/*----------------------------------------------------------------------------*/
Object* Queue::pPop(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUE,_FOOTER_);
  
  
  m_okLock.vGrabWrite();
  
  // are there queue buffers?
  if (! m_pTopBuffer) {
    ASSERT(! m_iCount);
    m_okLock.vDropWrite();
    return nullptr;
  }
  
  // attempt to pop a pointer from top buffer,
  // buffer may be empty
  bool bDiscardable;
  Object* pObject = m_pTopBuffer->pPop(bDiscardable);

  // is the top buffer discardable?
  if (bDiscardable) {
    ASSERT(pObject);
    vDiscard();
  }
  
  // a pointer was popped from a buffer,
  // therefore reduce the total number of object pointers in the queue
  if (pObject)
    m_iCount--;
  ASSERT(m_iCount >= 0);
  
  m_okLock.vDropWrite();
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* discard top qbuffer (should be empty)                                      */
/*----------------------------------------------------------------------------*/
void Queue::vDiscard(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUE,_FOOTER_);
  
  QueueBuffer* pQueueBufferTBD = m_pTopBuffer;
  
  // ALKA CHECK WHEN INTEGRATING WITH RDP
  //ASSERT(0 == pQueueBufferTBD->iCount());
 
  m_pTopBuffer = m_pTopBuffer->pNext();
  if ( ! m_pTopBuffer )
    m_pEndBuffer = nullptr;
  
  delete pQueueBufferTBD;
}

/*----------------------------------------------------------------------------*/
/* extend queue by adding buffer                                              */
/*----------------------------------------------------------------------------*/
void Queue::vExpand(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUE,_FOOTER_);
  
  //vTrace("Queue::vExpand\n"_Stz);
  
  QueueBuffer* pNewQueueBuffer = new QueueBuffer;
  
  if (m_pEndBuffer)
    m_pEndBuffer->vNext(pNewQueueBuffer);
  
  m_pEndBuffer = pNewQueueBuffer;
  
  if ( ! m_pTopBuffer )
    m_pTopBuffer = pNewQueueBuffer;
}

/*----------------------------------------------------------------------------*/
/* verify the queue                                                           */
/*----------------------------------------------------------------------------*/
void Queue::vVerify(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUE,_FOOTER_);
  
  i8 iCount = 0;
  
  for (QueueBuffer* pQbuffer = m_pTopBuffer; pQbuffer; pQbuffer->pNext()) {
    iCount += pQbuffer->iCount();
    pQbuffer->vVerify();
  }
  
  ASSERT(iCount == m_iCount);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Queue::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUE);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
