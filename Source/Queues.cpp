/*============================================================================*/
/* Queues.cpp -- Collection of the Systems' Queues                            */
/*============================================================================*/

#include "Threads.h"

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;
extern Arguments  gxArguments;

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Queues* Threads::_pQueues(void* aAddr)
{
  Queues* pQueues = reinterpret_cast<Queues*>(aAddr);
  
  RASSERT( !pQueues || CN_QUEUES == pQueues->eClass() );
  
  return pQueues;
}

/*----------------------------------------------------------------------------*/
/* new a queues from existing memory                                          */
/*----------------------------------------------------------------------------*/
void* Queues::operator new(size_t iSize)
{
  void* pObject = Heap::pAllocate(CN_QUEUES);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* Queues are never delete (for the moment)                                   */
/*----------------------------------------------------------------------------*/
void Queues::operator delete(void* aAddr)
{
  RASSERT(aAddr);
  vFatal("fatal: Queues class cannot be deleted"_Stz);
  
  NEVER_REACH;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Queues::Queues(void)
: Object(CN_QUEUES)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES,_FOOTER_);
  
  m_pThreadVector  = new ThreadVector;
  
  m_pSleepingQueue = new TaskQueue(bTaskCompareWakeupTime);
  m_pReadyQueue  = new TaskQueue(bTaskCompareTaskNo);
  
  for (i8 i_Slot = 0; i_Slot < C_SLOTS_PER_NODE; i_Slot++)
    m_pReadySlotQueues[i_Slot] = nullptr;

  for (i8 i_Slot = 0; i_Slot < gxArguments.m_nSlots; i_Slot++)
    m_pReadySlotQueues[i_Slot] = new TaskQueue(bTaskCompareTaskNo);
  
  m_pExecutingQueue = new TaskQueue(bTaskCompareTaskNo);
  m_pWaitingQueue   = new TaskQueue(bTaskCompareTaskNo);
  
  _checkInitialised_(CN_QUEUES);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_QUEUES,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Queues::~Queues(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add thread to thread vector                                                */
/*----------------------------------------------------------------------------*/
void Queues::vAddThread(Thread* pThread)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);
  
  RASSERT(TC_IDLE == pThread->eThreadState());
  
  m_pThreadVector->vGrabLock();
  m_pThreadVector->vAdd(pThread);
  m_pThreadVector->vDropLock();
}

/*----------------------------------------------------------------------------*/
/* add task to executing queue                                                */
/*----------------------------------------------------------------------------*/
void Queues::vAddToExecutingQueue(Task* pTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);
  
  RASSERT(TS_EXECUTING == pTask->eState());
  
  pTask->vQueue(QR_EXECUTING_QUEUE);
  
  m_pExecutingQueue->vGrabLock();
  m_pExecutingQueue->vAddTask(pTask);
  m_pExecutingQueue->vDropLock();
}

/*----------------------------------------------------------------------------*/
/* add task to ready queue                                                    */
/*----------------------------------------------------------------------------*/
void Queues::vAddToReadyQueue(Task* pTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);
  
  RASSERT(TS_READY == pTask->eState());
  
  pTask->vQueue(QR_READY_QUEUE);
  
  m_pReadyQueue->vGrabLock();
  m_pReadyQueue->vAddTask(pTask);
  m_pReadyQueue->vDropLock();
}

/*----------------------------------------------------------------------------*/
/* add task to a slot queue                                                   */
/*----------------------------------------------------------------------------*/
void Queues::vAddToReadySlotQueue(Task* pTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);
  
  // NOTE: Task has a m_iPartition... not being set
  // this needs to set for partition tasks
  
  RASSERT(TS_READY == pTask->eState());
  
  // get slot no of the task
  i8 iSlot = Partition::iPartitionToSlot(pTask->iPartition());
  
  pTask->vQueue((eQR)iSlot);
  
  m_pReadySlotQueues[iSlot]->vGrabLock();
  m_pReadySlotQueues[iSlot]->vAddTask(pTask);
  m_pReadySlotQueues[iSlot]->vDropLock();
}

/*----------------------------------------------------------------------------*/
/* add task to sleeping queue                                                 */
/*----------------------------------------------------------------------------*/
void Queues::vAddToSleepingQueue(Task* pTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);
  
  RASSERT(TS_SLEEPING == pTask->eState());
  
  pTask->vQueue(QR_SLEEPING_QUEUE);
  
  m_pSleepingQueue->vGrabLock();
  m_pSleepingQueue->vAddTask(pTask);
  m_pSleepingQueue->vDropLock();
}

/*----------------------------------------------------------------------------*/
/* add task to waiting queue                                                  */
/*----------------------------------------------------------------------------*/
void Queues::vAddToWaitingQueue(Task* pTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);
  
  RASSERT(TS_WAITING_ACK == pTask->eState() ||
          TS_WAITING_SIGNAL == pTask->eState());
  
  pTask->vQueue(QR_WAITING_QUEUE);
  
  m_pWaitingQueue->vGrabLock();
  m_pWaitingQueue->vAddTask(pTask);
  m_pWaitingQueue->vDropLock();
}

/*----------------------------------------------------------------------------*/
/* attempt to find idle thread                                                */
/*----------------------------------------------------------------------------*/
Thread* Queues::pGrabIdleThread(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);
  
  m_pThreadVector->vGrabLock();

  i8 nThreads = m_pThreadVector->nItems();
  for (i8 i_Thread = 0; i_Thread < nThreads; i_Thread++) {

    Thread* pThread = m_pThreadVector->pGet(i_Thread);
    
    // attempt to grab threads mutex
    //if (! pThread->bMutexLock())
    //  continue;
    
    //ASSERT(pThread->bMutexIsLocked());
    
    if (pThread->bIsIdle()) {
      m_pThreadVector->vDropLock();
      return pThread;
    }
    
    //pThread->vMutexUnlock();
  }
  
  m_pThreadVector->vDropLock();
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* inspect the ready queue                                                    */
/*----------------------------------------------------------------------------*/
Task* Queues::pInspectReadyQueue(Thread* pCallingThread)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);
  
  Task* pPoppedTask;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // lock queue and check for task
  
  // attempt to grab a write lock on the general queue
  if (! m_pReadyQueue->bGrabLock()) {
    // unable to lock queue (another thread must be processing it)
    // so move on to the next queue
    return nullptr;
  }
  
  // pop task off the general queue
  pPoppedTask = m_pReadyQueue->pPopTask();
  
  // drop the lock
  m_pReadyQueue->vDropLock();
  
  // is there a task to run?
  if (! pPoppedTask)
    return nullptr;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // return the task popped from the ready queue
  return pPoppedTask;
}

/*----------------------------------------------------------------------------*/
/* inspect a ready slot queue                                                 */
/*----------------------------------------------------------------------------*/
Task* Queues::pInspectReadySlotQueue(Thread* pCallingThread)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);
  
  Task* pPoppedTask;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // lock queue and check for task
  
  // attempt to grab a write lock on the queue
  if (! m_pReadySlotQueues[pCallingThread->m_iLastRRqueue]->bGrabLock()) {
    // unable to lock queue (another thread must be processing it)
    // so move on to the next queue
    return nullptr;
  }
  
  // pop a task off the partition queues
  pPoppedTask = m_pReadySlotQueues[pCallingThread->m_iLastRRqueue]->pPopTask();
  
  // drop lock on the queue
  m_pReadySlotQueues[pCallingThread->m_iLastRRqueue]->vDropLock();
  
  // is there a task to run?
  if (! pPoppedTask)
    return nullptr;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // there is a task to run
  
  // return the task popped from the slot queue
  return pPoppedTask;
}

/*----------------------------------------------------------------------------*/
/* inspect the sleeping queue                                                 */
/*----------------------------------------------------------------------------*/
Task* Queues::pInspectSleepingQueue(Thread* pCallingThread)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);
  
  Task* pPeekedTask;
  Task* pPoppedTask;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // lock queue and peek at task
  
  // attempt to grab a write lock on the sleeping queue
  if (! m_pSleepingQueue->bGrabLock()) {
    // unable to lock queue (another thread must be processing it)
    // so move on to the next queue
    return nullptr;
  }
  
  // peek at the first task in the sleeping queue
  pPeekedTask = m_pSleepingQueue->pPeekTask();
  
  // check if peeked task's wakeup time is after now
  if (! pPeekedTask || ! pPeekedTask->bWakeupNow()) {
    
    // - peeked task is to be woken up in the future
    
    // drop lock on sleeping queue
    m_pSleepingQueue->vDropLock();
    
    // didn't find a task, move to next queue
    return nullptr;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // peeked task can be be woken up
  
  // pop the task off the sleeping queue
  pPoppedTask = m_pSleepingQueue->pPopTask();
  ASSERT(pPoppedTask == pPeekedTask);
  
  // drop lock on sleeping queue
  m_pSleepingQueue->vDropLock();
  
  // clear wakeup time of the task
  pPoppedTask->vWakeupTask();
  
  // return the task popped from the sleeping queue
  return pPoppedTask;
}

/*----------------------------------------------------------------------------*/
/* pop next queued task in round robin fashion                                */
/*----------------------------------------------------------------------------*/
bool Queues::bPopQueuedTask(Task** ppPoppedTask, Thread* pCallingThread)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);
  
  Task* pPoppedTask;
  
  // set popped task to be null to start with
  *ppPoppedTask = NULL;
  
  // start a round robin sequence
  pCallingThread->vRRstart();
  
  // round robin thru the queues
  while (pCallingThread->bRRnext()) {
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // inspect the queues
    
    if (QR_SLEEPING_QUEUE == pCallingThread->m_iLastRRqueue) {
      
      if ( ! (pPoppedTask = pInspectSleepingQueue(pCallingThread)) )
        continue;
      *ppPoppedTask = pPoppedTask;
      return true;
    }
    
    if (QR_READY_QUEUE == pCallingThread->m_iLastRRqueue) {
      
      if ( ! (pPoppedTask = pInspectReadyQueue(pCallingThread)) )
        continue;
      *ppPoppedTask = pPoppedTask;
      return true;
    }
    
    if ( ! (pPoppedTask = pInspectReadySlotQueue(pCallingThread)) )
      continue;
    *ppPoppedTask = pPoppedTask;
    return true;
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
  } // while (true)
  
  NEVER_REACH;
  return false;
}

/*----------------------------------------------------------------------------*/
/* remove thread from thread vector                                           */
/*----------------------------------------------------------------------------*/
void Queues::vRemoveThread(Thread* pThread)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);
  
  m_pThreadVector->vGrabLock();
  m_pThreadVector->vRemove(pThread);
  m_pThreadVector->vDropLock();
}

/*----------------------------------------------------------------------------*/
/* remove task from executing queue                                           */
/*----------------------------------------------------------------------------*/
void Queues::vRemoveFromExecutingQueue(Task* pTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);
  
  m_pExecutingQueue->vGrabLock();
  m_pExecutingQueue->vRemoveTask(pTask);
  m_pExecutingQueue->vDropLock();
  
  pTask->vQueue(QR_NONE);
}

/*----------------------------------------------------------------------------*/
/* remove task from waiting queue                                             */
/*----------------------------------------------------------------------------*/
void Queues::vRemoveFromWaitingQueue(Task* pTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);
  
  m_pWaitingQueue->vGrabLock();
  m_pWaitingQueue->vRemoveTask(pTask);
  m_pWaitingQueue->vDropLock();
  
  pTask->vQueue(QR_NONE);
}

/*----------------------------------------------------------------------------*/
/* schedule a ready to execute task                                           */
/*----------------------------------------------------------------------------*/
void Queues::vScheduleReadyTask(Task* pTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);
  
  RASSERT(TS_READY == pTask->eState());
  
  Thread* pIdleThread = pGrabIdleThread();
  
  if (pIdleThread)
    pIdleThread->vExecuteTask(pTask);
  else {
    NEVER_REACH; // E1 temp
  //vAddToReadyQueue(pTask);
  }
}

/*----------------------------------------------------------------------------*/
/* schedule ready slot task                                                   */
/*----------------------------------------------------------------------------*/
void Queues::vScheduleReadySlotTask(Task* pTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);
  
  RASSERT(TS_READY == pTask->eState());
  
  Thread* pIdleThread = pGrabIdleThread();
  
  if (pIdleThread)
    pIdleThread->vExecuteTask(pTask);
  else
    vAddToReadySlotQueue(pTask);
}

/*----------------------------------------------------------------------------*/
/* schedule sleeping task                                                     */
/*----------------------------------------------------------------------------*/
void Queues::vScheduleSleepingTask(Task* pTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);
  
  RASSERT(TS_SLEEPING == pTask->eState());
  
  // is the task still actually sleeping
  if (! pTask->bWakeupNow()) {
    vAddToSleepingQueue(pTask);
    return;
  }
  
  // - tasks wake up time has arrived. run it
  
  pTask->vWakeupTask();
  
  Thread* pIdleThread = pGrabIdleThread();
  
  //! pIdleThread mutex is held by calling thread

  if (pIdleThread)
    pIdleThread->vExecuteTask(pTask);
  else
    vAddToReadyQueue(pTask);
}

/*----------------------------------------------------------------------------*/
/* schedule ready waiting task                                                */
/*----------------------------------------------------------------------------*/
void Queues::vScheduleWaitingTask(Task* pTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);
  
  RASSERT(TS_WAITING_ACK    == pTask->eState() ||
          TS_WAITING_SIGNAL == pTask->eState());
  
  vAddToWaitingQueue(pTask);
}

/*----------------------------------------------------------------------------*/
/* get the thread vector                                                      */
/*----------------------------------------------------------------------------*/
ThreadVector* Queues::pThreadVector(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);
  
  return m_pThreadVector;
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Queues::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
