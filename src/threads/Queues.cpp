/*============================================================================*/
/* Queues.cpp -- Collection of the Systems' Queues                            */
/*============================================================================*/

#include "Threads.h"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Queues* Threads::_pQueues(void* aAddr)
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
  vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
         "fatal: Queues class cannot be deleted"_Stz);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Queues::Queues(void)
: Focal(CN_QUEUES,0,C_UNKNOWN)
, m_okScheduling(LK_QUEUES,this)
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
  // task sets
  
  m_pBidingSet     = new TaskList(QK_BIDING_SET);
  m_pExecutingSet  = new TaskList(QK_EXECUTING_SET);
  m_pWaitingSet    = new TaskList(QK_WAITING_SET);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // jobs
  
  m_pDemonJobs         = new JobList(bJobCompare,JL_DEMON);
  m_pLocalJobs         = new JobList(bJobCompare,JL_LOCAL);
  m_pOriginatingJobs   = new JobList(bJobCompare,JL_ORIGINATING);
  m_pParticipatingJobs = new JobList(bJobCompare,JL_PARTICIPATING);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

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
  
  Queues* pQueues = AM::pQueues();
  
  pQueues->m_pGeneralThreadVec->vGrabLock();

  i8 nThreads = pQueues->m_pGeneralThreadVec->nItems();
  for (i8 i_Thread = 0; i_Thread < nThreads; i_Thread++) {
    
    Thread* pThread = pQueues->m_pGeneralThreadVec->pGet(i_Thread);
    
    pThread->m_pPsxThd->bGetCpu(iUserTime,iSystemTime,
                                 iPercentageUsage,iSleepSsec);
    
    vTrace("  Gen Thread %2lld %p"
           " base %llx size %lld"
           " user %lld system %lld usage %lld%% sleep %llds"
           " %s %s"
           " assigned %lld\n"_Stz,
           pThread->iThreadNo(),
           pThread,
           
           pThread->m_pPsxThd->m_aStackBase,
           pThread->m_pPsxThd->m_iStackSize,
           
           iUserTime,
           iSystemTime,
           iPercentageUsage,
           iSleepSsec,
           
           aHS[pThread->m_eThreadState],
           pThread->m_eDedicated ? aCN[pThread->m_eDedicated]->azChar() : "",
           
           pThread->m_nAssignedTasks
           );
  }
  
  pQueues->m_pGeneralThreadVec->vDropLock();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  pQueues->m_pDedicatedThreadVec->vGrabLock();
  
  nThreads = pQueues->m_pDedicatedThreadVec->nItems();
  for (i8 i_Thread = 0; i_Thread < nThreads; i_Thread++) {
    
    Thread* pThread = pQueues->m_pDedicatedThreadVec->pGet(i_Thread);
    
    pThread->m_pPsxThd->bGetCpu(iUserTime,iSystemTime,
                                 iPercentageUsage,iSleepSsec);
    
    vTrace("  Ded Thread %2lld %p"
           " base %llx size %lld"
           " user %lld system %lld usage %lld%% sleep %llds"
           " %7s %s"
           " assigned %lld\n"_Stz,
           pThread->iThreadNo(),
           pThread,
           
           pThread->m_pPsxThd->m_aStackBase,
           pThread->m_pPsxThd->m_iStackSize,
           
           iUserTime,
           iSystemTime,
           iPercentageUsage,
           iSleepSsec,
           
           aHS[pThread->m_eThreadState],
           pThread->m_eDedicated ? aCN[pThread->m_eDedicated]->azChar() : "",
           
           pThread->m_nAssignedTasks
           );
  }
  
  pQueues->m_pDedicatedThreadVec->vDropLock();
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* focal outline                                                              */
/*----------------------------------------------------------------------------*/
void Queues::vFocalOutline(s4K& sOutline)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES,_FOOTER_);
  
  sOutline.vSprint("<%s focal>"_Stz,aCN[eClass()]);
}


////////////////////////////////////////////////////////////////////////////////
//                          implementation for tasks                          //
////////////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add task to biding set                                                     */
/*----------------------------------------------------------------------------*/
void Queues::vAddBidingSet(Task* pTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES,_FOOTER_);
  
  // note: a biding task must locked at this point because it is visible to
  //       other tasks in the system which could invoke a trigger() on it
  //       before the biding task gets into the biding set
  RASSERT(pTask->bIsLocked());

  RASSERT(TS_BIDING == pTask->eState());
  RASSERT(QK_NONE == pTask->eQueue());
  
  pTask->vQueue(QK_BIDING_SET);
  
  m_pBidingSet->vGrabLock();
  
  m_pBidingSet->vAddTask(pTask);
  
  m_pBidingSet->vDropLock();
  
  // **do not touch task** (other than to drop lock)
}

/*----------------------------------------------------------------------------*/
/* add task to executing queue                                                */
/*----------------------------------------------------------------------------*/
void Queues::vAddExecutingSet(Task* pTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES,_FOOTER_);
  
  RASSERT(TS_EXECUTING == pTask->eState());
  RASSERT(QK_NONE == pTask->eQueue());

  pTask->vQueue(QK_EXECUTING_SET);
  
  m_pExecutingSet->vGrabLock();
  
  m_pExecutingSet->vAddTask(pTask);
  
  m_pExecutingSet->vDropLock();
  
  // **do not touch task**
}

/*----------------------------------------------------------------------------*/
/* add task to sleeping queue                                                 */
/*----------------------------------------------------------------------------*/
void Queues::vAddSleepingQueue(Task* pTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES,_FOOTER_);
  
  RASSERT(TS_SLEEPING == pTask->eState());
  RASSERT(QK_NONE == pTask->eQueue());

  pTask->vQueue(QK_SLEEPING_QUEUE);

  m_pSleepingQueue->vGrabLock();
  
  i8 nPriorItems = m_pSleepingQueue->nTasks();
  m_pSleepingQueue->vAssertItems(nPriorItems);
  m_pSleepingQueue->vAddTask(pTask);
  m_pSleepingQueue->vAssertItems(nPriorItems + 1);
  
  m_pSleepingQueue->vDropLock();
  
  // **do not touch task**
}

/*----------------------------------------------------------------------------*/
/* add task to waiting set                                                    */
/*----------------------------------------------------------------------------*/
void Queues::vAddWaitingSet(Task* pTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES,_FOOTER_);
  
  // note: a waiting task must locked at this point because it is visible to
  //       other tasks in the system which could invoke an ack() on it
  //       before the waiting task gets into the waiting set
  RASSERT(pTask->bIsLocked());

  RASSERT(TS_WAITING == pTask->eState());
  RASSERT(QK_NONE == pTask->eQueue());

  pTask->vQueue(QK_WAITING_SET);
  
  m_pWaitingSet->vGrabLock();
  
  m_pWaitingSet->vAddTask(pTask);
    
  m_pWaitingSet->vDropLock();
  
  // **do not touch task**
}

/*----------------------------------------------------------------------------*/
/* add task to ready queue (normal priority)                                  */
/*----------------------------------------------------------------------------*/
void Queues::vAppendReadyQueue(Task* pTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES,_FOOTER_);
  
  RASSERT(TS_READY == pTask->eState());
  RASSERT(QK_NONE == pTask->eQueue());

  pTask->vQueue(QK_READY_QUEUE);
  
  m_pReadyQueue->vGrabLock();
  
  m_pReadyQueue->vAddTask(pTask);
  
  m_pReadyQueue->vDropLock();
  
  // **do not touch task**
}

/*----------------------------------------------------------------------------*/
/* add task to low priority ready queue                                       */
/*----------------------------------------------------------------------------*/
void Queues::vAppendReadyQueueLP(Task* pTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES,_FOOTER_);
  
  RASSERT(TS_READY == pTask->eState());
  RASSERT(QK_NONE == pTask->eQueue());
  
  pTask->vQueue(QK_READY_QUEUE_LP);
  
  m_pReadyQueueLP->vGrabLock();
  
  m_pReadyQueueLP->vAddTask(pTask);
    
  m_pReadyQueueLP->vDropLock();
  
  // **do not touch task**
}

/*----------------------------------------------------------------------------*/
/* pop a task from the ready queue (normal priority)                          */
/*----------------------------------------------------------------------------*/
Task* Queues::pPopReadyQueue(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES,_FOOTER_);
  
  Task* pTask;
  
  if (! m_pReadyQueue->bGrabLock()) {
    // unable to lock queue (another thread must be processing it)
    // so move on to the next queue
    return nullptr;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  pTask = m_pReadyQueue->pPopTask();
  
  m_pReadyQueue->vDropLock();
  
  // is there a task to run?
  if (! pTask)
    return nullptr;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  pTask->vQueue(QK_NONE);
  
  return pTask;
}

/*----------------------------------------------------------------------------*/
/* pop a task from the low priority ready queue                               */
/*----------------------------------------------------------------------------*/
Task* Queues::pPopReadyQueueLP(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES,_FOOTER_);
  
  Task* pTask;
  
  if (! m_pReadyQueueLP->bGrabLock()) {
    // unable to lock queue (another thread must be processing it)
    // so move on to the next queue
    return nullptr;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  pTask = m_pReadyQueueLP->pPopTask();
  
  m_pReadyQueueLP->vDropLock();
  
  // is there a task to run?
  if (! pTask)
    return nullptr;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  pTask->vQueue(QK_NONE);
  
  return pTask;
}

/*----------------------------------------------------------------------------*/
/* pop a wakeable task from sleeping queue                                    */
/*----------------------------------------------------------------------------*/
Task* Queues::pPopSleepingQueue(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES,_FOOTER_);
  
  Task* pPeekTask;
  
  if (! m_pSleepingQueue->bGrabLock()) {
    // unable to lock queue (another thread must be processing it)
    // so move on to the next queue
    return nullptr;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // peek at the first task in the sleeping queue
  pPeekTask = m_pSleepingQueue->pPeekTask();
  
  // check if peeked task's wakeup time is after now
  if (! pPeekTask || ! pPeekTask->bWakeupNow()) {
    
    // - peeked task is to be woken up in the future
    
    m_pSleepingQueue->vDropLock();
    
    return nullptr;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // peeked task can be be woken up
  
  Task* pPopTask;
  
  // pop the task off the sleeping queue
  i8 nPriorItems = m_pSleepingQueue->nTasks();
  m_pSleepingQueue->vAssertItems(nPriorItems);
  pPopTask = m_pSleepingQueue->pPopTask();
  
  RASSERT(pPeekTask == pPopTask);
  
  m_pSleepingQueue->vAssertItems(nPriorItems - 1);
  
  m_pSleepingQueue->vDropLock();
  
  pPopTask->vQueue(QK_NONE);
  pPopTask->vState(TS_DESISTED);

  pPopTask->vWakeupClear();
    
  return pPopTask;
}

/*----------------------------------------------------------------------------*/
/* remove task from biding set                                               */
/*----------------------------------------------------------------------------*/
void Queues::vRemoveBidingSet(Task* pTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES,_FOOTER_);
  
  RASSERT(QK_BIDING_SET == pTask->eQueue());
  
  m_pBidingSet->vGrabLock();
  m_pBidingSet->vRemoveTask(pTask);
  m_pBidingSet->vDropLock();
  
  pTask->vQueue(QK_NONE);
  pTask->vState(TS_DESISTED);
}

/*----------------------------------------------------------------------------*/
/* remove task from executing queue                                           */
/*----------------------------------------------------------------------------*/
void Queues::vRemoveExecutingSet(Task* pTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES,_FOOTER_);
  
  RASSERT(QK_EXECUTING_SET == pTask->eQueue());
  
  m_pExecutingSet->vGrabLock();
  m_pExecutingSet->vRemoveTask(pTask);
  m_pExecutingSet->vDropLock();
  
  pTask->vQueue(QK_NONE);
  pTask->vState(TS_DESISTED);
}

/*----------------------------------------------------------------------------*/
/* remove task from waiting queue                                             */
/*----------------------------------------------------------------------------*/
void Queues::vRemoveWaitingSet(Task* pTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES,_FOOTER_);
  
  // note: a waiting task can ony be removed from the waiting set when it
  //       has been fully ack'd
  RASSERT(0 == pTask->iAcksRmg_assert_only_());
  
  RASSERT(TS_WAITING == pTask->eState());
  RASSERT(QK_WAITING_SET == pTask->eQueue());
  
  m_pWaitingSet->vGrabLock();
  m_pWaitingSet->vRemoveTask(pTask);
  m_pWaitingSet->vDropLock();
  
  pTask->vQueue(QK_NONE);
  pTask->vState(TS_DESISTED);
}


////////////////////////////////////////////////////////////////////////////////
//                          implementation for threads                        //
////////////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add  thread to dedicated thread vector                                     */
/*----------------------------------------------------------------------------*/
void Queues::vAddDedicatedThread(Thread* pThread)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES,_FOOTER_);
  
  RASSERT(HS_IDLE == pThread->eThreadState());
  
  m_pDedicatedThreadVec->vGrabLock();
  m_pDedicatedThreadVec->vAdd(pThread);
  m_pDedicatedThreadVec->vDropLock();
}

/*----------------------------------------------------------------------------*/
/* add thread to general thread vector                                        */
/*----------------------------------------------------------------------------*/
void Queues::vAddGeneralThread(Thread* pThread)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES,_FOOTER_);
  
  RASSERT(HS_IDLE == pThread->eThreadState());
  
  m_pGeneralThreadVec->vGrabLock();
  m_pGeneralThreadVec->vAdd(pThread);
  m_pGeneralThreadVec->vDropLock();
}

/*----------------------------------------------------------------------------*/
/* append thread to idle thread queue                                          */
/*----------------------------------------------------------------------------*/
void Queues::vThreadAppendIdle(Thread* pThread)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES,_FOOTER_);
  
  RASSERT(HS_IDLE == pThread->eThreadState());
  
  m_pIdleThreadQueue->vGrabLock();
  m_pIdleThreadQueue->vAppendThread(pThread);
  m_pIdleThreadQueue->vDropLock();
}

/*----------------------------------------------------------------------------*/
/* pop idle thread                                                            */
/*----------------------------------------------------------------------------*/
Thread* Queues::pThreadPopIdleNL(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES,_FOOTER_);
  
  //m_pIdleThreadQueue->vGrabLock();
  Thread* pThread = m_pIdleThreadQueue->pPopThread();
  //m_pIdleThreadQueue->vDropLock();
  
  RASSERT(!pThread || HS_IDLE == pThread->eThreadState());
  
  return pThread;
}

/*----------------------------------------------------------------------------*/
/* remove thread from dedicated thread vector                                 */
/*----------------------------------------------------------------------------*/
void Queues::vRemoveDedicatedThread(Thread* pThread)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES,_FOOTER_);
  
  m_pDedicatedThreadVec->vGrabLock();
  m_pDedicatedThreadVec->vRemove(pThread);
  m_pDedicatedThreadVec->vDropLock();
}

/*----------------------------------------------------------------------------*/
/* remove thread from general thread vector                                   */
/*----------------------------------------------------------------------------*/
void Queues::vRemoveGeneralThread(Thread* pThread)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES,_FOOTER_);
  
  m_pGeneralThreadVec->vGrabLock();
  m_pGeneralThreadVec->vRemove(pThread);
  m_pGeneralThreadVec->vDropLock();
}


////////////////////////////////////////////////////////////////////////////////
//                                 misc                                       //
////////////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* drop idle queue lock                                                       */
/*----------------------------------------------------------------------------*/
void Queues::vDropThreadIdleQueue(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES,_FOOTER_);
  
  m_pIdleThreadQueue->vDropLock();
}

/*----------------------------------------------------------------------------*/
/* dump the biding set                                                        */
/*----------------------------------------------------------------------------*/
void Queues::vDumpBidingSet(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES,_FOOTER_);
  
  m_pBidingSet->vGrabLock();
  vTrace("------------------------------------\n"_Stz);
  vTrace("Biding Set: %p Tasks %lld\n"_Stz,
         m_pBidingSet,m_pBidingSet->nItems());
  vTrace("------------------------------------\n"_Stz);
  m_pBidingSet->vContentDump();
  m_pBidingSet->vDropLock();
  vTrace("====================================\n"_Stz);
  vTrace("\n"_Stz);
  vTrace("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* dump the executing set                                                     */
/*----------------------------------------------------------------------------*/
void Queues::vDumpExecutingSet(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES,_FOOTER_);
  
  m_pExecutingSet->vGrabLock();
  vTrace("------------------------------------\n"_Stz);
  vTrace("Executing Set: %p Tasks %lld\n"_Stz,
         m_pExecutingSet,m_pExecutingSet->nItems());
  vTrace("------------------------------------\n"_Stz);
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
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES,_FOOTER_);
  
  m_pReadyQueue->vGrabLock();
  vTrace("------------------------------------\n"_Stz);
  vTrace("Ready Queue (normal priority): %p Tasks %lld\n"_Stz,
         m_pReadyQueue,m_pReadyQueue->nItems());
  vTrace("------------------------------------\n"_Stz);
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
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES,_FOOTER_);
  
  m_pReadyQueueLP->vGrabLock();
  vTrace("------------------------------------\n"_Stz);
  vTrace("Ready Queue (low Priority): %p Tasks %lld\n"_Stz,
         m_pReadyQueueLP,m_pReadyQueueLP->nItems());
  vTrace("------------------------------------\n"_Stz);
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
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES,_FOOTER_);

  m_pSleepingQueue->vGrabLock();
  vTrace("------------------------------------\n"_Stz);
  vTrace("Sleeping Queue: %p Tasks %lld\n"_Stz,
         m_pSleepingQueue,m_pSleepingQueue->nTasks());
  vTrace("------------------------------------\n"_Stz);
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
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES,_FOOTER_);
  
  m_pWaitingSet->vGrabLock();
  vTrace("------------------------------------\n"_Stz);
  vTrace("Waiting Set: %p Tasks %lld\n"_Stz,
         m_pWaitingSet,m_pWaitingSet->nItems());
  vTrace("------------------------------------\n"_Stz);
  m_pWaitingSet->vContentDump();
  m_pWaitingSet->vDropLock();
  vTrace("====================================\n"_Stz);
  vTrace("\n"_Stz);
  vTrace("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* grab idle queue lock                                                       */
/*----------------------------------------------------------------------------*/
void Queues::vGrabThreadIdleQueue(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES,_FOOTER_);
  
  m_pIdleThreadQueue->vGrabLock();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get a portrait of the queues                                               */
/*----------------------------------------------------------------------------*/
void Queues::vMonitorPortrait(s48K& sPortrait)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES,_FOOTER_);
  
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
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES,_FOOTER_);
  
  i8 nIdle = 0;
  i8 nRunning = 0;
  i8 nDedicated = 0;
  
  m_pGeneralThreadVec->vGrabLock();
  
  i8 nThreads = m_pGeneralThreadVec->nItems();
  for (i8 i_Thread = 0; i_Thread < nThreads; i_Thread++) {
    
    Thread* pThread = m_pGeneralThreadVec->pGet(i_Thread);
    
    if (HS_IDLE == pThread->eThreadState())
      nIdle++;
    else
      if (HS_ASSIGNED == pThread->eThreadState())
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
/* get no of tasks in the biding set                                          */
/*----------------------------------------------------------------------------*/
i8 Queues::nTasksBiding(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES,_FOOTER_);
  
  return m_pBidingSet->nTasks();
}

/*----------------------------------------------------------------------------*/
/* get no of tasks in the executing set                                       */
/*----------------------------------------------------------------------------*/
i8 Queues::nTasksExecuting(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES,_FOOTER_);
  
  return m_pExecutingSet->nTasks();
}

/*----------------------------------------------------------------------------*/
/* get no of tasks in the ready queue                                         */
/*----------------------------------------------------------------------------*/
i8 Queues::nTasksReady(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES,_FOOTER_);
  
  return m_pReadyQueue->nTasks();
}

/*----------------------------------------------------------------------------*/
/* get no of tasks in the low priority ready queue                            */
/*----------------------------------------------------------------------------*/
i8 Queues::nTasksReadyLP(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES,_FOOTER_);
  
  return m_pReadyQueueLP->nTasks();
}

/*----------------------------------------------------------------------------*/
/* get no of tasks in the sleeping queue                                      */
/*----------------------------------------------------------------------------*/
i8 Queues::nTasksSleeping(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES,_FOOTER_);
  
  return m_pSleepingQueue->nTasks();
}

/*----------------------------------------------------------------------------*/
/* get no of tasks in the waiting set                                         */
/*----------------------------------------------------------------------------*/
i8 Queues::nTasksWaiting(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES,_FOOTER_);
  
  return m_pWaitingSet->nTasks();
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__aFooterFunction(Queues);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
