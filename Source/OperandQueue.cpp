/*============================================================================*/
/* OperandQueue.cpp -- queue of buffers to be processed by an op               */
/*============================================================================*/

#include "PushDown.h"

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
OperandQueue* PushDown::_pOperandQueue(void* aAddr)
{
  OperandQueue* pOperandQueue = reinterpret_cast<OperandQueue*>(aAddr);
  
  RASSERT_TYPE(pOperandQueue,CN_OPERANDQUEUE);
  
  return pOperandQueue;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* OperandQueue::operator new(size_t)
{
  void* pObject = HeapMem::pAllocate(CN_OPERANDQUEUE);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void OperandQueue::operator delete(void* aAddr)
{
  OperandQueue* pOperandQueue = reinterpret_cast<OperandQueue*>(aAddr);
  
  HeapMem::vFree(pOperandQueue);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
OperandQueue::OperandQueue(void)
: ListNL_t<OperandBuffer>(CN_OPERANDQUEUE)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPERANDQUEUE,_FOOTER_);
  
  m_okLock.vInitLock(LK_THREAD_QUEUE);
  
  _checkInitialised_(CN_OPERANDQUEUE);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_OPERANDQUEUE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
OperandQueue::~OperandQueue(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPERANDQUEUE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* append thread to end of queue                                              */
/*----------------------------------------------------------------------------*/
void OperandQueue::vAppendBuffer(OperandBuffer* pBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPERANDQUEUE,_FOOTER_);
  
  ListNL_t<OperandBuffer>::vAppend(pBuffer);
}

/*----------------------------------------------------------------------------*/
/* drop lock for write                                                        */
/*----------------------------------------------------------------------------*/
void OperandQueue::vDropLock(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPERANDQUEUE,_FOOTER_);
  
  m_okLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* grab lock for write                                                        */
/*----------------------------------------------------------------------------*/
void OperandQueue::vGrabLock(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPERANDQUEUE,_FOOTER_);
  
  m_okLock.vGrabWrite();
}

/*----------------------------------------------------------------------------*/
/* grab lock for write                                                        */
/*----------------------------------------------------------------------------*/
bool OperandQueue::bGrabLock(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPERANDQUEUE,_FOOTER_);
  
  return m_okLock.bGrabWrite();
}

/*----------------------------------------------------------------------------*/
/* is locked ?                                                                */
/*----------------------------------------------------------------------------*/
bool OperandQueue::bIsLocked(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPERANDQUEUE,_FOOTER_);
  
  return m_okLock.bIsLocked();
}

/*----------------------------------------------------------------------------*/
/* peek at first thread at top of queue                                       */
/*----------------------------------------------------------------------------*/
OperandBuffer* OperandQueue::pPeekBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPERANDQUEUE,_FOOTER_);
  
  ASSERT(m_okLock.bIsLocked());
  
  OperandBuffer* pBuffer = ListNL_t<OperandBuffer>::pHead();

  return pBuffer;
}

/*----------------------------------------------------------------------------*/
/* pop first thread from top of queue                                         */
/*----------------------------------------------------------------------------*/
OperandBuffer* OperandQueue::pPopBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPERANDQUEUE,_FOOTER_);
  
  ASSERT(m_okLock.bIsLocked());
  
  OperandBuffer* pBuffer = ListNL_t<OperandBuffer>::pPop();

  return pBuffer;
}

/*----------------------------------------------------------------------------*/
/* get number of threads in queue                                             */
/*----------------------------------------------------------------------------*/
i8 OperandQueue::nQueuedOps(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPERANDQUEUE,_FOOTER_);

  i8 nOps = ListNL_t<OperandBuffer>::nObjects();

  return nOps;
}

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void OperandQueue::vContentDump(void)
{
  //vDump(CN_OPERANDQUEUE, this, Stz::aTab(1));
  
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
Footer* OperandQueue::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPERANDQUEUE);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
