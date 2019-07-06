/*============================================================================*/
/* OpQueue.cpp -- queue of ops waiting to be serviced                         */
/*============================================================================*/

#include "PushDown.h"

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
OpQueue* PushDown::_pOpQueue(void* aAddr)
{
  OpQueue* pOpQueue = reinterpret_cast<OpQueue*>(aAddr);
  
  RASSERT_TYPE(pOpQueue,CN_THREADQUEUE);
  
  return pOpQueue;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* OpQueue::operator new(size_t)
{
  void* pObject = HeapMem::pAllocate(CN_THREADQUEUE);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void OpQueue::operator delete(void* aAddr)
{
  OpQueue* pOpQueue = reinterpret_cast<OpQueue*>(aAddr);
  
  HeapMem::vFree(pOpQueue);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
OpQueue::OpQueue(void)
: ListNL_t<Op>(CN_THREADQUEUE)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREADQUEUE,_FOOTER_);
  
  m_okLock.vInitLock(LK_THREAD_QUEUE);
  
  _checkInitialised_(CN_THREADQUEUE);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_THREADQUEUE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
OpQueue::~OpQueue(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREADQUEUE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* append thread to end of queue                                              */
/*----------------------------------------------------------------------------*/
void OpQueue::vAppendOp(Op* pOp)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREADQUEUE,_FOOTER_);
  
  ListNL_t<Op>::vAppend(pOp);
}

/*----------------------------------------------------------------------------*/
/* drop lock for write                                                        */
/*----------------------------------------------------------------------------*/
void OpQueue::vDropLock(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREADQUEUE,_FOOTER_);
  
  m_okLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* grab lock for write                                                        */
/*----------------------------------------------------------------------------*/
void OpQueue::vGrabLock(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREADQUEUE,_FOOTER_);
  
  m_okLock.vGrabWrite();
}

/*----------------------------------------------------------------------------*/
/* grab lock for write                                                        */
/*----------------------------------------------------------------------------*/
bool OpQueue::bGrabLock(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREADQUEUE,_FOOTER_);
  
  return m_okLock.bGrabWrite();
}

/*----------------------------------------------------------------------------*/
/* is locked ?                                                                */
/*----------------------------------------------------------------------------*/
bool OpQueue::bIsLocked(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREADQUEUE,_FOOTER_);
  
  return m_okLock.bIsLocked();
}

/*----------------------------------------------------------------------------*/
/* peek at first thread at top of queue                                       */
/*----------------------------------------------------------------------------*/
Op* OpQueue::pPeekOp(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREADQUEUE,_FOOTER_);
  
  ASSERT(m_okLock.bIsLocked());
  
  Op* pOp = ListNL_t<Op>::pHead();

  return pOp;
}

/*----------------------------------------------------------------------------*/
/* pop first thread from top of queue                                         */
/*----------------------------------------------------------------------------*/
Op* OpQueue::pPopOp(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREADQUEUE,_FOOTER_);
  
  ASSERT(m_okLock.bIsLocked());
  
  Op* pOp = ListNL_t<Op>::pPop();

  return pOp;
}

/*----------------------------------------------------------------------------*/
/* get number of threads in queue                                             */
/*----------------------------------------------------------------------------*/
i8 OpQueue::nQueuedOps(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREADQUEUE,_FOOTER_);

  i8 nOps = ListNL_t<Op>::nObjects();

  return nOps;
}

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void OpQueue::vContentDump(void)
{
  //vDump(CN_THREADQUEUE, this, Stz::aTab(1));
  
  ASSERT(m_okLock.bIsLocked());

  vTrace("ITEMS %lld\n"_Stz,ListNL_t<Op>::nObjects());
  
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
Footer* OpQueue::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_THREADQUEUE);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
