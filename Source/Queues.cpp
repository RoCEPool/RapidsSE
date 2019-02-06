/*============================================================================*/
/* Queues.cpp -- Collection of the Systems' Queues                            */
/*============================================================================*/

#include "Threads.h"

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

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
  vFatal("fatal: Queues class cannot be deleted"_Stz);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Queues::Queues(void)
: Object(CN_QUEUES)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES,_FOOTER_);
  
  m_pThreadVec  = new ThreadVec;
  m_ixQueueOrder   = 0;
  
  m_pSleepingQueue = new TaskQueue(QK_SLEEPING_QUEUE,
                                   C_NON_UNQIUE_KEY,
                                   bTaskCompareWakeupTime);
  
  m_pReadyQueue    = new TaskList(QK_READY_QUEUE);
  
  for (i8 i_Slot = 0; i_Slot < C_SLOTS_PER_NODE; i_Slot++)
    m_pReadySlotQueues[i_Slot] = nullptr;
  for (i8 i_Slot = 0; i_Slot < gpAttachMap->Arg.m_nSlotsPerNode; i_Slot++)
    m_pReadySlotQueues[i_Slot] = new TaskList(QK_READY_SLOT_QUEUE);
  
  for (i8 i_Node = 0; i_Node < C_MAX_NODES; i_Node++)
    m_pReadySendQueues[i_Node] = nullptr;
  for (i8 i_Node = 0; i_Node < gpAttachMap->Arg.m_nNodes; i_Node++)
    m_pReadySendQueues[i_Node] = new TaskList(QK_READY_SEND_QUEUE);
  
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
void Queues::vThreadsReport()
{
  i8 iUser;
  i8 iSystem;
  i8 iUse;
  i8 iSleep;
  
  Queues* pQueues = gpAttachMap->m_pQueues;
  
  pQueues->m_pThreadVec->vGrabLock();

  i8 nThreads = pQueues->m_pThreadVec->nItems();
  for (i8 i_Thread = 0; i_Thread < nThreads; i_Thread++) {
    
    Thread* pThread = pQueues->m_pThreadVec->pGet(i_Thread);
    
    pThread->m_pPthread->bGetCpu(iUser,iSystem,iUse,iSleep);
    
    vTrace("  Thread %2lld %p"
           " base %llx size %lld"
           " user %lld system %lld use %lld sleep %lld"
           " %7s %s\n"_Stz,
           pThread->m_iThreadNo,
           pThread,
           
           pThread->m_aStackBase,
           pThread->m_iStackSize,
           
           iUser,
           iSystem,
           iUse,
           iSleep,
           
           aTD[pThread->m_eThreadState],
           pThread->m_eDedicated ? aCN[pThread->m_eDedicated]->aChar() : ""
           );
  }
  
  pQueues->m_pThreadVec->vDropLock();
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
  
  m_pThreadVec->vGrabLock();
  m_pThreadVec->vAdd(pThread);
  m_pThreadVec->vDropLock();
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
/* add task to a slot queue                                                   */
/*----------------------------------------------------------------------------*/
void Queues::vAddToReadySlotQueue(Task* pTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);
  
  // NOTE: Task has a m_iPartition... not being set
  // this needs to set for partition tasks
  
  // get slot no of the task
  i8 iSlot = pTask->iSlot();
  
  m_pReadySlotQueues[iSlot]->vGrabLock();
  vAddToReadySlotQueue_NL(pTask);
  m_pReadySlotQueues[iSlot]->vDropLock();
}

/*----------------------------------------------------------------------------*/
/* add task to send queue                                                     */
/*----------------------------------------------------------------------------*/
void Queues::vAddToSendQueue(Task* pTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);
  
  // get send node no of the task
  i8 iSendNode = pTask->pSendNode()->iNodeNo();
  
  m_pReadySendQueues[iSendNode]->vGrabLock();
  vAddToSendQueue_NL(pTask);
  m_pReadySendQueues[iSendNode]->vDropLock();
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
  
  RASSERT(TK_EXECUTING == pTask->eState());
  
  pTask->vQueue(QR_EXECUTING_QUEUE);
  pTask->vQueueOrder(++m_ixQueueOrder);
  
  if (pTask->pJob()->m_bMark)
    ActivityMap::vMark(pTask->pJob()->iJobId(),
                       "Add to ExecutingSet: %s %p"_Stz,
                       aCN[pTask->pJob()->eClass()],
                       pTask->pJob());

  ASSERT(m_pExecutingSet->bIsLocked());
  m_pExecutingSet->vAddTask(pTask);
}

/*----------------------------------------------------------------------------*/
/* add task to ready queue                                                    */
/*----------------------------------------------------------------------------*/
void Queues::vAddToReadyQueue_NL(Task* pTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);
  
  RASSERT(TK_READY == pTask->eState());
  RASSERT( ! pTask->bIsPartitioned());
  
  pTask->vQueue(QR_READY_QUEUE);
  pTask->vQueueOrder(++m_ixQueueOrder);
  
  if (pTask->pJob()->m_bMark)
    ActivityMap::vMark(pTask->pJob()->iJobId(),
                       "Add to ReadyQueue: %s %p"_Stz,
                       aCN[pTask->pJob()->eClass()],
                       pTask->pJob());

  ASSERT(m_pReadyQueue->bIsLocked());
  m_pReadyQueue->vAddTask(pTask);
}

/*----------------------------------------------------------------------------*/
/* add task to a slot queue                                                   */
/*----------------------------------------------------------------------------*/
void Queues::vAddToReadySlotQueue_NL(Task* pTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);
  
  // NOTE: Task has a m_iPartition... not being set
  // this needs to set for partition tasks
  
  RASSERT(TK_READY == pTask->eState());
  RASSERT(pTask->bIsPartitioned());
  
  // get slot no of the task
  i8 iSlot = TablePtn::iPartitionToSlot(pTask->iPartition());
  
  pTask->vQueue((eQR)iSlot);
  pTask->vQueueOrder(++m_ixQueueOrder);
  
  if (pTask->pJob()->m_bMark)
    ActivityMap::vMark(pTask->pJob()->iJobId(),
                       "Add to ReadySlot: %s %p"_Stz,
                       aCN[pTask->pJob()->eClass()],
                       pTask->pJob());

  ASSERT(m_pReadySlotQueues[iSlot]->bIsLocked());
  m_pReadySlotQueues[iSlot]->vAddTask(pTask);
}

/*----------------------------------------------------------------------------*/
/* add task to a slot queue                                                   */
/*----------------------------------------------------------------------------*/
void Queues::vAddToSendQueue_NL(Task* pTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);
  
  RASSERT(TK_SEND == pTask->eState());
  RASSERT(pTask->pSendNode()->iNodeNo() != -1);
  
  // get send node from the task
  i8 iSendNode = pTask->pSendNode()->iNodeNo();
  
  pTask->vQueue((eQR)(C_SLOTS_PER_NODE + iSendNode));
  pTask->vQueueOrder(++m_ixQueueOrder);
  
  if (pTask->pJob()->m_bMark)
    ActivityMap::vMark(pTask->pJob()->iJobId(),
                       "Add to SendQueue: %s %p"_Stz,
                       aCN[pTask->pJob()->eClass()],
                       pTask->pJob());

  ASSERT(m_pReadySendQueues[iSendNode]->bIsLocked());
  m_pReadySendQueues[iSendNode]->vAddTask(pTask);
}

/*----------------------------------------------------------------------------*/
/* add task to sleeping queue                                                 */
/*----------------------------------------------------------------------------*/
void Queues::vAddToSleepingQueue_NL(Task* pTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);
  
  RASSERT(TK_SLEEPING == pTask->eState());
  
  pTask->vQueue(QR_SLEEPING_QUEUE);
  
  if (pTask->pJob()->m_bMark)
    ActivityMap::vMark(pTask->pJob()->iJobId(),
                       "Add to SleepingQueue: %s %p"_Stz,
                       aCN[pTask->pJob()->eClass()],
                       pTask->pJob());

  ASSERT(m_pSleepingQueue->bIsLocked());
  
  i8 nPriorItems = m_pSleepingQueue->nTasks();
  m_pSleepingQueue->vAssertItems(nPriorItems);
  m_pSleepingQueue->vAddTask(pTask);
  m_pSleepingQueue->vAssertItems(nPriorItems + 1);
}

/*----------------------------------------------------------------------------*/
/* add task to waiting queue                                                  */
/*----------------------------------------------------------------------------*/
void Queues::vAddToWaitingSet_NL(Task* pTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);
    
  // task must be in the waiting state
  RASSERT(TK_WAITING == pTask->eState());
  
  // task must be locked
  RASSERT(pTask->bIsLocked());

  // task must not be in any queue
  RASSERT(QR_NONE == pTask->eQueue());
  
  // mark task as being in the waiting queue
  pTask->vQueue(QR_WAITING_QUEUE);
  pTask->vQueueOrder(++m_ixQueueOrder);
  
  if (pTask->pJob()->m_bMark)
    ActivityMap::vMark(pTask->pJob()->iJobId(),
                       "Add to WaitingSet: %s %p"_Stz,
                       aCN[pTask->pJob()->eClass()],
                       pTask->pJob());

  // put task into the waiting queue
  ASSERT(m_pWaitingSet->bIsLocked());
  m_pWaitingSet->vAddTask(pTask);
  
  // drop task lock
  pTask->vDropLock();
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
  vTrace("Ready Queue: %p\n"_Stz, m_pReadyQueue);
  vTrace("------------------------------------\n"_Stz);
  m_pReadyQueue->vGrabLock();
  m_pReadyQueue->vContentDump();
  m_pReadyQueue->vDropLock();
  vTrace("====================================\n"_Stz);
  vTrace("\n"_Stz);
  vTrace("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* dump the ready send queues                                                 */
/*----------------------------------------------------------------------------*/
void Queues::vDumpReadySendQueues(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);
  
  for (i8 i_Node = 0; i_Node < gpAttachMap->Arg.m_nNodes; i_Node++ ) {
    vTrace("------------------------------------\n"_Stz);
    vTrace("Ready Send Queue: %p\n"_Stz, m_pReadySendQueues[i_Node]);
    vTrace("------------------------------------\n"_Stz);
    m_pReadySendQueues[i_Node]->vGrabLock();
    m_pReadySendQueues[i_Node]->vContentDump();
    m_pReadySendQueues[i_Node]->vDropLock();
    vTrace("====================================\n"_Stz);
    vTrace("\n"_Stz);
    vTrace("\n"_Stz);
  }
}

/*----------------------------------------------------------------------------*/
/* dump ready slot queues                                                     */
/*----------------------------------------------------------------------------*/
void Queues::vDumpReadySlotQueues(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);
  
  for (i8 i_Slot = 0; i_Slot < gpAttachMap->Arg.m_nSlotsPerNode; i_Slot++ ) {
    vTrace("------------------------------------\n"_Stz);
    vTrace("Ready Slot Queue: %p\n"_Stz, m_pReadySlotQueues[i_Slot]);
    vTrace("------------------------------------\n"_Stz);
    m_pReadySlotQueues[i_Slot]->vGrabLock();
    m_pReadySlotQueues[i_Slot]->vContentDump();
    m_pReadySlotQueues[i_Slot]->vDropLock();
    vTrace("====================================\n"_Stz);
    vTrace("\n"_Stz);
    vTrace("\n"_Stz);
  }
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
  
  m_pThreadVec->vGrabLock();

  i8 nThreads = m_pThreadVec->nItems();
  for (i8 i_Thread = 0; i_Thread < nThreads; i_Thread++) {

    Thread* pThread = m_pThreadVec->pGet(i_Thread);
    
    // skip this thread
    if (pThread == pThisThread)
      continue;

    // attempt to grab this thread as an idle thread
    if (pThread->bGrabIfIdle()) {
      m_pThreadVec->vDropLock();
      return pThread;
    }
  }
  
  m_pThreadVec->vDropLock();
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* is iQueueRR refering to the ready queue ?                                  */
/*----------------------------------------------------------------------------*/
bool Queues::bIsReadyQueue(i8 i_QueueRR)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);
  
  if (i_QueueRR == QR_READY_QUEUE)
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* is iQueueRR refering to a ready send queue ?                               */
/*----------------------------------------------------------------------------*/
bool Queues::bIsReadySendQueue(i8 i_QueueRR)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);

  /*
   
      The send queues are logically after the slot queues, and
      the slot queues are 0 .. nSlotsPerNode - 1

      As an example, let's say nSlotsPerNode = 2, nNodes = 3

      .: an iQueueRR of 2 .. 4 refers to a send queue

      which rebased (by subtracting nSlotsPerNode) is 0 .. 2
   
   */
  
  if (i_QueueRR >= gpAttachMap->Arg.m_nSlotsPerNode &&
      i_QueueRR <  gpAttachMap->Arg.m_nSlotsPerNode + gpAttachMap->Arg.m_nNodes)
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* is iQueueRR refering to a ready slot queue ?                               */
/*----------------------------------------------------------------------------*/
bool Queues::bIsReadySlotQueue(i8 i_QueueRR)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);
  
  if (i_QueueRR >= QR_READY_QUEUE + 1 &&
      i_QueueRR <  gpAttachMap->Arg.m_nSlotsPerNode)
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* is iQueueRR refering to the sleeping queue ?                               */
/*----------------------------------------------------------------------------*/
bool Queues::bIsSleepingQueue(i8 i_QueueRR)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);
  
  if (i_QueueRR == QR_SLEEPING_QUEUE)
    return true;
  
  return false;
}

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
  
  m_pThreadVec->vGrabLock();
  
  i8 nThreads = m_pThreadVec->nItems();
  for (i8 i_Thread = 0; i_Thread < nThreads; i_Thread++) {
    
    Thread* pThread = m_pThreadVec->pGet(i_Thread);
    
    if (TD_IDLE == pThread->eThreadState())
      nIdle++;
    else
      if (TD_RUNNING == pThread->eThreadState())
        nRunning++;

    if (CN_NONE != pThread->m_eDedicated)
      nDedicated++;
  }
  
  m_pThreadVec->vDropLock();
  
  sPortrait.vSprint("Threads: running %lld (dedicated %lld) idle %lld\n"_Stz,
                    nThreads,
                      nDedicated,
                    nIdle);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

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
  
  // start a round robin sequence
  pCallingThread->vRRstart();
  
  Queues* pQueues = gpAttachMap->m_pQueues;
  
  // round robin thru the queues
  while (pCallingThread->bRRnext()) {
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // inspect the queues
    
    if (pQueues->bIsSleepingQueue(pCallingThread->m_iLastQueueRR)) {
      if ( ! (pPoppedTask = pPopSleepingQueue(pCallingThread)) )
        continue;
      *ppPoppedTask = pPoppedTask;
      return true;
    }
    
    if (pQueues->bIsReadyQueue(pCallingThread->m_iLastQueueRR)) {
      if ( ! (pPoppedTask = pPopReadyQueue(pCallingThread)) )
        continue;
      *ppPoppedTask = pPoppedTask;
      return true;
    }
    
    if (pQueues->bIsReadySlotQueue(pCallingThread->m_iLastQueueRR)) {
      if ( ! (pPoppedTask = pPopReadySlotQueue(pCallingThread)) )
        continue;
      *ppPoppedTask = pPoppedTask;
      return true;
    }
    
    if (pQueues->bIsReadySendQueue(pCallingThread->m_iLastQueueRR)) {
      if ( ! (pPoppedTask = pPopReadySendQueue(pCallingThread)) )
        continue;
      *ppPoppedTask = pPoppedTask;
      return true;
    }
    
    NEVER_REACH;
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
  } // while (true)
  
  return false;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* pop task from the ready queue                                              */
/*----------------------------------------------------------------------------*/
Task* Queues::pPopReadyQueue(Thread* pCallingThread)
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
  
  RASSERT( ! pTask->bIsPartitioned());
  
  if (pTask->pJob()->m_bMark)
    pTask->vWriteMark("pPopReadyQueue()"_Stz);
  
  // return the task popped from the ready queue
  return pTask;
}

/*----------------------------------------------------------------------------*/
/* pop task from ready send queue                                             */
/*----------------------------------------------------------------------------*/
Task* Queues::pPopReadySendQueue(Thread* pCallingThread)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);
  
  Task* pTask;
  
  i8 i_Queue = i_ToReadySendQueue(pCallingThread->m_iLastQueueRR);
  
  TaskList* pReadySendQueue = m_pReadySendQueues[i_Queue];
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // lock queue and check for task
  
  // attempt to grab a write lock on the queue
  if (! pReadySendQueue->bGrabLock()) {
    // unable to lock queue (another thread must be processing it)
    // so move on to the next queue
    return nullptr;
  }
  
  // pop a task off the partition queues
  pTask = pReadySendQueue->pPopTask();
  
  // task no longer in any queue/set
  if (pTask)
    pTask->vQueue(QR_NONE);
  
 // drop lock on the queue
  pReadySendQueue->vDropLock();
  
  // is there a task to run?
  if (! pTask)
    return nullptr;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // there is a task to run
  
  RASSERT(pTask->pSendNode() != nullptr);
  
  if (pTask->pJob()->m_bMark)
    pTask->vWriteMark("pPopReadySendQueue()"_Stz);
  
  // return the task popped from the send queue
  return pTask;
}

/*----------------------------------------------------------------------------*/
/* pop task from a ready slot queue                                           */
/*----------------------------------------------------------------------------*/
Task* Queues::pPopReadySlotQueue(Thread* pCallingThread)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);
  
  Task* pTask;
  
  i8 i_Queue = i_ToReadySlotQueue(pCallingThread->m_iLastQueueRR);
  
  TaskList* pReadySlotQueue = m_pReadySlotQueues[i_Queue];
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // lock queue and check for task
  
  // attempt to grab a write lock on the queue
  if (! pReadySlotQueue->bGrabLock()) {
    // unable to lock queue (another thread must be processing it)
    // so move on to the next queue
    return nullptr;
  }
  
  // pop a task off the partition queues
  pTask = pReadySlotQueue->pPopTask();
  
  // task no longer in any queue/set
  if (pTask)
    pTask->vQueue(QR_NONE);

  // drop lock on the queue
  pReadySlotQueue->vDropLock();
  
  // is there a task to run?
  if (! pTask)
    return nullptr;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // there is a task to run
  
  RASSERT(pTask->bIsPartitioned());
  
  if (pTask->pJob()->m_bMark)
    pTask->vWriteMark("pPopReadySlotQueue()"_Stz);
  
  // return the task popped from the slot queue
  return pTask;
}

/*----------------------------------------------------------------------------*/
/* pop task from sleeping queue                                               */
/*----------------------------------------------------------------------------*/
Task* Queues::pPopSleepingQueue(Thread* pCallingThread)
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
  
  if (pTask->pJob()->m_bMark)
    pTask->vWriteMark("pPopSleepingQueue()"_Stz);
  
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
  
  return QR_SLEEPING_QUEUE;
}

/*----------------------------------------------------------------------------*/
/* get the last queue                                                         */
/*----------------------------------------------------------------------------*/
i8 Queues::i_QueueLast(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);
  
  i8 i_Last = gpAttachMap->m_pGrid->m_nSlotsPerNode
               + gpAttachMap->m_pGrid->m_nNodes
               - 1;
  
  return i_Last;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* remove thread from thread vector                                           */
/*----------------------------------------------------------------------------*/
void Queues::vRemoveThread(Thread* pThread)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);
  
  m_pThreadVec->vGrabLock();
  m_pThreadVec->vRemove(pThread);
  m_pThreadVec->vDropLock();
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
  
  if (pTask->pJob() && pTask->pJob()->m_bMark)
    pTask->vWriteMark("vRemoveFromExecutingSet()"_Stz);
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
  
  if (pTask->pJob()->m_bMark)
    pTask->vWriteMark("vRemoveFromWaitingSet()"_Stz);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* schedule a fully acked task                                                */
/*----------------------------------------------------------------------------*/
void Queues::vScheduleAckedTask(Task* pTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);
  
  if (pTask->pJob()->m_bMark)
    pTask->vWriteMark("vScheduleAckedTask()"_Stz);

  pTask->vGrabLock();

  // having got the lock, the task should be TK_WAITING and QR_WAITING_QUEUE

  RASSERT(TK_WAITING == pTask->eState());
  RASSERT(QR_WAITING_QUEUE == pTask->eQueue());
  
  pTask->vDropLock();
  
  vRemoveFromWaitingSet(pTask);

  RASSERT(QR_NONE == pTask->eQueue());
  
  pTask->vState(TK_READY);
  
  if (pTask->bIsNonPartitioned())
    vScheduleReadyTask(pTask);
  else
    vScheduleReadySlotTask(pTask);
}

/*----------------------------------------------------------------------------*/
/* schedule an exited task                                                    */
/*----------------------------------------------------------------------------*/
void Queues::vScheduleExitedTask(Task* pTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);

  RASSERT(TK_EXITED == pTask->eState());
  
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
  
  RASSERT(TK_READY == pTask->eState());
  
  if (pTask->pJob()->m_bMark)
    ActivityMap::vMark(pTask->pJob()->iJobId(),
                       "Schedule ready: %s %p"_Stz,
                       aCN[pTask->pJob()->eClass()],
                       pTask->pJob());

  Thread* pIdleThread = pFindIdleThread();

  if (pIdleThread) {
    pIdleThread->vExecuteTask(pTask);
  }
  else {
    if (pTask->bIsNonPartitioned())
      vAddToReadyQueue(pTask);
    else
      vAddToReadySlotQueue(pTask);
  }
}

/*----------------------------------------------------------------------------*/
/* schedule ready slot task                                                   */
/*----------------------------------------------------------------------------*/
void Queues::vScheduleReadySlotTask(Task* pTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);
  
  RASSERT(TK_READY == pTask->eState());
  
  if (pTask->pJob()->m_bMark)
    ActivityMap::vMark(pTask->pJob()->iJobId(),
                       "Schedule ready slot: %s %p"_Stz,
                       aCN[pTask->pJob()->eClass()],
                       pTask->pJob());

  Thread* pIdleThread = pFindIdleThread();
  
  if (pIdleThread) {
    pIdleThread->vExecuteTask(pTask);
  }
  else
    vAddToReadySlotQueue(pTask);
}

/*----------------------------------------------------------------------------*/
/* schedule a task which wants to send                                        */
/*----------------------------------------------------------------------------*/
void Queues::vScheduleSendTask(Task* pTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);
  
  RASSERT(TK_SEND == pTask->eState());
  
  if (pTask->pJob()->m_bMark)
    ActivityMap::vMark(pTask->pJob()->iJobId(),
                       "Schedule send: %s %p"_Stz,
                       aCN[pTask->pJob()->eClass()],
                       pTask->pJob());

  // add task to send queue
  vAddToSendQueue(pTask);
}

/*----------------------------------------------------------------------------*/
/* schedule sleeping task                                                     */
/*----------------------------------------------------------------------------*/
void Queues::vScheduleSleepingTask(Task* pTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);
  
  RASSERT(TK_SLEEPING == pTask->eState());
  
  if (pTask->pJob()->m_bMark)
    ActivityMap::vMark(pTask->pJob()->iJobId(),
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
  
  pTask->vState(TK_READY);
  
  if (pTask->pJob()->m_bMark) {
    if (C_UNKNOWN == pTask->pJob()->iJobId())
      ActivityMap::vMark(0,
                         "Schedule spawned: %s %p"_Stz,
                         aCN[pTask->pJob()->eClass()],
                         pTask->pJob());
    else
      ActivityMap::vMark(pTask->pJob()->iJobId(),
                         "Schedule spawned: %s %p"_Stz,
                         aCN[pTask->pJob()->eClass()],
                         pTask->pJob());
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
  
  RASSERT(TK_WAITING == pTask->eState());
  
  if (pTask->pJob()->m_bMark) {
    if (C_UNKNOWN == pTask->pJob()->iJobId())
      ActivityMap::vMark(0,
                         "Schedule waiting: %s %p"_Stz,
                         aCN[pTask->pJob()->eClass()],
                         pTask->pJob());
    else
      ActivityMap::vMark(pTask->pJob()->iJobId(),
                         "Schedule waiting: %s %p"_Stz,
                         aCN[pTask->pJob()->eClass()],
                         pTask->pJob());
 }

  vAddToWaitingSet(pTask);
}

/*----------------------------------------------------------------------------*/
/* schedule yielded task                                                      */
/*----------------------------------------------------------------------------*/
void Queues::vScheduleYieldedTask(Task* pTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);
  
  Thread* pCallingThread = Thread::pThisTLSThread();
  
  if (pTask->pJob()->m_bMark) {
    if (C_UNKNOWN == pTask->pJob()->iJobId())
      ActivityMap::vMark(0,
                         "Schedule yielded: %s %p"_Stz,
                         aCN[pTask->pJob()->eClass()],
                         pTask->pJob());
    else
      ActivityMap::vMark(pTask->pJob()->iJobId(),
                         "Schedule yielded: %s %p"_Stz,
                         aCN[pTask->pJob()->eClass()],
                         pTask->pJob());
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // non-partitioned task yield
  
  if (pTask->bIsNonPartitioned()) {
    
    // - A general thread has executed a task which retured TK_YIELDED.
    //   If the ready queue is empty then this general thread can
    //     continue to execute the yield task.
    //   Otherwise we add the task to the ready queue
    
    m_pReadyQueue->vGrabLock();
    i8 iLengthReadyQueue = nTasksReady();
    if (0 == iLengthReadyQueue) {
      m_pReadyQueue->vDropLock();
      pCallingThread->vReExecuteTask(pTask);
    }
    else {
      pTask->vState(TK_READY);
      vAddToReadyQueue_NL(pTask);
      m_pReadyQueue->vDropLock();
    }

    return;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // partitioned task yield
  

  // - A general thread has executed a partition task which retured TK_YIELDED.
  //   If the ready queue and ready slot queue is empty then this general thread can
  //     continue to execute the yield task.
  //   Otherwise we add the task to the ready slot queue

  //for (i8 iSlot = 0; iSlot < gpAttachMap->m_pGrid->iSlots(); iSlot++)
  
  i8 i_Slot = pTask->iSlot();

  // first check if any tasks in ready queue
  i8 iLengthReadyQueue = nTasksReady();
  vTrace("ReadyQ %lld\n"_Stz, i_Slot, iLengthReadyQueue);
  
  if (0 == iLengthReadyQueue) {
    
    // now check if any ready tasks in ready slot queue
    m_pReadySlotQueues[i_Slot]->vGrabLock();
    i8 iLengthReadySlotQueue = nTasksReadySlot(i_Slot);
    
    vTrace("SlotQ[%lld] %lld\n"_Stz, i_Slot, iLengthReadySlotQueue);
    
    if (0 == iLengthReadySlotQueue) {
      m_pReadySlotQueues[i_Slot]->vDropLock();
      pCallingThread->vReExecuteTask(pTask);
      return;
    }
    
    pTask->vState(TK_READY);
    vAddToReadySlotQueue_NL(pTask);
    m_pReadySlotQueues[i_Slot]->vDropLock();
    return;
  
  }
  
  // just add task to ready slot queue
  m_pReadySlotQueues[i_Slot]->vGrabLock();
  pTask->vState(TK_READY);
  vAddToReadySlotQueue_NL(pTask);
  m_pReadySlotQueues[i_Slot]->vDropLock();
  
  return;
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
/* get no of tasks in a ready slot queue                                      */
/*----------------------------------------------------------------------------*/
i8 Queues::nTasksReadySlot(i8 iSlot)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);
  
  return m_pReadySlotQueues[iSlot]->nTasks();
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
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* convert iQueueRR to a ready send queue subscript                           */
/*----------------------------------------------------------------------------*/
i8 Queues::i_ToReadySendQueue(i8 i_QueueRR)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);
  
  /*
   
      The send queues are logically after the slot queues, and
      the slot queues are 0 .. nSlotsPerNode - 1

      As an example, let's say nSlotsPerNode = 2, nNodes = 3

      .: an iQueueRR of 2 .. 4 refers to a send queue

      which rebased (by subtracting nSlotsPerNode) is 0 .. 2
   
   */
  
  i8 i_ReadySendQueue = i_QueueRR - gpAttachMap->Arg.m_nSlotsPerNode;
  
  RASSERT(i_ReadySendQueue >= 0 &&
          i_ReadySendQueue < gpAttachMap->Arg.m_nSlotsPerNode);
  
  return i_ReadySendQueue;
}

/*----------------------------------------------------------------------------*/
/* convert iQueueRR to a ready slot queue subscript                           */
/*----------------------------------------------------------------------------*/
i8 Queues::i_ToReadySlotQueue(i8 i_QueueRR)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);
  
  i8 i_ReadySlotQueue = i_QueueRR;
  
  RASSERT(i_ReadySlotQueue >= 0 &&
          gpAttachMap->Arg.m_nSlotsPerNode);
  
  return i_ReadySlotQueue;
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
