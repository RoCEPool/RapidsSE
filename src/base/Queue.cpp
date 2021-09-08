/*============================================================================*/
/* Queue.cpp -- Queue of Values                                               */
/*============================================================================*/

#include "BASE.h"

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Queue* BASE::_pQueue(void* aAddr)
{
  Queue* pQueue = reinterpret_cast<Queue*>(aAddr);
  
  RASSERT_TYPE(pQueue,CN_QUEUE);
  
  return pQueue;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Queue::operator new(size_t)
{
  void* pObject = Heaps::pAllocate(CN_QUEUE);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Queue::operator new(size_t, Queue* pQueue)
{
  return pQueue;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void Queue::operator delete(void* pQueueAddress)
{
  Queue* pQueue = reinterpret_cast<Queue*>(pQueueAddress);
  
  Heaps::vFree(pQueue);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Queue::Queue(eCN eClass)
: Object(eClass)
, m_okLock(this)
{
  _precondition_(_ADDRESS_,_MAGIC_,eClass);
  
  m_nItems = 0;
  m_nMaxItems = 0;
  m_pTopBuffer = nullptr;
  m_pEndBuffer = nullptr;

  _checkInitialised_(CN_QUEUE);
  
  _postcondition_(_ADDRESS_,_MAGIC_,eClass);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Queue::~Queue(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  if (m_nItems)
    vPostMortem();
  
  vReset();
      
  ASSERT( ! m_pEndBuffer );
  ASSERT( ! m_pTopBuffer );
}

/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* append object pointer to this queue                                        */
/*----------------------------------------------------------------------------*/
void Queue::vAppend(Object* pObject)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_okLock.vGrabWrite();
  
  if ( ! m_pEndBuffer ) {
    ASSERT(0 == m_nItems);
    // need to add first buffer
    vExpand();
  }
  
  // atempt to add into
  bool bAppended = m_pEndBuffer->bAppend(pObject);
  
  if ( ! bAppended ) {
    vExpand();
    bool b = m_pEndBuffer->bAppend(pObject);
    ASSERT(b);
    UNUSED(b);
  }
  
  m_nItems++;

  if (m_nItems > m_nMaxItems)
    m_nMaxItems = m_nItems;
  
  m_okLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* dump the queue                                                             */
/*----------------------------------------------------------------------------*/
void Queue::vContentDump(Stz* aTab)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  vTrace("%sQueue: %p\n"_Stz, aTab, this);
  vTrace("%s  Count %lld\n"_Stz, aTab, m_nItems);
  vTrace("%s  TopBuffer %p\n"_Stz, aTab, m_pTopBuffer);
  vTrace("%s  EndBuffer %p\n"_Stz, aTab, m_pEndBuffer);
  
  for (QueueBuffer* pQbuffer = m_pTopBuffer; pQbuffer; pQbuffer = pQbuffer->pNext())
    pQbuffer->vContentDump(aTab);
}

/*----------------------------------------------------------------------------*/
/* get number of items in queue                                               */
/*----------------------------------------------------------------------------*/
i8 Queue::nItems(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return m_nItems;
}

/*----------------------------------------------------------------------------*/
/* get max number of items in queue                                           */
/*----------------------------------------------------------------------------*/
i8 Queue::nMaxItems(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  return m_nMaxItems;
}

/*----------------------------------------------------------------------------*/
/* peek                                                                       */
/*----------------------------------------------------------------------------*/
Object* Queue::pPeek(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  //RASSERT(0);
  
  Object* pObject = nullptr;

  m_okLock.vGrabWrite();
  
  if (m_pTopBuffer)
    pObject = m_pTopBuffer->pPeek();

  m_okLock.vDropWrite();

  return pObject;
}

/*----------------------------------------------------------------------------*/
/* pop value from this queue (there may not be any)                           */
/*----------------------------------------------------------------------------*/
Object* Queue::pPop(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  
  m_okLock.vGrabWrite();
  
  // are there queue buffers?
  if (! m_pTopBuffer) {
    ASSERT(! m_nItems);
    ASSERT(! m_pEndBuffer);
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
    m_nItems--;
  ASSERT(m_nItems >= 0);
  
  m_okLock.vDropWrite();
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* dump grid                                                                  */
/*----------------------------------------------------------------------------*/
void Queue::vReset(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  if (m_nItems)
    vPostMortem();

  RASSERT(0 == m_nItems);

  while (m_pTopBuffer)
    vDiscard();
  
  m_nItems = 0;
    
  ASSERT( ! m_pEndBuffer );
  ASSERT( ! m_pTopBuffer );
}


/*----------------------------------------------------------------------------*/
/*-- private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* discard top qbuffer (should be empty)                                      */
/*----------------------------------------------------------------------------*/
void Queue::vDiscard(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
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
  _precondition_(_ADDRESS_,_MAGIC_);
  
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
  _precondition_(_ADDRESS_,_MAGIC_);
  
  i8 iCount = 0;
  
  for (QueueBuffer* pQbuffer = m_pTopBuffer;
       pQbuffer;
       pQbuffer = pQbuffer->pNext()) {
    iCount += pQbuffer->iCount();
    pQbuffer->vVerify();
  }
  
  ASSERT(iCount == m_nItems);
}


/*----------------------------------------------------------------------------*/
/* something has gone wrong at ~Queue                                         */
/*----------------------------------------------------------------------------*/
void Queue::vPostMortem(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  vTrace("PostMortem Queue::\n"_Stz);
  
  vContentDump(Stz::szTab(1));
  
  while (Object* pObj = pPop() ) {
    i8 iOperandKind = ZK_NONE;
    if (pObj) {
      iOperandKind = AM::iOperandZkKind((Operand *) pObj);
    }
    vTrace("  object %p class %s Operand %s\n"_Stz
           ,pObj
           ,pObj ? pObj->szClassNm() : "none"_Stz
           ,aZK[(eZK)iOperandKind]);
  }

  vTrace("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Queue::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
