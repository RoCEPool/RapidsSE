/*============================================================================*/
/* Queue.cpp - queue of values                                                */
/*============================================================================*/

#include "Tasks.h"

extern BufferPool*  gpBufferPool;

/*----------------------------------------------------------------------------*/
/* new operator                                                               */
/*----------------------------------------------------------------------------*/
void* Queue::operator new(size_t)
{
  void* pObject = gpSmallHeap->pAllocObject();
  
  return pObject;
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
  _poolAddress_(this);
  
  vInitialize();
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Queue::~Queue(void)
{
  _poolAddress_(this);
}

/*----------------------------------------------------------------------------*/
/* append value to this queue                                                 */
/*----------------------------------------------------------------------------*/
bool Queue::bAppend(Value* pValue)
{
  _poolAddress_(this);
  
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
  _poolAddress_(this);
  
  return m_iCount;
}

/*----------------------------------------------------------------------------*/
/* dump the queue                                                             */
/*----------------------------------------------------------------------------*/
void Queue::vDump(void)
{
  _poolAddress_(this);
  
  vTrace("Queue: %p Count %lld TopBuffer %p EndBuffer%p\n",
         this,
         m_iCount,
         m_pTopBuffer,
         m_pEndBuffer);
  
  for (QueueBuffer* pQbuffer = m_pTopBuffer; pQbuffer; pQbuffer = pQbuffer->pNext())
    pQbuffer->vDump();
}

/*----------------------------------------------------------------------------*/
/* intialize this queue                                                       */
/*----------------------------------------------------------------------------*/
void Queue::vInitialize(void)
{
  _poolAddress_(this);
  
  m_iCount = 0;
  m_pTopBuffer = nullptr;
  m_pEndBuffer = nullptr;
}

/*----------------------------------------------------------------------------*/
/* pop value from this queue (there may not be any)                           */
/*----------------------------------------------------------------------------*/
bool Queue::bPop(ValWsp* pValWsp)
{
  _poolAddress_(this);

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
  _poolAddress_(this);
  
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
  _poolAddress_(this);
  
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
  _poolAddress_(this);
  
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
  _poolAddress_(this);
  
  i8 iCount = 0;
  
  for (QueueBuffer* pQbuffer = m_pTopBuffer; pQbuffer; pQbuffer->pNext()) {
    iCount += pQbuffer->iCount();
    pQbuffer->vVerify();
  }
  
  ASSERT(iCount == m_iCount);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/