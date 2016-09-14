/*============================================================================*/
/* Queue.cpp - queue of values                                                */
/*============================================================================*/

#include "Tasks.h"

extern BufferPool*  gpBufferPool;

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Queue* Structures::_pQueue(void* pAddr)
{
  Queue* pQueue = reinterpret_cast<Queue*>(pAddr);
  
  return pQueue;
}

/*----------------------------------------------------------------------------*/
/* new operator                                                               */
/*----------------------------------------------------------------------------*/
void* Queue::operator new(size_t iSize)
{
  ASSERT(iSize);
  void* pObject = gpSmallHeap->pAllocObject(CN_QUEUE);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* new operator                                                               */
/*----------------------------------------------------------------------------*/
void* Queue::operator new(size_t iSize, Queue* pQueue)
{
  ASSERT(iSize);
  return pQueue;
}

/*----------------------------------------------------------------------------*/
/* delete operator                                                            */
/*----------------------------------------------------------------------------*/
void Queue::operator delete(void* pQueueAddress)
{
  Queue* pQueue = reinterpret_cast<Queue*>(pQueueAddress);
  
  gpSmallHeap->vFreeObject(pQueue);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Queue::Queue(void)
: Object(CN_QUEUE)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUE,_FOOTER_);
  
  vInitialize();
  
  Memory::vCheckInitialised(this,sizeof(Queue));
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_QUEUE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Queue::~Queue(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* append value to this queue                                                 */
/*----------------------------------------------------------------------------*/
bool Queue::bAppend(Value* pValue)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUE,_FOOTER_);
  
  m_zLock.vGrabWrite();
  
  if ( ! m_pEndBuffer ) {
    ASSERT(0 == m_iCount);
    // need to add first buffer
    vExpand();
  }

  // atempt to add into
  bool bAppended = m_pEndBuffer->bAppend(pValue);
  
  if ( ! bAppended ) {
    vExpand();
    bool b = m_pEndBuffer->bAppend(pValue);
    ASSERT(b);
  }
  
  m_iCount++;
  
  m_zLock.vDropWrite();

  return true;
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
void Queue::vDump(const char* pTab)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUE,_FOOTER_);
  
  vTrace("%sQueue: %p\n", pTab, this);
  vTrace("%s  Count %lld\n", pTab, m_iCount);
  vTrace("%s  TopBuffer %p\n", pTab, m_pTopBuffer);
  vTrace("%s  EndBuffer %p\n", pTab, m_pEndBuffer);
  
  for (QueueBuffer* pQbuffer = m_pTopBuffer; pQbuffer; pQbuffer = pQbuffer->pNext())
    pQbuffer->vDump(pTab);
}

/*----------------------------------------------------------------------------*/
/* dump grid                                                                  */
/*----------------------------------------------------------------------------*/
void Queue::vDump(eCN eClass, Object* pObject, const char* pTab)
{
  ASSERT(eClass == CN_QUEUE);
  ASSERT(pObject->eClass() == CN_QUEUE);
  
  Queue* pQueue = reinterpret_cast<Queue*>(pObject);
  
  pQueue->vDump(pTab);
}

/*----------------------------------------------------------------------------*/
/* intialize this queue                                                       */
/*----------------------------------------------------------------------------*/
void Queue::vInitialize(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUE,_FOOTER_);
  
  m_iCount = 0;
  m_pTopBuffer = nullptr;
  m_pEndBuffer = nullptr;
}

/*----------------------------------------------------------------------------*/
/* pop value from this queue (there may not be any)                           */
/*----------------------------------------------------------------------------*/
bool Queue::bPop(ValWsp* pValWsp)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUE,_FOOTER_);

  m_zLock.vGrabWrite();
  
  if ( ! m_pTopBuffer ) {
    ASSERT( ! m_iCount );
    m_zLock.vDropWrite();
    return nullptr;
  }
  
  bool bDiscardable;
  bool bPopped = m_pTopBuffer->bPop(pValWsp,bDiscardable);

  if (bDiscardable) {
    ASSERT(bPopped);
    vDiscard();
  }
  
  if (bPopped)
    m_iCount--;
  
  ASSERT(m_iCount >= 0);
  
  m_zLock.vDropWrite();

  return bPopped;
}

/*----------------------------------------------------------------------------*/
/* pop value set from this queue (there may not be any)                       */
/*----------------------------------------------------------------------------*/
bool Queue::bPop(ValSet* pValSet)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUE,_FOOTER_);
  
  m_zLock.vGrabWrite();
  
  if ( ! m_pTopBuffer ) {
    ASSERT( ! m_iCount );
    m_zLock.vDropWrite();
    return nullptr;
  }
  
  bool bDiscardable;
  bool bPopped = m_pTopBuffer->bPop(pValSet,bDiscardable);
  
  if (bDiscardable) {
    ASSERT(bPopped);
    vDiscard();
  }
  
  if (bPopped)
    m_iCount--;

  ASSERT(m_iCount >= 0);
  
  m_zLock.vDropWrite();
  
  return bPopped;
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
  
  ASSERT(0 == pQueueBufferTBD->iCount());
 
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
Footer* Queue::pFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUE);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/