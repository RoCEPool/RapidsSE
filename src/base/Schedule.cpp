/*============================================================================*/
/* Schedule.cpp -- System Schedule (tasks x threads)                          */
/*============================================================================*/

#include "BASE.h"

#include "PsxThd.hpp"
#include "SP_Ivl.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Schedule* BASE::_pQueues(void* aAddr)
{
  Schedule* pSchedule = reinterpret_cast<Schedule*>(aAddr);
  
  RASSERT_TYPE(pSchedule,CN_SCHEDULE);
  
  return pSchedule;
}

/*----------------------------------------------------------------------------*/
/* new a queues from existing memory                                          */
/*----------------------------------------------------------------------------*/
void* Schedule::operator new(size_t)
{
  void* pObject = Heaps::pAllocate(CN_SCHEDULE);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* Schedule are never deleted (for the moment)                                  */
/*----------------------------------------------------------------------------*/
void Schedule::operator delete(void* )
{
  vThrow(FK_FATAL,__func__,__FILE__,__LINE__,
         "fatal: Schedule class cannot be deleted"_Stz);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Schedule::Schedule(void)
: Object(CN_SCHEDULE)
, m_okLock(this)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SCHEDULE,_FOOTER_);
  
  
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
                                   bTaskCompareWakeup);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // task sets
  
  m_pBidingSet     = new TaskList(QK_BIDING_SET);
  m_pExecutingSet  = new TaskList(QK_EXECUTING_SET);
  m_pWaitingSet    = new TaskList(QK_WAITING_SET);
  m_pRetiredSet    = new TaskList(QK_RETIRED_SET);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  _checkInitialised_(CN_SCHEDULE);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_SCHEDULE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Schedule::~Schedule(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SCHEDULE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* threads report                                                             */
/*----------------------------------------------------------------------------*/
void Schedule::vThreadsReport(void)
{
  i8 iUserTime;
  i8 iSystemTime;
  i8 iPercentageUsage;
  i8 iSleepSsec;
  
  Schedule* pSchedule = AM::pSchedule();
  
  pSchedule->m_pGeneralThreadVec->vGrabLock();

  i8 nThreads = pSchedule->m_pGeneralThreadVec->nItems();

  vTrace("====================================\n"_Stz);
  vTrace("General Threads: %lld\n"_Stz,nThreads);
  if (nThreads)
    vTrace("------------------------------------\n"_Stz);

  for (i8 i_Thread = 0; i_Thread < nThreads; i_Thread++) {
    
    Thread* pThread = pSchedule->m_pGeneralThreadVec->pGet(i_Thread);
    
    pThread->pPsxThd()->bGetCpu(iUserTime,iSystemTime,
                                 iPercentageUsage,iSleepSsec);
    
    vTrace("  [%02lld] "
           "%p %2lld %s"
           " base %p size %lldB"
           " user %12lld system %12lld usage %02lld%% sleep %02llds"
           " %11s"
           " assigned %lld\n"_Stz
           
           ,i_Thread
           
           ,pThread
           ,pThread->iThreadNo()
           ,pThread->szThreadNm()

           ,pThread->pPsxThd()->m_aStackBase
           ,pThread->pPsxThd()->m_iStackSize
           
           ,iUserTime
           ,iSystemTime
           ,iPercentageUsage
           ,iSleepSsec
           
           ,aHS[pThread->eThreadState()]
           
           ,pThread->nAssignedTasks()
           );
  }
  
  vTrace("====================================\n"_Stz);
  vTrace("\n"_Stz);

  pSchedule->m_pGeneralThreadVec->vDropLock();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  pSchedule->m_pDedicatedThreadVec->vGrabLock();
  
  nThreads = pSchedule->m_pDedicatedThreadVec->nItems();
    
  vTrace("====================================\n"_Stz);
  vTrace("Dedicated Threads: %lld\n"_Stz,nThreads);
  if (nThreads)
    vTrace("------------------------------------\n"_Stz);

  for (i8 i_Thread = 0; i_Thread < nThreads; i_Thread++) {
    
    Thread* pThread = pSchedule->m_pDedicatedThreadVec->pGet(i_Thread);
    
    pThread->pPsxThd()->bGetCpu(iUserTime,iSystemTime,
                                 iPercentageUsage,iSleepSsec);
    
    vTrace("  [%02lld] "
           "%p %2lld %s"
           " base %p size %lldB"
           " user %12lld system %12lld usage %02lld%% sleep %02llds"
           " %11s"
           " assigned %lld\n"_Stz
           
           ,i_Thread
           
           ,pThread
           ,pThread->iThreadNo()
           ,pThread->szThreadNm()

           ,pThread->pPsxThd()->m_aStackBase
           ,pThread->pPsxThd()->m_iStackSize
           
           ,iUserTime
           ,iSystemTime
           ,iPercentageUsage
           ,iSleepSsec
           
           ,aHS[pThread->eThreadState()]
           
           ,pThread->nAssignedTasks()
           );
  }
  
  vTrace("====================================\n"_Stz);
  vTrace("\n"_Stz);

  pSchedule->m_pDedicatedThreadVec->vDropLock();
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* outline                                                                    */
/*----------------------------------------------------------------------------*/
void Schedule::vOutline(s4K& sOutline)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SCHEDULE,_FOOTER_);
  
  sOutline.vSprint("<%s focal>"_Stz,aCN[eGetClassNo()]);
}


////////////////////////////////////////////////////////////////////////////////
//                          implementation for tasks                          //
////////////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add task to biding set                                                     */
/*----------------------------------------------------------------------------*/
void Schedule::vAddBidingSet(Task* pTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SCHEDULE,_FOOTER_);
  
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
void Schedule::vAddExecutingSet(Task* pTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SCHEDULE,_FOOTER_);
  
  RASSERT(TS_EXECUTING == pTask->eState());
  RASSERT(QK_NONE == pTask->eQueue());

  pTask->vQueue(QK_EXECUTING_SET);
  
  m_pExecutingSet->vGrabLock();
  
  m_pExecutingSet->vAddTask(pTask);
  
  m_pExecutingSet->vDropLock();
  
  // **do not touch task**
}

/*----------------------------------------------------------------------------*/
/* add task to retired set                                                    */
/*----------------------------------------------------------------------------*/
void Schedule::vAddRetiredSet(Task* pTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SCHEDULE,_FOOTER_);
  
  RASSERT(pTask->bIsLocked());

  RASSERT(TS_WAITING == pTask->eState());
  RASSERT(QK_NONE == pTask->eQueue());

  pTask->vQueue(QK_RETIRED_SET);
  
  m_pRetiredSet->vGrabLock();
  
  m_pRetiredSet->vAddTask(pTask);
    
  m_pRetiredSet->vDropLock();
  
  // **do not touch task**
}

/*----------------------------------------------------------------------------*/
/* add task to sleeping queue                                                 */
/*----------------------------------------------------------------------------*/
void Schedule::vAddSleepingQueue(Task* pTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SCHEDULE,_FOOTER_);
  
  RASSERT(TS_SLEEPING == pTask->eState());
  RASSERT(QK_NONE == pTask->eQueue());

  pTask->vQueue(QK_SLEEPING_QUEUE);

  m_pSleepingQueue->vGrabLock();
  
  i8 nPriorItems = m_pSleepingQueue->nItems();
  m_pSleepingQueue->vAssertItems(nPriorItems);
  m_pSleepingQueue->vAddTask(pTask);
  m_pSleepingQueue->vAssertItems(nPriorItems + 1);
  
  m_pSleepingQueue->vDropLock();
  
  // **do not touch task**
}

/*----------------------------------------------------------------------------*/
/* add task to waiting set                                                    */
/*----------------------------------------------------------------------------*/
void Schedule::vAddWaitingSet(Task* pTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SCHEDULE,_FOOTER_);
  
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
void Schedule::vAppendReadyQueue(Task* pTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SCHEDULE,_FOOTER_);
  
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
void Schedule::vAppendReadyQueueLP(Task* pTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SCHEDULE,_FOOTER_);
  
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
Task* Schedule::pPopReadyQueue(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SCHEDULE,_FOOTER_);
  
  Task* pTask;
  
//  if (! m_pReadyQueue->bGrabLock()) {
//    // unable to lock queue (another thread must be processing it)
//    // so move on to the next queue
//    return nullptr;
//  }
  
  m_pReadyQueue->vGrabLock();
  
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
Task* Schedule::pPopReadyQueueLP(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SCHEDULE,_FOOTER_);
  
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
Task* Schedule::pPopSleepingQueue(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SCHEDULE,_FOOTER_);
  
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
  i8 nPriorItems = m_pSleepingQueue->nItems();
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
void Schedule::vRemoveBidingSet(Task* pTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SCHEDULE,_FOOTER_);
  
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
void Schedule::vRemoveExecutingSet(Task* pTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SCHEDULE,_FOOTER_);
  
  RASSERT(QK_EXECUTING_SET == pTask->eQueue());
  
  m_pExecutingSet->vGrabLock();
  m_pExecutingSet->vRemoveTask(pTask);
  m_pExecutingSet->vDropLock();
  
  pTask->vQueue(QK_NONE);
  pTask->vState(TS_DESISTED);
}

/*----------------------------------------------------------------------------*/
/* remove task from retired set                                               */
/*----------------------------------------------------------------------------*/
void Schedule::vRemoveRetiredSet(Task* pTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SCHEDULE,_FOOTER_);
  
  RASSERT(0 == pTask->iAcksRmg_assert_only_());
  
  RASSERT(TS_RETIRED == pTask->eState());
  RASSERT(QK_RETIRED_SET == pTask->eQueue());
  
  m_pRetiredSet->vGrabLock();
  m_pRetiredSet->vRemoveTask(pTask);
  m_pRetiredSet->vDropLock();
  
  pTask->vQueue(QK_NONE);
  pTask->vState(TS_REAPABLE);
}

/*----------------------------------------------------------------------------*/
/* remove task from waiting set                                               */
/*----------------------------------------------------------------------------*/
void Schedule::vRemoveWaitingSet(Task* pTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SCHEDULE,_FOOTER_);
  
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
void Schedule::vAddDedicatedThread(Thread* pThread)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SCHEDULE,_FOOTER_);
  
  RASSERT(HS_IDLE == pThread->eThreadState());
  
  m_pDedicatedThreadVec->vGrabLock();
  m_pDedicatedThreadVec->vAppend(pThread);
  m_pDedicatedThreadVec->vDropLock();
}

/*----------------------------------------------------------------------------*/
/* add thread to general thread vector                                        */
/*----------------------------------------------------------------------------*/
void Schedule::vAddGeneralThread(Thread* pThread)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SCHEDULE,_FOOTER_);
  
  RASSERT(HS_IDLE == pThread->eThreadState());
  
  m_pGeneralThreadVec->vGrabLock();
  m_pGeneralThreadVec->vAppend(pThread);
  m_pGeneralThreadVec->vDropLock();
}

/*----------------------------------------------------------------------------*/
/* append thread to idle thread queue                                          */
/*----------------------------------------------------------------------------*/
void Schedule::vThreadAppendIdle(Thread* pThread)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SCHEDULE,_FOOTER_);
  
  RASSERT(HS_IDLE == pThread->eThreadState());
  
  RASSERT(m_pIdleThreadQueue->bIsLocked());
  
  //m_pIdleThreadQueue->vGrabLock();
  m_pIdleThreadQueue->vAppendThread(pThread);
  //m_pIdleThreadQueue->vDropLock();
}

/*----------------------------------------------------------------------------*/
/* pop idle thread                                                            */
/*----------------------------------------------------------------------------*/
Thread* Schedule::pThreadPopIdleNL(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SCHEDULE,_FOOTER_);
  
  RASSERT(m_pIdleThreadQueue->bIsLocked());

  //m_pIdleThreadQueue->vGrabLock();
  Thread* pThread = m_pIdleThreadQueue->pPopThread();
  //m_pIdleThreadQueue->vDropLock();
  
  RASSERT(!pThread || HS_IDLE == pThread->eThreadState());
  
  return pThread;
}

/*----------------------------------------------------------------------------*/
/* remove thread from dedicated thread vector                                 */
/*----------------------------------------------------------------------------*/
void Schedule::vRemoveDedicatedThread(Thread* pThread)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SCHEDULE,_FOOTER_);
  
  m_pDedicatedThreadVec->vGrabLock();
  m_pDedicatedThreadVec->vRemove(pThread);
  m_pDedicatedThreadVec->vDropLock();
}

/*----------------------------------------------------------------------------*/
/* remove thread from general thread vector                                   */
/*----------------------------------------------------------------------------*/
void Schedule::vRemoveGeneralThread(Thread* pThread)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SCHEDULE,_FOOTER_);
  
  m_pGeneralThreadVec->vGrabLock();
  m_pGeneralThreadVec->vRemove(pThread);
  m_pGeneralThreadVec->vDropLock();
}

/*----------------------------------------------------------------------------*/
/* return dedicated thread vector                                             */
/*----------------------------------------------------------------------------*/
ThreadVec* Schedule::pDedicatedThreadVec(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SCHEDULE,_FOOTER_);
  
  return m_pDedicatedThreadVec;
}

/*----------------------------------------------------------------------------*/
/* return general thread vector                                               */
/*----------------------------------------------------------------------------*/
ThreadVec* Schedule::pGeneralThreadVec(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SCHEDULE,_FOOTER_);
  
  return m_pGeneralThreadVec;
}

/*----------------------------------------------------------------------------*/
/* return number of idle threads                                              */
/*----------------------------------------------------------------------------*/
i8 Schedule::nIdleThreads(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SCHEDULE,_FOOTER_);
  
  i8 nIdle = m_pIdleThreadQueue->nQueuedThreads();
  
  return nIdle;
}


////////////////////////////////////////////////////////////////////////////////
//                                 misc                                       //
////////////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* drop idle queue lock                                                       */
/*----------------------------------------------------------------------------*/
void Schedule::vDropThreadIdleQueue(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SCHEDULE,_FOOTER_);
  
  m_pIdleThreadQueue->vDropLock();
}

/*----------------------------------------------------------------------------*/
/* dump the biding set                                                        */
/*----------------------------------------------------------------------------*/
void Schedule::vDumpBidingSet(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SCHEDULE,_FOOTER_);
  
  m_pBidingSet->vGrabLock();
  
  i8 nTasks = m_pBidingSet->nItems();
  
  vTrace("====================================\n"_Stz);
  vTrace("Biding Set: Tasks %lld\n"_Stz,nTasks);
  
  if (nTasks) {
    vTrace("------------------------------------\n"_Stz);
    m_pBidingSet->vContentDump();
  }
  
  vTrace("====================================\n"_Stz);
  vTrace("\n"_Stz);

  m_pBidingSet->vDropLock();
}

/*----------------------------------------------------------------------------*/
/* dump the executing set                                                     */
/*----------------------------------------------------------------------------*/
void Schedule::vDumpExecutingSet(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SCHEDULE,_FOOTER_);
  
  m_pExecutingSet->vGrabLock();

  i8 nTasks = m_pExecutingSet->nItems();
  
  vTrace("====================================\n"_Stz);
  vTrace("Executing Set: Tasks %lld\n"_Stz,nTasks);
  
  if (nTasks) {
    vTrace("------------------------------------\n"_Stz);
    m_pExecutingSet->vContentDump();
  }
  
  vTrace("====================================\n"_Stz);
  vTrace("\n"_Stz);
  
  m_pExecutingSet->vDropLock();
}

/*----------------------------------------------------------------------------*/
/* dump the ready queue                                                       */
/*----------------------------------------------------------------------------*/
void Schedule::vDumpReadyQueue(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SCHEDULE,_FOOTER_);
  
  m_pReadyQueue->vGrabLock();
  
  i8 nTasks = m_pReadyQueue->nItems();

  vTrace("====================================\n"_Stz);
  vTrace("Ready Queue: Tasks %lld\n"_Stz,nTasks);
  
  if (nTasks) {
    vTrace("------------------------------------\n"_Stz);
    m_pReadyQueue->vContentDump();
  }
  
  vTrace("====================================\n"_Stz);
  vTrace("\n"_Stz);

  m_pReadyQueue->vDropLock();
}

/*----------------------------------------------------------------------------*/
/* dump the low priority ready queue                                          */
/*----------------------------------------------------------------------------*/
void Schedule::vDumpReadyQueueLP(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SCHEDULE,_FOOTER_);
  
  m_pReadyQueueLP->vGrabLock();
  
  i8 nTasks = m_pReadyQueueLP->nItems();

  vTrace("====================================\n"_Stz);
  vTrace("Ready Queue (low Priority): Tasks %lld\n"_Stz,nTasks);
  
  if (nTasks) {
    vTrace("------------------------------------\n"_Stz);
    m_pReadyQueueLP->vContentDump();
  }

  vTrace("====================================\n"_Stz);
  vTrace("\n"_Stz);
  
  m_pReadyQueueLP->vDropLock();
}

/*----------------------------------------------------------------------------*/
/* dump sleeping queue                                                        */
/*----------------------------------------------------------------------------*/
void Schedule::vDumpSleepingQueue(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SCHEDULE,_FOOTER_);

  SP_Ivl spIvl;
  
  m_pSleepingQueue->vGrabLock();
  
  i8 nTasks = m_pSleepingQueue->nItems();

  Ivl wCurIvl = IvlLib::wGetCurrIvl();
  
  vTrace("====================================\n"_Stz);
  vTrace("Sleeping Queue: Tasks %lld ; CurIvl %s\n"_Stz
         ,nTasks
         ,spIvl.sz(wCurIvl.T.m_iAsI8)
         );
  
  if (nTasks) {
    vTrace("------------------------------------\n"_Stz);
    m_pSleepingQueue->vContentDump();
  }

  vTrace("====================================\n"_Stz);
  vTrace("\n"_Stz);

  m_pSleepingQueue->vDropLock();
}

/*----------------------------------------------------------------------------*/
/* dump waiting set                                                           */
/*----------------------------------------------------------------------------*/
void Schedule::vDumpWaitingSet(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SCHEDULE,_FOOTER_);
  
  m_pWaitingSet->vGrabLock();
  
  i8 nTasks = m_pWaitingSet->nItems();

  vTrace("====================================\n"_Stz);
  vTrace("Waiting Set: Tasks %lld\n"_Stz,nTasks);
  
  if (nTasks) {
    vTrace("------------------------------------\n"_Stz);
    m_pWaitingSet->vContentDump();
  }

  vTrace("====================================\n"_Stz);
  vTrace("\n"_Stz);

  m_pWaitingSet->vDropLock();
}

/*----------------------------------------------------------------------------*/
/* grab idle queue lock                                                       */
/*----------------------------------------------------------------------------*/
void Schedule::vGrabThreadIdleQueue(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SCHEDULE,_FOOTER_);
  
  m_pIdleThreadQueue->vGrabLock();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get no of tasks in the biding set                                          */
/*----------------------------------------------------------------------------*/
i8 Schedule::nTasksBiding(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SCHEDULE,_FOOTER_);
  
  return m_pBidingSet->nTasks();
}

/*----------------------------------------------------------------------------*/
/* get no of tasks in the executing set                                       */
/*----------------------------------------------------------------------------*/
i8 Schedule::nTasksExecuting(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SCHEDULE,_FOOTER_);
  
  return m_pExecutingSet->nTasks();
}

/*----------------------------------------------------------------------------*/
/* get no of tasks in the ready queue                                         */
/*----------------------------------------------------------------------------*/
i8 Schedule::nTasksReady(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SCHEDULE,_FOOTER_);
  
  return m_pReadyQueue->nTasks();
}

/*----------------------------------------------------------------------------*/
/* get no of tasks in the low priority ready queue                            */
/*----------------------------------------------------------------------------*/
i8 Schedule::nTasksReadyLP(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SCHEDULE,_FOOTER_);
  
  return m_pReadyQueueLP->nTasks();
}

/*----------------------------------------------------------------------------*/
/* get no of tasks in the sleeping queue                                      */
/*----------------------------------------------------------------------------*/
i8 Schedule::nTasksSleeping(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SCHEDULE,_FOOTER_);
  
  return m_pSleepingQueue->nItems();
}

/*----------------------------------------------------------------------------*/
/* get no of tasks in the waiting set                                         */
/*----------------------------------------------------------------------------*/
i8 Schedule::nTasksWaiting(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SCHEDULE,_FOOTER_);
  
  return m_pWaitingSet->nTasks();
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__aFooterFunction(Schedule);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
