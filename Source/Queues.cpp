/*============================================================================*/
/* Queues.cpp -- Collection of the Systems' Queues                            */
/*============================================================================*/

#include "Scheduling.h"

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Queues* Scheduling::_pQueues(void* aAddr)
{
  Queues* pQueues = reinterpret_cast<Queues*>(aAddr);
  
  RASSERT_TYPE(pQueues,CN_QUEUES);
  
  return pQueues;
}

/*----------------------------------------------------------------------------*/
/* new a queues from existing memory                                          */
/*----------------------------------------------------------------------------*/
void* Queues::operator new(size_t)
{
  void* pObject = HeapMem::pAllocate(CN_QUEUES);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* Queues are never deleted (for the moment)                                  */
/*----------------------------------------------------------------------------*/
void Queues::operator delete(void* aAddr)
{
  vFatal("fatal: Queues class cannot be deleted"_Stz);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Queues::Queues(void)
: Object(CN_QUEUES)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES,_FOOTER_);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // threads
  
  m_pGenThreadVec = new ThreadVec;
  
  m_pIdleThreadQueue = new ThreadQueue;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  //
  
  m_ixQueueOrder = 0;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // task queues
  
  m_pSleepingQueue = new TaskQueue(QK_SLEEPING_QUEUE,
                                   C_NON_UNQIUE_KEY,
                                   bTaskCompareWakeupTime);
  
  m_pReadyQueue    = new TaskList(QK_READY_QUEUE);
  m_pReadyQueueLP  = new TaskList(QK_READY_QUEUE_LP);
  m_pExecutingSet  = new TaskList(QK_EXECUTING_SET);
  m_pWaitingSet    = new TaskList(QK_WAITING_SET);

  _checkInitialised_(CN_QUEUES);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_QUEUES,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Queues::~Queues(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* threads report                                                             */
/*----------------------------------------------------------------------------*/
void Queues::vThreadsReport(void)
{
  i8 iUser;
  i8 iSystem;
  i8 iUse;
  i8 iSleep;
  
  Queues* pQueues = gpAttachMap->m_pQueues;
  
  pQueues->m_pGenThreadVec->vGrabLock();

  i8 nThreads = pQueues->m_pGenThreadVec->nItems();
  for (i8 i_Thread = 0; i_Thread < nThreads; i_Thread++) {
    
    Thread* pThread = pQueues->m_pGenThreadVec->pGet(i_Thread);
    
    pThread->m_pPthread->bGetCpu(iUser,iSystem,iUse,iSleep);
    
    vTrace("  Thread %2lld %p"
           " base %llx size %lld"
           " user %lld system %lld use %lld sleep %lld"
           " %7s %s"
           " (%lld,%lld) assigned %lld popped %lld\n"_Stz,
           pThread->m_iThreadNo,
           pThread,
           
           pThread->m_aStackBase,
           pThread->m_iStackSize,
           
           iUser,
           iSystem,
           iUse,
           iSleep,
           
           aTD[pThread->m_eThreadState],
           pThread->m_eDedicated ? aCN[pThread->m_eDedicated]->aChar() : "",
           
           pThread->m_iLastQueueRR,
           pThread->m_iStartQueueRR,
           pThread->m_nAssignedTasks,
           pThread->m_nPoppedTasks
           );
  }
  
  pQueues->m_pGenThreadVec->vDropLock();
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
  
  RASSERT(TD_IDLE == pThread->eThreadState());
  
  m_pGenThreadVec->vGrabLock();
  m_pGenThreadVec->vAdd(pThread);
  m_pGenThreadVec->vDropLock();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add task to executing queue                                                */
/*----------------------------------------------------------------------------*/
void Queues::vAddToExecutingSet(Task* pTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);
  
  m_pExecutingSet->vGrabLock();
  vAddToExecutingSet_NL(pTask);
  m_pExecutingSet->vDropLock();
}

/*----------------------------------------------------------------------------*/
/* add task to ready queue                                                    */
/*----------------------------------------------------------------------------*/
void Queues::vAddToReadyQueue(Task* pTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);
  
  m_pReadyQueue->vGrabLock();
  vAddToReadyQueue_NL(pTask);
  m_pReadyQueue->vDropLock();
}

/*----------------------------------------------------------------------------*/
/* add task to low priority ready queue                                       */
/*----------------------------------------------------------------------------*/
void Queues::vAddToReadyQueueLP(Task* pTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);
  
  m_pReadyQueueLP->vGrabLock();
  vAddToReadyQueueLP_NL(pTask);
  m_pReadyQueueLP->vDropLock();
}

/*----------------------------------------------------------------------------*/
/* add task to sleeping queue                                                 */
/*----------------------------------------------------------------------------*/
void Queues::vAddToSleepingQueue(Task* pTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);
  
  m_pSleepingQueue->vGrabLock();
  vAddToSleepingQueue_NL(pTask);
  m_pSleepingQueue->vDropLock();
}

/*----------------------------------------------------------------------------*/
/* add task to waiting queue                                                  */
/*----------------------------------------------------------------------------*/
void Queues::vAddToWaitingSet(Task* pTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);
  
  m_pWaitingSet->vGrabLock();
  vAddToWaitingSet_NL(pTask);
  m_pWaitingSet->vDropLock();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add task to executing queue                                                */
/*----------------------------------------------------------------------------*/
void Queues::vAddToExecutingSet_NL(Task* pTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);
  
  RASSERT(TS_EXECUTING == pTask->eState());
  
  pTask->vQueue(QR_EXECUTING_QUEUE);
  pTask->vQueueOrder(++m_ixQueueOrder);
  
  if (pTask->pJob()->m_bMarkActivity)
    ActivityMap::vMarkJob(pTask->pJob(),
                       "Add to ExecutingSet: %s %p"_Stz,
                       aCN[pTask->pJob()->eClass()],
                       pTask->pJob());

  ASSERT(m_pExecutingSet->bIsLocked());
  m_pExecutingSet->vAddTask(pTask);
  
  ActionSequence::vAdd(pTask->pJob(),AN_ADD_EXECUTING_SET);
}

/*----------------------------------------------------------------------------*/
/* add task to ready queue                                                    */
/*----------------------------------------------------------------------------*/
void Queues::vAddToReadyQueue_NL(Task* pTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);
  
  RASSERT(TS_READY == pTask->eState());
  
  pTask->vQueue(QR_READY_QUEUE);
  pTask->vQueueOrder(++m_ixQueueOrder);
  
  if (pTask->pJob()->m_bMarkActivity)
    ActivityMap::vMarkJob(pTask->pJob(),
                       "Add to ReadyQueue: %s %p"_Stz,
                       aCN[pTask->pJob()->eClass()],
                       pTask->pJob());

  ASSERT(m_pReadyQueue->bIsLocked());
  
  m_pReadyQueue->vAddTask(pTask);
  
  ActionSequence::vAdd(pTask->pJob(),AN_ADD_READY_QUEUE);
}

/*----------------------------------------------------------------------------*/
/* add task to low priority ready queue                                       */
/*----------------------------------------------------------------------------*/
void Queues::vAddToReadyQueueLP_NL(Task* pTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);
  
  RASSERT(TS_READY == pTask->eState());
  
  pTask->vQueue(QR_READY_QUEUE_LP);
  pTask->vQueueOrder(++m_ixQueueOrder);
  
  if (pTask->pJob()->m_bMarkActivity)
    ActivityMap::vMarkJob(pTask->pJob(),
                          "Add to ReadyQueueLP: %s %p"_Stz,
                          aCN[pTask->pJob()->eClass()],
                          pTask->pJob());
  
  ASSERT(m_pReadyQueueLP->bIsLocked());
  
  m_pReadyQueueLP->vAddTask(pTask);
  
  ActionSequence::vAdd(pTask->pJob(),AN_ADD_READY_QUEUE_LP);
}

/*----------------------------------------------------------------------------*/
/* add task to sleeping queue                                                 */
/*----------------------------------------------------------------------------*/
void Queues::vAddToSleepingQueue_NL(Task* pTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);
  
  RASSERT(TS_SLEEPING == pTask->eState());
  
  pTask->vQueue(QR_SLEEPING_QUEUE);
  
  if (pTask->pJob()->m_bMarkActivity)
    ActivityMap::vMarkJob(pTask->pJob(),
                       "Add to SleepingQueue: %s %p"_Stz,
                       aCN[pTask->pJob()->eClass()],
                       pTask->pJob());

  ASSERT(m_pSleepingQueue->bIsLocked());
  
  i8 nPriorItems = m_pSleepingQueue->nTasks();
  m_pSleepingQueue->vAssertItems(nPriorItems);
  m_pSleepingQueue->vAddTask(pTask);
  m_pSleepingQueue->vAssertItems(nPriorItems + 1);
  
  ActionSequence::vAdd(pTask->pJob(),AN_ADD_SLEEPING_QUEUE);
}

/*----------------------------------------------------------------------------*/
/* add task to waiting queue                                                  */
/*----------------------------------------------------------------------------*/
void Queues::vAddToWaitingSet_NL(Task* pTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);
    
  // task must be in the waiting state
  RASSERT(TS_WAITING == pTask->eState());
  
  // task must be locked
  RASSERT(pTask->bIsLocked());

  // task must not be in any queue
  RASSERT(QR_NONE == pTask->eQueue());
  
  // mark task as being in the waiting queue
  pTask->vQueue(QR_WAITING_QUEUE);
  pTask->vQueueOrder(++m_ixQueueOrder);
  
  if (pTask->pJob()->m_bMarkActivity)
    ActivityMap::vMarkJob(pTask->pJob(),
                       "Add to WaitingSet: %s %p"_Stz,
                       aCN[pTask->pJob()->eClass()],
                       pTask->pJob());

  // put task into the waiting queue
  ASSERT(m_pWaitingSet->bIsLocked());
  m_pWaitingSet->vAddTask(pTask);
  
  ActionSequence::vAdd(pTask->pJob(),AN_ADD_WAITING_SET);

  // drop task lock
  pTask->vDropLock();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* check the sleeping queue                                                   */
/*----------------------------------------------------------------------------*/
void Queues::vCheckSleeping(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);
  
  while (Task* pWokenTask = pPopSleepingQueue()) {
     vScheduleWokenTask(pWokenTask);
  }
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump the executing set                                                     */
/*----------------------------------------------------------------------------*/
void Queues::vDumpExecutingSet(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);
  
  vTrace("------------------------------------\n"_Stz);
  vTrace("Executing Set: %p\n"_Stz, m_pExecutingSet);
  vTrace("------------------------------------\n"_Stz);
  m_pExecutingSet->vGrabLock();
  m_pExecutingSet->vContentDump();
  m_pExecutingSet->vDropLock();
  vTrace("====================================\n"_Stz);
  vTrace("\n"_Stz);
  vTrace("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* dump the ready queue                                                       */
/*----------------------------------------------------------------------------*/
void Queues::vDumpReadyQueue(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);
  
  vTrace("------------------------------------\n"_Stz);
  vTrace("Ready Queue (normal priority): %p\n"_Stz, m_pReadyQueue);
  vTrace("------------------------------------\n"_Stz);
  m_pReadyQueue->vGrabLock();
  m_pReadyQueue->vContentDump();
  m_pReadyQueue->vDropLock();
  vTrace("====================================\n"_Stz);
  vTrace("\n"_Stz);
  vTrace("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* dump the low priority ready queue                                          */
/*----------------------------------------------------------------------------*/
void Queues::vDumpReadyQueueLP(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);
  
  vTrace("------------------------------------\n"_Stz);
  vTrace("Ready Queue (low Priority): %p\n"_Stz, m_pReadyQueueLP);
  vTrace("------------------------------------\n"_Stz);
  m_pReadyQueueLP->vGrabLock();
  m_pReadyQueueLP->vContentDump();
  m_pReadyQueueLP->vDropLock();
  vTrace("====================================\n"_Stz);
  vTrace("\n"_Stz);
  vTrace("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* dump sleeping queue                                                        */
/*----------------------------------------------------------------------------*/
void Queues::vDumpSleepingQueue(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);
  
  vTrace("------------------------------------\n"_Stz);
  vTrace("Sleeping Queue: %p\n"_Stz, m_pSleepingQueue);
  vTrace("------------------------------------\n"_Stz);
  m_pSleepingQueue->vGrabLock();
  m_pSleepingQueue->vContentDump();
  m_pSleepingQueue->vDropLock();
  vTrace("====================================\n"_Stz);
  vTrace("\n"_Stz);
  vTrace("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* dump waiting set                                                           */
/*----------------------------------------------------------------------------*/
void Queues::vDumpWaitingSet(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);
  
  vTrace("------------------------------------\n"_Stz);
  vTrace("Waiting Set: %p\n"_Stz, m_pWaitingSet);
  vTrace("------------------------------------\n"_Stz);
  m_pWaitingSet->vGrabLock();
  m_pWaitingSet->vContentDump();
  m_pWaitingSet->vDropLock();
  vTrace("====================================\n"_Stz);
  vTrace("\n"_Stz);
  vTrace("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* attempt to find idle thread                                                */
/*----------------------------------------------------------------------------*/
Thread* Queues::pFindIdleThread(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);
  
  Thread* pThisThread = nullptr;
  
  if ( ! Pthread::bIsJavaThread() )
    pThisThread = Thread::pThisTLSThread();
  
  m_pGenThreadVec->vGrabLock();

  i8 nThreads = m_pGenThreadVec->nItems();
  for (i8 i_Thread = 0; i_Thread < nThreads; i_Thread++) {

    Thread* pThread = m_pGenThreadVec->pGet(i_Thread);
    
    // skip this thread
    if (pThread == pThisThread)
      continue;

    // attempt to grab this thread as an idle thread
    if (pThread->bGrabIfIdle()) {
      m_pGenThreadVec->vDropLock();
      return pThread;
    }
  }
  
  m_pGenThreadVec->vDropLock();
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get a portrait of the queues                                               */
/*----------------------------------------------------------------------------*/
void Queues::vMonitorPortrait(s48K& sPortrait)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);
  
  s48K sTasksPortrait;
  
  sPortrait = ""_Stz;
  sPortrait.vConcat("Queues:\n"_Stz);
  
  m_pExecutingSet->vPortrait(sTasksPortrait);
  sPortrait.vConcat(sTasksPortrait);
  
  m_pReadyQueue->vPortrait(sTasksPortrait);
  sPortrait.vConcat(sTasksPortrait);
  
  m_pReadyQueueLP->vPortrait(sTasksPortrait);
  sPortrait.vConcat(sTasksPortrait);
  
  m_pWaitingSet->vPortrait(sTasksPortrait);
  sPortrait.vConcat(sTasksPortrait);
  
  m_pSleepingQueue->vPortrait(sTasksPortrait);
  sPortrait.vConcat(sTasksPortrait);
}

/*----------------------------------------------------------------------------*/
/* get a portrait of the threads                                              */
/*----------------------------------------------------------------------------*/
void Queues::vMonitorThread(s48K& sPortrait)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);
  
  i8 nIdle = 0;
  i8 nRunning = 0;
  i8 nDedicated = 0;
  
  m_pGenThreadVec->vGrabLock();
  
  i8 nThreads = m_pGenThreadVec->nItems();
  for (i8 i_Thread = 0; i_Thread < nThreads; i_Thread++) {
    
    Thread* pThread = m_pGenThreadVec->pGet(i_Thread);
    
    if (TD_IDLE == pThread->eThreadState())
      nIdle++;
    else
      if (TD_RUNNING == pThread->eThreadState())
        nRunning++;

    if (CN_NONE != pThread->m_eDedicated)
      nDedicated++;
  }
  
  m_pGenThreadVec->vDropLock();
  
  sPortrait.vSprint("Threads: running %lld (dedicated %lld) idle %lld\n"_Stz,
                    nThreads,
                      nDedicated,
                    nIdle);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* pause till general threads are all idle                                    */
/*----------------------------------------------------------------------------*/
void Queues::vPauseForGeneralThreads(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);
  
  // pause till all general threads have all begun
  while (*gpAttachMap->m_ixThreads != gpAttachMap->Arg.m_nThreads)
    Pthread::vSleepMsec(10,CN_NONE);
  
  RASSERT(m_pGenThreadVec->nItems() == gpAttachMap->Arg.m_nThreads);
  
  // pause till all general threads are in idle state
  for (i8 i_Thread = 0; i_Thread < m_pGenThreadVec->nItems(); i_Thread++) {
    
    Thread* pThread = m_pGenThreadVec->pGet(i_Thread);
    
    while (TD_IDLE != pThread->m_eThreadState)
      Pthread::vSleepMsec(10,CN_QUEUES);
  }
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* pop next queued task in round robin fashion                                */
/*----------------------------------------------------------------------------*/
bool Queues::bPopQueuedTask(Task** ppPoppedTask, Thread* pCallingThread)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);
  
  Task* pPoppedTask;
  
  // set popped task to be null to start with
  *ppPoppedTask = NULL;
  
  if ( (pPoppedTask = pPopReadyQueue()) ) {
    *ppPoppedTask = pPoppedTask;
    return true;
  }
  
  if ( (pPoppedTask = pPopReadyQueueLP()) ) {
    *ppPoppedTask = pPoppedTask;
    return true;
  }
  
  return false;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* pop task from the ready queue                                              */
/*----------------------------------------------------------------------------*/
Task* Queues::pPopReadyQueue(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);
  
  Task* pTask;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // lock queue and check for task
  
  // attempt to grab a write lock on the general queue
  if (! m_pReadyQueue->bGrabLock()) {
    // unable to lock queue (another thread must be processing it)
    // so move on to the next queue
    return nullptr;
  }
  
  //m_pReadyQueue->vGrabLock();
  
  // pop task off the general queue
  pTask = m_pReadyQueue->pPopTask();
 
  // task no longer in any queue/set
  if (pTask)
    pTask->vQueue(QR_NONE);

  // drop the lock
  m_pReadyQueue->vDropLock();
  
  // is there a task to run?
  if (! pTask)
    return nullptr;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  if (pTask->pJob()->m_bMarkActivity)
    ActivityMap::vMarkJob(pTask->pJob(),"pPopReadyQueue()"_Stz);
  
  // return the task popped from the ready queue
  return pTask;
}

/*----------------------------------------------------------------------------*/
/* pop task from the low priority ready queue                                 */
/*----------------------------------------------------------------------------*/
Task* Queues::pPopReadyQueueLP(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);
  
  Task* pTask;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // lock queue and check for task
  
  // attempt to grab a write lock on the general queue
  if (! m_pReadyQueueLP->bGrabLock()) {
    // unable to lock queue (another thread must be processing it)
    // so move on to the next queue
    return nullptr;
  }
  
  //m_pReadyQueue->vGrabLock();
  
  // pop task off the general queue
  pTask = m_pReadyQueueLP->pPopTask();
  
  // task no longer in any queue/set
  if (pTask)
    pTask->vQueue(QR_NONE);
  
  // drop the lock
  m_pReadyQueueLP->vDropLock();
  
  // is there a task to run?
  if (! pTask)
    return nullptr;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  if (pTask->pJob()->m_bMarkActivity)
    ActivityMap::vMarkJob(pTask->pJob(),"pPopReadyQueueLP()"_Stz);
  
  // return the task popped from the ready queue
  return pTask;
}

/*----------------------------------------------------------------------------*/
/* pop task from sleeping queue                                               */
/*----------------------------------------------------------------------------*/
Task* Queues::pPopSleepingQueue(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);
  
  Task* pTask;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // lock queue and peek at task
  
  // attempt to grab a write lock on the sleeping queue
  if (! m_pSleepingQueue->bGrabLock()) {
    // unable to lock queue (another thread must be processing it)
    // so move on to the next queue
    return nullptr;
  }
  
  // peek at the first task in the sleeping queue
  ASSERT(m_pSleepingQueue->bIsLocked());
  pTask = m_pSleepingQueue->pPeekTask();
  
  // check if peeked task's wakeup time is after now
  if (! pTask || ! pTask->bWakeupNow()) {
    
    // - peeked task is to be woken up in the future
    
    // drop lock on sleeping queue
    m_pSleepingQueue->vDropLock();
    
    // didn't find a task, move to next queue
    return nullptr;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // peeked task can be be woken up
  
  // pop the task off the sleeping queue
  ASSERT(m_pSleepingQueue->bIsLocked());
  i8 nPriorItems = m_pSleepingQueue->nTasks();
  m_pSleepingQueue->vAssertItems(nPriorItems);
  pTask = m_pSleepingQueue->pPopTask();
  m_pSleepingQueue->vAssertItems(nPriorItems - 1);

  // task no longer in any queue/set
  pTask->vQueue(QR_NONE);
  
  // drop lock on sleeping queue
  m_pSleepingQueue->vDropLock();
  
  // clear wakeup time of the task
  pTask->vWakeupTask();
  
  if (pTask->pJob()->m_bMarkActivity)
    ActivityMap::vMarkInfo(
                       "Popped SleepingQueue: %s %p"_Stz,
                       aCN[pTask->pJob()->eClass()],
                       pTask->pJob());

  // return the task popped from the sleeping queue
  return pTask;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get the first queue                                                        */
/*----------------------------------------------------------------------------*/
i8 Queues::i_QueueFirst(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);
  
  return QR_READY_QUEUE;
}

/*----------------------------------------------------------------------------*/
/* get the last queue                                                         */
/*----------------------------------------------------------------------------*/
i8 Queues::i_QueueLast(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);
  
  return QR_READY_QUEUE_LP;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* remove thread from thread vector                                           */
/*----------------------------------------------------------------------------*/
void Queues::vRemoveThread(Thread* pThread)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);
  
  m_pGenThreadVec->vGrabLock();
  m_pGenThreadVec->vRemove(pThread);
  m_pGenThreadVec->vDropLock();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* remove task from executing queue                                           */
/*----------------------------------------------------------------------------*/
void Queues::vRemoveFromExecutingSet(Task* pTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);
  
  m_pExecutingSet->vGrabLock();
  m_pExecutingSet->vRemoveTask(pTask);
  m_pExecutingSet->vDropLock();
  
  // task no longer in any queue/set
  pTask->vQueue(QR_NONE);
  
  if (pTask->pJob() && pTask->pJob()->m_bMarkActivity)
    ActivityMap::vMarkJob(pTask->pJob(),
                       "Removed from ExecutingSet: %s %p"_Stz,
                       aCN[pTask->pJob()->eClass()],
                       pTask->pJob());
}

/*----------------------------------------------------------------------------*/
/* remove task from waiting queue                                             */
/*----------------------------------------------------------------------------*/
void Queues::vRemoveFromWaitingSet(Task* pTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);
  
  m_pWaitingSet->vGrabLock();
  m_pWaitingSet->vRemoveTask(pTask);
  m_pWaitingSet->vDropLock();
  
  // task no longer in any queue/set
  pTask->vQueue(QR_NONE);
  
  if (pTask->pJob()->m_bMarkActivity)
    ActivityMap::vMarkJob(pTask->pJob(),
                       "Removed from WaitingSet: %s %p"_Stz,
                       aCN[pTask->pJob()->eClass()],
                       pTask->pJob());
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* schedule a fully acked task                                                */
/*----------------------------------------------------------------------------*/
void Queues::vScheduleAckedTask(Task* pTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);
  
  if (pTask->pJob()->m_bMarkActivity)
    ActivityMap::vMarkJob(pTask->pJob(),
                       "Scheduling acked: %s %p"_Stz,
                       aCN[pTask->pJob()->eClass()],
                       pTask->pJob());
  
  //ActionSequence::vAdd(pTask->pJob(),AN_TASK_ACKED);

  pTask->vGrabLock();

  // having got the lock, the task should be TS_WAITING and QR_WAITING_QUEUE

  RASSERT(TS_WAITING == pTask->eState());
  RASSERT(QR_WAITING_QUEUE == pTask->eQueue());
  
  pTask->vDropLock();
  
  vRemoveFromWaitingSet(pTask);

  RASSERT(QR_NONE == pTask->eQueue());
  
  pTask->vState(TS_READY);
  
  vScheduleReadyTask(pTask);
}

/*----------------------------------------------------------------------------*/
/* schedule an exited task                                                    */
/*----------------------------------------------------------------------------*/
void Queues::vScheduleExitedTask(Task* pTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);

  RASSERT(TS_EXITED == pTask->eState());
  
  ASSERT(! pTask->bWordBody());
  
  //vTrace("Exited Task %p\n"_Stz,pTask);
  
  delete pTask;
}

/*----------------------------------------------------------------------------*/
/* schedule a ready to execute task                                           */
/*----------------------------------------------------------------------------*/
void Queues::vScheduleReadyTask(Task* pTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);
  
  RASSERT(TS_READY == pTask->eState());
  
  if (pTask->pJob()->m_bMarkActivity)
    ActivityMap::vMarkJob(pTask->pJob(),
                          "Schedule ready: %s %p"_Stz,
                          aCN[pTask->pJob()->eClass()],
                          pTask->pJob());

  Thread* pIdleThread = pFindIdleThread();

  if (pIdleThread) {
    ActionSequence::vAdd(pTask->pJob(),AN_THREAD_EXECUTE,pIdleThread->iThreadNo());
    pIdleThread->vExecuteTask(pTask);
  }
  else {
    if (pTask->pJob()->m_pJobClass->m_ePriority == PY_NORMAL)
      vAddToReadyQueue(pTask);
    else
      vAddToReadyQueueLP(pTask);
  }
}

/*----------------------------------------------------------------------------*/
/* schedule sleeping task                                                     */
/*----------------------------------------------------------------------------*/
void Queues::vScheduleSleepingTask(Task* pTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);
  
  RASSERT(TS_SLEEPING == pTask->eState());
  
  if (pTask->pJob()->m_bMarkActivity)
    ActivityMap::vMarkJob(pTask->pJob(),
                       "Schedule sleeping: %s %p"_Stz,
                       aCN[pTask->pJob()->eClass()],
                       pTask->pJob());

  // is the task still actually sleeping
  if (! pTask->bWakeupNow()) {
    vAddToSleepingQueue(pTask);
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // tasks wake up time has already arrived. run it
  
  pTask->vWakeupTask();
  
  Thread* pCallingThread = Thread::pThisTLSThread();
  
  pCallingThread->vReExecuteTask(pTask);
}

/*----------------------------------------------------------------------------*/
/* schedule a newly spawned task                                              */
/*----------------------------------------------------------------------------*/
void Queues::vScheduleSpawnedTask(Task* pTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);
  
  RASSERT(QR_NONE == pTask->eQueue());
  
  pTask->vState(TS_READY);
  
  if (pTask->pJob()->m_bMarkActivity) {
    if (C_UNKNOWN == pTask->pJob()->iJobId())
      ActivityMap::vMarkInfo("Schedule spawned: %s %p slot[%lld]"_Stz,
                             aCN[pTask->pJob()->eClass()],
                             pTask->pJob(),
                             pTask->pJob()->m_i_Slot);
    else
      ActivityMap::vMarkJob(pTask->pJob(),
                            "Schedule spawned: %s %p slot[%lld]"_Stz,
                            aCN[pTask->pJob()->eClass()],
                            pTask->pJob(),
                            pTask->pJob()->m_i_Slot);
  }
  
  eQR eQueue = pTask->eWhichQueue();
  RASSERT(eQueue >= QR_READY_QUEUE  || eQueue < gpAttachMap->m_pGrid->m_nSlotsPerNode);
  
  //vTrace("Spawned Task %p\n"_Stz,pTask);
  
  vScheduleReadyTask(pTask);
}

/*----------------------------------------------------------------------------*/
/* schedule ready waiting task                                                */
/*----------------------------------------------------------------------------*/
void Queues::vScheduleWaitingTask(Task* pTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);
  
  RASSERT(TS_WAITING == pTask->eState());
  
  if (pTask->pJob()->m_bMarkActivity) {
    if (C_UNKNOWN == pTask->pJob()->iJobId())
      ActivityMap::vMarkInfo(
                         "Schedule waiting: %s %p"_Stz,
                         aCN[pTask->pJob()->eClass()],
                         pTask->pJob());
    else
      ActivityMap::vMarkJob(pTask->pJob(),
                         "Schedule waiting: %s %p"_Stz,
                         aCN[pTask->pJob()->eClass()],
                         pTask->pJob());
 }

  vAddToWaitingSet(pTask);
}

/*----------------------------------------------------------------------------*/
/* schedule woken task                                                        */
/*----------------------------------------------------------------------------*/
void Queues::vScheduleWokenTask(Task* pTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);
  
  pTask->vState(TS_READY);
  
  eQR eQueue = pTask->eWhichQueue();
  RASSERT(eQueue >= QR_READY_QUEUE  || eQueue < gpAttachMap->m_pGrid->m_nSlotsPerNode);
  
  //vTrace("Spawned Task %p\n"_Stz,pTask);
  
  vScheduleReadyTask(pTask);
}

/*----------------------------------------------------------------------------*/
/* schedule yielded task                                                      */
/*----------------------------------------------------------------------------*/
void Queues::vScheduleYieldedTask(Task* pTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);
  
  Thread* pCallingThread = Thread::pThisTLSThread();
  
  if (pTask->pJob()->m_bMarkActivity) {
    if (C_UNKNOWN == pTask->pJob()->iJobId())
      ActivityMap::vMarkInfo(
                         "Schedule yielded: %s %p"_Stz,
                         aCN[pTask->pJob()->eClass()],
                         pTask->pJob());
    else
      ActivityMap::vMarkJob(pTask->pJob(),
                         "Schedule yielded: %s %p"_Stz,
                         aCN[pTask->pJob()->eClass()],
                         pTask->pJob());
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // non-partitioned task yield
  
  
  // - A general thread has executed a task which retured TS_YIELDED.
  //   If the ready queue is empty then this general thread can
  //     continue to execute the yield task.
  //   Otherwise we add the task to the ready queue
  
  if (pTask->pJob()->m_pJobClass->m_ePriority == PY_NORMAL) {
    m_pReadyQueue->vGrabLock();
    i8 iLengthReadyQueue = nTasksReady();
    if (0 == iLengthReadyQueue) {
      m_pReadyQueue->vDropLock();
      pCallingThread->vReExecuteTask(pTask);
    }
    else {
      pTask->vState(TS_READY);
      vAddToReadyQueue_NL(pTask);
      m_pReadyQueue->vDropLock();
    }
  }
  else {
    m_pReadyQueueLP->vGrabLock();
    pTask->vState(TS_READY);
    vAddToReadyQueueLP_NL(pTask);
    m_pReadyQueueLP->vDropLock();
  }
  
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get no of tasks in the executing set                                       */
/*----------------------------------------------------------------------------*/
i8 Queues::nTasksExecuting(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);
  
  return m_pExecutingSet->nTasks();
}

/*----------------------------------------------------------------------------*/
/* get no of tasks in the ready queue                                         */
/*----------------------------------------------------------------------------*/
i8 Queues::nTasksReady(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);
  
  return m_pReadyQueue->nTasks();
}

/*----------------------------------------------------------------------------*/
/* get no of tasks in the low priority ready queue                            */
/*----------------------------------------------------------------------------*/
i8 Queues::nTasksReadyLP(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);
  
  return m_pReadyQueueLP->nTasks();
}

/*----------------------------------------------------------------------------*/
/* get no of tasks in the sleeping queue                                      */
/*----------------------------------------------------------------------------*/
i8 Queues::nTasksSleeping(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);
  
  return m_pSleepingQueue->nTasks();
}

/*----------------------------------------------------------------------------*/
/* get no of tasks in the waiting set                                         */
/*----------------------------------------------------------------------------*/
i8 Queues::nTasksWaiting(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);
  
  return m_pWaitingSet->nTasks();
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
