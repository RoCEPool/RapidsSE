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
  
  m_pThreadVector  = new ThreadVector;
  m_ziQueueOrder   = 0;
  
  m_pSleepingQueue = new TaskQueue(bTaskCompareWakeupTime);
  
  m_pReadyQueue    = new TaskList;
  
  for (i8 i_Slot = 0; i_Slot < C_SLOTS_PER_NODE; i_Slot++)
    m_pReadySlotQueues[i_Slot] = nullptr;
  for (i8 i_Slot = 0; i_Slot < gxArguments.m_nSlots; i_Slot++)
    m_pReadySlotQueues[i_Slot] = new TaskList;
  
  for (i8 i_Node = 0; i_Node < C_MAX_NODES; i_Node++)
    m_pReadySendQueues[i_Node] = nullptr;
  for (i8 i_Node = 0; i_Node < gxArguments.m_nNodes; i_Node++)
    m_pReadySendQueues[i_Node] = new TaskList;
  
  m_pExecutingSet  = new TaskList;

  m_pWaitingSet    = new TaskList;

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
  Queues* pQueues = gpAttachMap->m_pQueues;
  
  pQueues->m_pThreadVector->vGrabLock();

  i8 nThreads = pQueues->m_pThreadVector->nItems();
  for (i8 i_Thread = 0; i_Thread < nThreads; i_Thread++) {
    
    Thread* pThread = pQueues->m_pThreadVector->pGet(i_Thread);
    
    vTrace("  Thread %2lld %p base %llx size %lld %7s os id %lld posix id %llx rr : last %2lld start %2lld %s\n"_Stz,
           pThread->m_iThreadNo,
           pThread,
           
           pThread->m_aStackBase,
           pThread->m_iStackSize,
           
           aTC[pThread->m_eThreadState],
           pThread->m_pPosixThread->m_iOpSysThreadId,
           pThread->m_pPosixThread->m_iPosixThreadId,
           pThread->m_iLastRRqueue,
           pThread->m_iStartRRqueue,
           
           pThread->m_eDedicated ? aCN[pThread->m_eDedicated]->aChar() : ""
           );
  }
  
  pQueues->m_pThreadVector->vDropLock();
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
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add task to executing queue                                                */
/*----------------------------------------------------------------------------*/
void Queues::vAddToExecutingQueue(Task* pTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);
  
  m_pExecutingSet->vGrabLock();
  vAddToExecutingQueue_NL(pTask);
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
void Queues::vAddToWaitingQueue(Task* pTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);
  
  m_pWaitingSet->vGrabLock();
  vAddToWaitingQueue_NL(pTask);
  m_pWaitingSet->vDropLock();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add task to executing queue                                                */
/*----------------------------------------------------------------------------*/
void Queues::vAddToExecutingQueue_NL(Task* pTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);
  
  RASSERT(TS_EXECUTING == pTask->eState());
  
  pTask->vQueue(QR_EXECUTING_QUEUE);
  pTask->vQueueOrder(++m_ziQueueOrder);
  
  m_pExecutingSet->vAddTask(pTask);
  
  if (pTask->pJob()->m_bMark) {
    //mark
  }
}

/*----------------------------------------------------------------------------*/
/* add task to ready queue                                                    */
/*----------------------------------------------------------------------------*/
void Queues::vAddToReadyQueue_NL(Task* pTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);
  
  RASSERT(TS_READY == pTask->eState());
  RASSERT( ! pTask->bIsPartitioned());
  
  pTask->vQueue(QR_READY_QUEUE);
  pTask->vQueueOrder(++m_ziQueueOrder);
  m_pReadyQueue->vAddTask(pTask);
  
  if (pTask->pJob()->m_bMark) {
    //mark
  }
}

/*----------------------------------------------------------------------------*/
/* add task to a slot queue                                                   */
/*----------------------------------------------------------------------------*/
void Queues::vAddToReadySlotQueue_NL(Task* pTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);
  
  // NOTE: Task has a m_iPartition... not being set
  // this needs to set for partition tasks
  
  RASSERT(TS_READY == pTask->eState());
  RASSERT(pTask->bIsPartitioned());
  
  // get slot no of the task
  i8 iSlot = TablePtn::iPartitionToSlot(pTask->iPartition());
  
  pTask->vQueue((eQR)iSlot);
  pTask->vQueueOrder(++m_ziQueueOrder);
  
  m_pReadySlotQueues[iSlot]->vAddTask(pTask);
  
  if (pTask->pJob()->m_bMark) {
    //mark
  }
}

/*----------------------------------------------------------------------------*/
/* add task to a slot queue                                                   */
/*----------------------------------------------------------------------------*/
void Queues::vAddToSendQueue_NL(Task* pTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);
  
  RASSERT(TS_SEND == pTask->eState());
  RASSERT(pTask->pSendNode()->iNodeNo() != -1);
  
  // get send node from the task
  i8 iSendNode = pTask->pSendNode()->iNodeNo();
  
  pTask->vQueue((eQR)(C_SLOTS_PER_NODE + iSendNode));
  pTask->vQueueOrder(++m_ziQueueOrder);
  
  m_pReadySendQueues[iSendNode]->vAddTask(pTask);
  
  if (pTask->pJob()->m_bMark) {
    //mark
  }
}

/*----------------------------------------------------------------------------*/
/* add task to sleeping queue                                                 */
/*----------------------------------------------------------------------------*/
void Queues::vAddToSleepingQueue_NL(Task* pTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);
  
  RASSERT(TS_SLEEPING == pTask->eState());
  
  pTask->vQueue(QR_SLEEPING_QUEUE);
  
  m_pSleepingQueue->vAddTask(pTask);
  
  if (pTask->pJob()->m_bMark) {
    //mark
  }
}

/*----------------------------------------------------------------------------*/
/* add task to waiting queue                                                  */
/*----------------------------------------------------------------------------*/
void Queues::vAddToWaitingQueue_NL(Task* pTask)
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
  pTask->vQueueOrder(++m_ziQueueOrder);
  
  // put task inyo the waiting queue
  m_pWaitingSet->vAddTask(pTask);
  
  // drop task lock
  pTask->vDropLock();
  
  if (pTask->pJob()->m_bMark) {
    //mark
  }
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump the executing queue                                                   */
/*----------------------------------------------------------------------------*/
void Queues::vDumpExecutingQueue(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);
  
  vTrace("------------------------------------\n"_Stz);
  vTrace("Executing Queue: %p\n"_Stz, m_pExecutingSet);
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
  
  for (i8 i_Node = 0; i_Node < gxArguments.m_nNodes; i_Node++ ) {
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
  
  for (i8 i_Slot = 0; i_Slot < gxArguments.m_nSlots; i_Slot++ ) {
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
  vTrace("SleepingQueue: %p\n"_Stz, m_pSleepingQueue);
  vTrace("------------------------------------\n"_Stz);
  m_pSleepingQueue->vGrabLock();
  m_pSleepingQueue->vContentDump();
  m_pSleepingQueue->vDropLock();
  vTrace("====================================\n"_Stz);
  vTrace("\n"_Stz);
  vTrace("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* dump waiting queue                                                         */
/*----------------------------------------------------------------------------*/
void Queues::vDumpWaitingQueue(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);
  
  vTrace("------------------------------------\n"_Stz);
  vTrace("Waiting Queue: %p\n"_Stz, m_pWaitingSet);
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
Thread* Queues::pGrabIdleThread(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);
  
  Thread* pThisThread = nullptr;
  
  if ( ! PosixThread::bIsJavaThread() )
    pThisThread = Thread::pThisTLSThread();
  
  m_pThreadVector->vGrabLock();

  i8 nThreads = m_pThreadVector->nItems();
  for (i8 i_Thread = 0; i_Thread < nThreads; i_Thread++) {

    Thread* pThread = m_pThreadVector->pGet(i_Thread);
    
    // skip this thread
    if (pThread == pThisThread)
      continue;
    
    // attempt to grab threads mutex
    if (! pThread->boLockMutexLock())
      continue;
    
    ASSERT(pThread->bMutexIsLocked());
    
    if (pThread->bIsIdle()) {
      pThread->m_eThreadState = TC_RUNNING;
      m_pThreadVector->vDropLock();
      return pThread;
    }
    
    pThread->vMutexUnlock();
  }
  
  m_pThreadVector->vDropLock();
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

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
  
  // task no longer in queue
  pPoppedTask->vQueue(QR_NONE);
  
  RASSERT( ! pPoppedTask->bIsPartitioned());
  
  if (pPoppedTask->pJob()->m_bMark) {
    //mark
  }
  
  // return the task popped from the ready queue
  return pPoppedTask;
}

/*----------------------------------------------------------------------------*/
/* inspect a ready send queue                                                 */
/*----------------------------------------------------------------------------*/
Task* Queues::pInspectReadySendQueue(Thread* pCallingThread)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);
  
  Task* pPoppedTask;
  
  i8 i_Queue = i_ToReadySendQueue(pCallingThread->m_iLastRRqueue);
  
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
  pPoppedTask = pReadySendQueue->pPopTask();
  
  // drop lock on the queue
  pReadySendQueue->vDropLock();
  
  // is there a task to run?
  if (! pPoppedTask)
  return nullptr;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // there is a task to run
  
  // task no longer in queue
  pPoppedTask->vQueue(QR_NONE);
  
  RASSERT(pPoppedTask->pSendNode() != nullptr);
  
  if (pPoppedTask->pJob()->m_bMark) {
    //mark
  }
  
  // return the task popped from the slot queue
  return pPoppedTask;
}

/*----------------------------------------------------------------------------*/
/* inspect a ready slot queue                                                 */
/*----------------------------------------------------------------------------*/
Task* Queues::pInspectReadySlotQueue(Thread* pCallingThread)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);
  
  Task* pPoppedTask;
  
  i8 i_Queue = i_ToReadySlotQueue(pCallingThread->m_iLastRRqueue);
  
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
  pPoppedTask = pReadySlotQueue->pPopTask();
  
  // drop lock on the queue
  pReadySlotQueue->vDropLock();
  
  // is there a task to run?
  if (! pPoppedTask)
    return nullptr;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // there is a task to run
  
  // task no longer in queue
  pPoppedTask->vQueue(QR_NONE);
  
  RASSERT(pPoppedTask->bIsPartitioned());
  
  if (pPoppedTask->pJob()->m_bMark) {
    //mark
  }
  
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
  
  // task no longer in queue
  pPoppedTask->vQueue(QR_NONE);
  
  // clear wakeup time of the task
  pPoppedTask->vWakeupTask();
  
  if (pPoppedTask->pJob()->m_bMark) {
    //mark
  }
  
  // return the task popped from the sleeping queue
  return pPoppedTask;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* is iRRqueue refering to the ready queue ?                                  */
/*----------------------------------------------------------------------------*/
bool Queues::bIsReadyQueue(i8 iRRqueue)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);
  
  if (iRRqueue == QR_READY_QUEUE)
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* is iRRqueue refering to a ready send queue ?                               */
/*----------------------------------------------------------------------------*/
bool Queues::bIsReadySendQueue(i8 iRRqueue)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);

  /*
   
      The send queues are logically after the slot queues, and
      the slot queues are 0 .. nSlots - 1

      As an example, let's say nSlots = 2, nNodes = 3

      .: an iRRqueue of 2 .. 4 refers to a send queue

      which rebased (by subtracting nSlots) is 0 .. 2
   
   */
  
  if (iRRqueue >= gxArguments.m_nSlots &&
      iRRqueue <  gxArguments.m_nSlots + gxArguments.m_nNodes)
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* is iRRqueue refering to a ready slot queue ?                               */
/*----------------------------------------------------------------------------*/
bool Queues::bIsReadySlotQueue(i8 iRRqueue)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);
  
  if (iRRqueue >= QR_READY_QUEUE + 1 &&
      iRRqueue <  gxArguments.m_nSlots)
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* is iRRqueue refering to the sleeping queue ?                               */
/*----------------------------------------------------------------------------*/
bool Queues::bIsSleepingQueue(i8 iRRqueue)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);
  
  if (iRRqueue == QR_SLEEPING_QUEUE)
    return true;
  
  return false;
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
  
  // start a round robin sequence
  pCallingThread->vRRstart();
  
  Queues* pQueues = gpAttachMap->m_pQueues;
  
  // round robin thru the queues
  while (pCallingThread->bRRnext()) {
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // inspect the queues
    
    if (pQueues->bIsSleepingQueue(pCallingThread->m_iLastRRqueue)) {
      if ( ! (pPoppedTask = pInspectSleepingQueue(pCallingThread)) )
        continue;
      *ppPoppedTask = pPoppedTask;
      return true;
    }
    
    if (pQueues->bIsReadyQueue(pCallingThread->m_iLastRRqueue)) {
      if ( ! (pPoppedTask = pInspectReadyQueue(pCallingThread)) )
        continue;
      *ppPoppedTask = pPoppedTask;
      return true;
    }
    
    if (pQueues->bIsReadySlotQueue(pCallingThread->m_iLastRRqueue)) {
      if ( ! (pPoppedTask = pInspectReadySlotQueue(pCallingThread)) )
        continue;
      *ppPoppedTask = pPoppedTask;
      return true;
    }
    
    if (pQueues->bIsReadySendQueue(pCallingThread->m_iLastRRqueue)) {
      if ( ! (pPoppedTask = pInspectReadySendQueue(pCallingThread)) )
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
/* get no of tasks in the executing queue                                     */
/*----------------------------------------------------------------------------*/
i8 Queues::nQueuedExecutingTasks(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);
  
  return m_pExecutingSet->nTasks();
}

/*----------------------------------------------------------------------------*/
/* get no of tasks in the ready queue                                         */
/*----------------------------------------------------------------------------*/
i8 Queues::nQueuedReadyTasks(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);
  
  return m_pReadyQueue->nTasks();
}

/*----------------------------------------------------------------------------*/
/* get no of tasks in the ready slot queue                                    */
/*----------------------------------------------------------------------------*/
i8 Queues::nQueuedReadySlotTasks(i8 iSlot)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);
  
  return m_pReadySlotQueues[iSlot]->nTasks();
}

/*----------------------------------------------------------------------------*/
/* get no of tasks in the sleeping queue                                      */
/*----------------------------------------------------------------------------*/
i8 Queues::nQueuedSleepingTasks(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);
  
  return m_pSleepingQueue->nTasks();
}

/*----------------------------------------------------------------------------*/
/* get no of tasks in the waiting queue                                       */
/*----------------------------------------------------------------------------*/
i8 Queues::nQueuedWaitingTasks(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);
  
  return m_pWaitingSet->nTasks();
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
  
  i8 i_Last = gpAttachMap->m_pGrid->m_nSlots
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
  
  m_pExecutingSet->vGrabLock();
  m_pExecutingSet->vRemoveTask(pTask);
  m_pExecutingSet->vDropLock();
  
  pTask->vQueue(QR_NONE);
}

/*----------------------------------------------------------------------------*/
/* remove task from waiting queue                                             */
/*----------------------------------------------------------------------------*/
void Queues::vRemoveFromWaitingQueue(Task* pTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);
  
  m_pWaitingSet->vGrabLock();
  m_pWaitingSet->vRemoveTask(pTask);
  m_pWaitingSet->vDropLock();
  
  pTask->vQueue(QR_NONE);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* schedule a fully acked task                                                */
/*----------------------------------------------------------------------------*/
void Queues::vScheduleAckedTask(Task* pTaskToBeAcked)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);
  
  pTaskToBeAcked->vGrabLock();

  // having got the lock, the task should be TS_WAITING and QR_WAITING_QUEUE

  RASSERT(TS_WAITING == pTaskToBeAcked->eState());
  RASSERT(QR_WAITING_QUEUE == pTaskToBeAcked->eQueue());
  
  pTaskToBeAcked->vDropLock();
  
  vRemoveFromWaitingQueue(pTaskToBeAcked);

  RASSERT(QR_NONE == pTaskToBeAcked->eQueue());
  
  pTaskToBeAcked->vState(TS_READY);
  
  if (pTaskToBeAcked->bIsNonPartitioned())
    vScheduleReadyTask(pTaskToBeAcked);
  else
    vScheduleReadySlotTask(pTaskToBeAcked);
}

/*----------------------------------------------------------------------------*/
/* schedule an exited task                                                    */
/*----------------------------------------------------------------------------*/
void Queues::vScheduleExitedTask(Task* pTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);

  RASSERT(TS_EXITED == pTask->eState());
  
  ASSERT( ! pTask->bWordBody() );
  
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
  
  Thread* pIdleThread = pGrabIdleThread();

  if (pIdleThread) {
    ASSERT(pIdleThread->bMutexIsLocked());
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
  
  RASSERT(TS_READY == pTask->eState());
  
  Thread* pIdleThread = pGrabIdleThread();
  
  if (pIdleThread) {
    ASSERT(pIdleThread->bMutexIsLocked());
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
  
  RASSERT(TS_SEND == pTask->eState());
  
  // add task to send queue
  vAddToSendQueue(pTask);
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
  
  Thread* pCallingThread = Thread::pThisTLSThread();
  
  if (pTask->pJob()->m_bMark) {
    //mark
  }
  
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
  
  eQR eQueue = pTask->eWhichQueue();
  RASSERT(eQueue >= QR_READY_QUEUE  || eQueue < gpAttachMap->m_pGrid->m_nSlots);
  
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
  
  vAddToWaitingQueue(pTask);
}

/*----------------------------------------------------------------------------*/
/* schedule yielded task                                                      */
/*----------------------------------------------------------------------------*/
void Queues::vScheduleYieldedTask(Task* pTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);
  
  Thread* pCallingThread = Thread::pThisTLSThread();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // non-partitioned task yield
  
  if (pTask->bIsNonPartitioned()) {
    
    // - A general thread has executed a task which retured TS_YIELDED.
    //   If the ready queue is empty then this general thread can
    //     continue to execute the yield task.
    //   Otherwise we add the task to the ready queue
    
    m_pReadyQueue->vGrabLock();
    i8 iLengthReadyQueue = m_pReadyQueue->nTasks();
    if (0 == iLengthReadyQueue) {
      m_pReadyQueue->vDropLock();
      pCallingThread->vReExecuteTask(pTask);
    }
    else {
      pTask->vState(TS_READY);
      vAddToReadyQueue_NL(pTask);
      m_pReadyQueue->vDropLock();
    }

    return;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // partitioned task yield
  

  // - A general thread has executed a partition task which retured TS_YIELDED.
  //   If the ready queue and ready slot queue is empty then this general thread can
  //     continue to execute the yield task.
  //   Otherwise we add the task to the ready slot queue

  //for (i8 iSlot = 0; iSlot < gpAttachMap->m_pGrid->iSlots(); iSlot++)
  
  i8 i_Slot = pTask->iSlot();

  // first check if any tasks in ready queue
  i8 iLengthReadyQueue = m_pReadyQueue->nTasks();
  vTrace("ReadyQ %lld\n"_Stz, i_Slot, iLengthReadyQueue);
  
  if (0 == iLengthReadyQueue) {
    
    // now check if any ready tasks in ready slot queue
    m_pReadySlotQueues[i_Slot]->vGrabLock();
    i8 iLengthReadySlotQueue = m_pReadySlotQueues[i_Slot]->nTasks();
    
    vTrace("SlotQ[%lld] %lld\n"_Stz, i_Slot, iLengthReadySlotQueue);
    
    if (0 == iLengthReadySlotQueue) {
      m_pReadySlotQueues[i_Slot]->vDropLock();
      pCallingThread->vReExecuteTask(pTask);
      return;
    }
    
    pTask->vState(TS_READY);
    vAddToReadySlotQueue_NL(pTask);
    m_pReadySlotQueues[i_Slot]->vDropLock();
    return;
  
  }
  
  // just add task to ready slot queue
  m_pReadySlotQueues[i_Slot]->vGrabLock();
  pTask->vState(TS_READY);
  vAddToReadySlotQueue_NL(pTask);
  m_pReadySlotQueues[i_Slot]->vDropLock();
  

  return;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get the thread vector                                                      */
/*----------------------------------------------------------------------------*/
ThreadVector* Queues::pThreadVector(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);
  
  return m_pThreadVector;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* convert iRRqueue to a ready send queue subscript                           */
/*----------------------------------------------------------------------------*/
i8 Queues::i_ToReadySendQueue(i8 i_RRqueue)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);
  
  /*
   
      The send queues are logically after the slot queues, and
      the slot queues are 0 .. nSlots - 1

      As an example, let's say nSlots = 2, nNodes = 3

      .: an iRRqueue of 2 .. 4 refers to a send queue

      which rebased (by subtracting nSlots) is 0 .. 2
   
   */
  
  i8 i_ReadySendQueue = i_RRqueue - gxArguments.m_nSlots;
  
  RASSERT(i_ReadySendQueue >= 0 and i_ReadySendQueue < gxArguments.m_nNodes);
  
  return i_ReadySendQueue;
}

/*----------------------------------------------------------------------------*/
/* convert iRRqueue to a ready slot queue subscript                           */
/*----------------------------------------------------------------------------*/
i8 Queues::i_ToReadySlotQueue(i8 i_RRqueue)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);
  
  i8 i_ReadySlotQueue = i_RRqueue;
  
  RASSERT(i_ReadySlotQueue >= 0 and i_ReadySlotQueue < gxArguments.m_nSlots);
  
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
