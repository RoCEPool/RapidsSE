/*============================================================================*/
/* BufferQueue.cpp -- queue of buffers to be processed by an op               */
/*============================================================================*/

#include "PushDown.h"

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
BufferQueue* PushDown::_pBufferQueue(void* aAddr)
{
  BufferQueue* pBufferQueue = reinterpret_cast<BufferQueue*>(aAddr);
  
  RASSERT_TYPE(pBufferQueue,CN_BUFFERQUEUE);
  
  return pBufferQueue;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* BufferQueue::operator new(size_t)
{
  void* pObject = HeapMem::pAllocate(CN_BUFFERQUEUE);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void BufferQueue::operator delete(void* aAddr)
{
  BufferQueue* pBufferQueue = reinterpret_cast<BufferQueue*>(aAddr);
  
  HeapMem::vFree(pBufferQueue);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
BufferQueue::BufferQueue(void)
: ListNL_t<OperandBuffer>(CN_BUFFERQUEUE)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUFFERQUEUE,_FOOTER_);
  
  m_okLock.vInitLock(LK_THREAD_QUEUE);
  
  _checkInitialised_(CN_BUFFERQUEUE);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_BUFFERQUEUE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
BufferQueue::~BufferQueue(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUFFERQUEUE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* append thread to end of queue                                              */
/*----------------------------------------------------------------------------*/
void BufferQueue::vAppendBuffer(OperandBuffer* pBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUFFERQUEUE,_FOOTER_);
  
  ListNL_t<OperandBuffer>::vAppend(pBuffer);
}

/*----------------------------------------------------------------------------*/
/* drop lock for write                                                        */
/*----------------------------------------------------------------------------*/
void BufferQueue::vDropLock(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUFFERQUEUE,_FOOTER_);
  
  m_okLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* grab lock for write                                                        */
/*----------------------------------------------------------------------------*/
void BufferQueue::vGrabLock(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUFFERQUEUE,_FOOTER_);
  
  m_okLock.vGrabWrite();
}

/*----------------------------------------------------------------------------*/
/* grab lock for write                                                        */
/*----------------------------------------------------------------------------*/
bool BufferQueue::bGrabLock(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUFFERQUEUE,_FOOTER_);
  
  return m_okLock.bGrabWrite();
}

/*----------------------------------------------------------------------------*/
/* is locked ?                                                                */
/*----------------------------------------------------------------------------*/
bool BufferQueue::bIsLocked(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUFFERQUEUE,_FOOTER_);
  
  return m_okLock.bIsLocked();
}

/*----------------------------------------------------------------------------*/
/* peek at first thread at top of queue                                       */
/*----------------------------------------------------------------------------*/
OperandBuffer* BufferQueue::pPeekBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUFFERQUEUE,_FOOTER_);
  
  ASSERT(m_okLock.bIsLocked());
  
  OperandBuffer* pBuffer = ListNL_t<OperandBuffer>::pHead();

  return pBuffer;
}

/*----------------------------------------------------------------------------*/
/* pop first thread from top of queue                                         */
/*----------------------------------------------------------------------------*/
OperandBuffer* BufferQueue::pPopBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUFFERQUEUE,_FOOTER_);
  
  ASSERT(m_okLock.bIsLocked());
  
  OperandBuffer* pBuffer = ListNL_t<OperandBuffer>::pPop();

  return pBuffer;
}

/*----------------------------------------------------------------------------*/
/* get number of threads in queue                                             */
/*----------------------------------------------------------------------------*/
i8 BufferQueue::nQueuedOps(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUFFERQUEUE,_FOOTER_);

  i8 nOps = ListNL_t<OperandBuffer>::nObjects();

  return nOps;
}

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void BufferQueue::vContentDump(void)
{
  //vDump(CN_BUFFERQUEUE, this, Stz::aTab(1));
  
  ASSERT(m_okLock.bIsLocked());

  vTrace("ITEMS %lld\n"_Stz,ListNL_t<OperandBuffer>::nObjects());
  
//  for (Op* pOp = ListNL_t<Op>::pHead();
//       pOp;
//       pOp = pOp->pNextNode()) {
//
//    vTrace("Op %p %s %s order %lld\n"_Stz,
//           pOp,
//           aCN[pOp->pJob()->eClass()],
//           aTK[pOp->eState()],
//           pOp->iQueueOrder()
//           );
//
//  }
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* BufferQueue::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BUFFERQUEUE);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
