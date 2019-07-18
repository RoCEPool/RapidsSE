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
  
  m_pDedicatedThreadVec = new ThreadVec;
  m_pGeneralThreadVec   = new ThreadVec;
  
  m_pIdleThreadQueue    = new ThreadQueue;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // task queues
  
  m_pReadyQueue    = new TaskList(QK_READY_QUEUE);
  m_pReadyQueueLP  = new TaskList(QK_READY_QUEUE_LP);
  m_pSleepingQueue = new TaskQueue(QK_SLEEPING_QUEUE,
                                   C_NON_UNQIUE_KEY,
                                   bTaskCompareWakeupTime);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // task series ordered by arrival
  
  m_ixQueueOrder   = 0;
  
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
  i8 iUserTime;
  i8 iSystemTime;
  i8 iPercentageUsage;
  i8 iSleepSsec;
  
  Queues* pQueues = gpAttachMap->m_pQueues;
  
  pQueues->m_pGeneralThreadVec->vGrabLock();

  i8 nThreads = pQueues->m_pGeneralThreadVec->nItems();
  for (i8 i_Thread = 0; i_Thread < nThreads; i_Thread++) {
    
    Thread* pThread = pQueues->m_pGeneralThreadVec->pGet(i_Thread);
    
    pThread->m_pPthread->bGetCpu(iUserTime,iSystemTime,
                                 iPercentageUsage,iSleepSsec);
    
    vTrace("  Gen Thread %2lld %p"
           " base %llx size %lld"
           " user %lld system %lld usage %lld%% sleep %llds"
           " %7s %s"
           " assigned %lld\n"_Stz,
           pThread->m_iThreadNo,
           pThread,
           
           pThread->m_aStackBase,
           pThread->m_iStackSize,
           
           iUserTime,
           iSystemTime,
           iPercentageUsage,
           iSleepSsec,
           
           aTD[pThread->m_eThreadState],
           pThread->m_eDedicated ? aCN[pThread->m_eDedicated]->aChar() : "",
           
           pThread->m_nAssignedTasks
           );
  }
  
  pQueues->m_pGeneralThreadVec->vDropLock();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  pQueues->m_pDedicatedThreadVec->vGrabLock();
  
  nThreads = pQueues->m_pDedicatedThreadVec->nItems();
  for (i8 i_Thread = 0; i_Thread < nThreads; i_Thread++) {
    
    Thread* pThread = pQueues->m_pDedicatedThreadVec->pGet(i_Thread);
    
    pThread->m_pPthread->bGetCpu(iUserTime,iSystemTime,
                                 iPercentageUsage,iSleepSsec);
    
    vTrace("  Ded Thread %2lld %p"
           " base %llx size %lld"
           " user %lld system %lld usage %lld%% sleep %llds"
           " %7s %s"
           " assigned %lld\n"_Stz,
           pThread->m_iThreadNo,
           pThread,
           
           pThread->m_aStackBase,
           pThread->m_iStackSize,
           
           iUserTime,
           iSystemTime,
           iPercentageUsage,
           iSleepSsec,
           
           aTD[pThread->m_eThreadState],
           pThread->m_eDedicated ? aCN[pThread->m_eDedicated]->aChar() : "",
           
           pThread->m_nAssignedTasks
           );
  }
  
  pQueues->m_pDedicatedThreadVec->vDropLock();
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

////////////////////////////////////////////////////////////////////////////////
//                          implementation for tasks                          //
////////////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add task to executing queue                                                */
/*----------------------------------------------------------------------------*/
void Queues::vAddExecutingSet(Task* pTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);
  
  m_pExecutingSet->vGrabLock();
  // + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + +
  
  RASSERT(TS_EXECUTING == pTask->eState());
  
  pTask->vQueue(QK_EXECUTING_SET);
  pTask->vQueueOrder(++m_ixQueueOrder);
  
//  if (pTask->pJob()->m_bMarkActivity)
//    ActivityLog::vMarkJob(pTask->pJob(),
//                          "Add to ExecutingSet: %s %p"_Stz,
//                          aCN[pTask->pJob()->eClass()],
//                          pTask->pJob());
  
  ASSERT(m_pExecutingSet->bIsLocked());
  m_pExecutingSet->vAddTask(pTask);
  
//  ActionSequence::vAdd(pTask->pJob(),AN_ADD_EXECUTING_SET);
  
  // + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + +
  m_pExecutingSet->vDropLock();
}

/*----------------------------------------------------------------------------*/
/* add task to sleeping queue                                                 */
/*----------------------------------------------------------------------------*/
void Queues::vAddSleepingQueue(Task* pTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);
  
  m_pSleepingQueue->vGrabLock();
  // + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + +
  
  RASSERT(TS_SLEEPING == pTask->eState());
  
  pTask->vQueue(QK_SLEEPING_QUEUE);
  
  if (pTask->pJob()->m_bMarkActivity)
    ActivityLog::vMarkJob(pTask->pJob(),
                          "Add to SleepingQueue: %s %p"_Stz,
                          aCN[pTask->pJob()->eClass()],
                          pTask->pJob());
  
  ASSERT(m_pSleepingQueue->bIsLocked());
  
  i8 nPriorItems = m_pSleepingQueue->nTasks();
  m_pSleepingQueue->vAssertItems(nPriorItems);
  m_pSleepingQueue->vAddTask(pTask);
  m_pSleepingQueue->vAssertItems(nPriorItems + 1);
  
  ActionSequence::vAdd(pTask->pJob(),AN_ADD_SLEEPING_QUEUE);
  
  // + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + +
  m_pSleepingQueue->vDropLock();
}

/*----------------------------------------------------------------------------*/
/* add task to waiting queue                                                  */
/*----------------------------------------------------------------------------*/
void Queues::vAddWaitingSet(Task* pTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);
  
  m_pWaitingSet->vGrabLock();
  // + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + +
  
  // task must be in the waiting state
  RASSERT(TS_WAITING == pTask->eState());
  
  // task must be locked
  RASSERT(pTask->bIsLocked());
  
  // task must not be in any queue
  RASSERT(QK_NONE == pTask->eQueue());
  
 
  // mark task as being in the waiting queue
  pTask->vQueue(QK_WAITING_SET);
  pTask->vQueueOrder(++m_ixQueueOrder);
  
  if (pTask->pJob()->m_bMarkActivity)
    ActivityLog::vMarkJob(pTask->pJob(),
                          "Add to WaitingSet: %s %p"_Stz,
                          aCN[pTask->pJob()->eClass()],
                          pTask->pJob());
  
  // put task into the waiting queue
  ASSERT(m_pWaitingSet->bIsLocked());
  m_pWaitingSet->vAddTask(pTask);
  
  ActionSequence::vAdd(pTask->pJob(),AN_ADD_WAITING_SET);
  
  // + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + +
  m_pWaitingSet->vDropLock();
}

/*----------------------------------------------------------------------------*/
/* add task to ready queue                                                    */
/*----------------------------------------------------------------------------*/
void Queues::vAppendReadyQueue(Task* pTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);
  
  m_pReadyQueue->vGrabLock();
  // + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + +
  
  RASSERT(TS_READY == pTask->eState());
  
  pTask->vQueue(QK_READY_QUEUE);
  pTask->vQueueOrder(++m_ixQueueOrder);
  
  if (pTask->pJob()->m_bMarkActivity)
    ActivityLog::vMarkJob(pTask->pJob(),
                          "Add to ReadyQueue: %s %p"_Stz,
                          aCN[pTask->pJob()->eClass()],
                          pTask->pJob());
  
  ASSERT(m_pReadyQueue->bIsLocked());
  
  m_pReadyQueue->vAddTask(pTask);
  
  ActionSequence::vAdd(pTask->pJob(),AN_ADD_READY_QUEUE);
  
  // + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + +
  m_pReadyQueue->vDropLock();
}

/*----------------------------------------------------------------------------*/
/* add task to low priority ready queue                                       */
/*----------------------------------------------------------------------------*/
void Queues::vAppendReadyQueueLP(Task* pTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);
  
  m_pReadyQueueLP->vGrabLock();
  // + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + +
  
  RASSERT(TS_READY == pTask->eState());
  
  pTask->vQueue(QK_READY_QUEUE_LP);
  pTask->vQueueOrder(++m_ixQueueOrder);
  
  if (pTask->pJob()->m_bMarkActivity)
    ActivityLog::vMarkJob(pTask->pJob(),
                          "Add to ReadyQueueLP: %s %p"_Stz,
                          aCN[pTask->pJob()->eClass()],
                          pTask->pJob());
  
  ASSERT(m_pReadyQueueLP->bIsLocked());
  
  m_pReadyQueueLP->vAddTask(pTask);
  
  ActionSequence::vAdd(pTask->pJob(),AN_ADD_READY_QUEUE_LP);
  
  // + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + +
  m_pReadyQueueLP->vDropLock();
}

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
  if (pTask) {
    pTask->vQueue(QK_NONE);
  }
  
  // drop the lock
  m_pReadyQueue->vDropLock();
  
  // is there a task to run?
  if (! pTask)
    return nullptr;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  if (pTask->pJob()->m_bMarkActivity)
    ActivityLog::vMarkJob(pTask->pJob(),"pPopReadyQueue()"_Stz);
  
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
  if (pTask) {
    pTask->vQueue(QK_NONE);
  }
  
  // drop the lock
  m_pReadyQueueLP->vDropLock();
  
  // is there a task to run?
  if (! pTask)
    return nullptr;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  if (pTask->pJob()->m_bMarkActivity)
    ActivityLog::vMarkJob(pTask->pJob(),"pPopReadyQueueLP()"_Stz);
  
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
  pTask->vQueue(QK_NONE);
  
  // drop lock on sleeping queue
  m_pSleepingQueue->vDropLock();
  
  // clear wakeup time of the task
  pTask->vWakeupTask();
  
  if (pTask->pJob()->m_bMarkActivity)
    ActivityLog::vMarkInfo(
                           "Popped SleepingQueue: %s %p"_Stz,
                           aCN[pTask->pJob()->eClass()],
                           pTask->pJob());
  
  // return the task popped from the sleeping queue
  return pTask;
}

/*----------------------------------------------------------------------------*/
/* remove task from executing queue                                           */
/*----------------------------------------------------------------------------*/
void Queues::vRemoveExecutingSet(Task* pTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);
  
  RASSERT(QK_EXECUTING_SET == pTask->eQueue());
  
  m_pExecutingSet->vGrabLock();
  m_pExecutingSet->vRemoveTask(pTask);
  m_pExecutingSet->vDropLock();
  
  // task no longer in any queue/set
  pTask->vQueue(QK_NONE);
  
//  if (pTask->pJob() && pTask->pJob()->m_bMarkActivity)
//    ActivityLog::vMarkJob(pTask->pJob(),
//                          "Removed from ExecutingSet: %s %p"_Stz,
//                          aCN[pTask->pJob()->eClass()],
//                          pTask->pJob());
}

/*----------------------------------------------------------------------------*/
/* remove task from waiting queue                                             */
/*----------------------------------------------------------------------------*/
void Queues::vRemoveWaitingSet(Task* pTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);
  
  RASSERT(TS_WAITING == pTask->eState());
  RASSERT(QK_WAITING_SET == pTask->eQueue());
  RASSERT(0 == pTask->iAcksRmg_assert_only_());
  
  m_pWaitingSet->vGrabLock();
  m_pWaitingSet->vRemoveTask(pTask);
  m_pWaitingSet->vDropLock();
  
  // task no longer in any queue/set
  pTask->vQueue(QK_NONE);
  
  if (pTask->pJob()->m_bMarkActivity)
    ActivityLog::vMarkJob(pTask->pJob(),
                          "Removed from WaitingSet: %s %p"_Stz,
                          aCN[pTask->pJob()->eClass()],
                          pTask->pJob());
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


////////////////////////////////////////////////////////////////////////////////
//                          implementation for threads                        //
////////////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add dedicated thread to thread vector                                      */
/*----------------------------------------------------------------------------*/
void Queues::vThreadAddDedicated(Thread* pThread)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);
  
  RASSERT(TD_IDLE == pThread->eThreadState());
  
  m_pDedicatedThreadVec->vGrabLock();
  m_pDedicatedThreadVec->vAdd(pThread);
  m_pDedicatedThreadVec->vDropLock();
}

/*----------------------------------------------------------------------------*/
/* add general thread to thread vector                                        */
/*----------------------------------------------------------------------------*/
void Queues::vThreadAddGeneral(Thread* pThread)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);
  
  RASSERT(TD_IDLE == pThread->eThreadState());
  
  m_pGeneralThreadVec->vGrabLock();
  m_pGeneralThreadVec->vAdd(pThread);
  m_pGeneralThreadVec->vDropLock();
}

/*----------------------------------------------------------------------------*/
/* append task to waiting queue                                               */
/*----------------------------------------------------------------------------*/
void Queues::vThreadAppendIdle(Thread* pThread)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);
  
  m_pIdleThreadQueue->vGrabLock();
  m_pIdleThreadQueue->vAppendThread(pThread);
  m_pIdleThreadQueue->vDropLock();
}

/*----------------------------------------------------------------------------*/
/* pop idle thread                                                            */
/*----------------------------------------------------------------------------*/
Thread* Queues::pThreadPopIdle(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);
  
  m_pIdleThreadQueue->vGrabLock();
  
  Thread* pThread = m_pIdleThreadQueue->pPopThread();
  
  m_pIdleThreadQueue->vDropLock();
  
  return pThread;
}


/*----------------------------------------------------------------------------*/
/* remove thread from dedicated thread vector                                 */
/*----------------------------------------------------------------------------*/
void Queues::vThreadRemoveDedicated(Thread* pThread)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);
  
  m_pDedicatedThreadVec->vGrabLock();
  m_pDedicatedThreadVec->vRemove(pThread);
  m_pDedicatedThreadVec->vDropLock();
}

/*----------------------------------------------------------------------------*/
/* remove thread from general thread vector                                   */
/*----------------------------------------------------------------------------*/
void Queues::vThreadRemoveGeneral(Thread* pThread)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);
  
  m_pGeneralThreadVec->vGrabLock();
  m_pGeneralThreadVec->vRemove(pThread);
  m_pGeneralThreadVec->vDropLock();
}


////////////////////////////////////////////////////////////////////////////////
//                                 misc                                       //
////////////////////////////////////////////////////////////////////////////////

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
  
  m_pGeneralThreadVec->vGrabLock();
  
  i8 nThreads = m_pGeneralThreadVec->nItems();
  for (i8 i_Thread = 0; i_Thread < nThreads; i_Thread++) {
    
    Thread* pThread = m_pGeneralThreadVec->pGet(i_Thread);
    
    if (TD_IDLE == pThread->eThreadState())
      nIdle++;
    else
      if (TD_ASSIGNED == pThread->eThreadState())
        nRunning++;
    
    if (CN_NONE != pThread->m_eDedicated)
      nDedicated++;
  }
  
  m_pGeneralThreadVec->vDropLock();
  
  sPortrait.vSprint("Threads: running %lld (dedicated %lld) idle %lld\n"_Stz,
                    nThreads,
                    nDedicated,
                    nIdle);
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

__aFooterFunction(Queues);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
