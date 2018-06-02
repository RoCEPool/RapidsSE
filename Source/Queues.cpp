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
  m_pGeneralQueue  = new TaskQueue(bTaskCompareTaskNo);
  
  for (i8 i_Slot = 0; i_Slot < C_SLOTS_PER_NODE; i_Slot++)
    m_pSlotQueues[i_Slot] = nullptr;

  for (i8 i_Slot = 0; i_Slot < gxArguments.m_nSlots; i_Slot++)
    m_pSlotQueues[i_Slot] = new TaskQueue(bTaskCompareTaskNo);
  
  m_pRunningQueue   = new TaskQueue(bTaskCompareTaskNo);
  m_pSuspendedQueue = new TaskQueue(bTaskCompareTaskNo);
  
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
  
  m_pThreadVector->vGrabLock();
  m_pThreadVector->vAdd(pThread);
  m_pThreadVector->vDropLock();
}

/*----------------------------------------------------------------------------*/
/* add task to general queue                                                  */
/*----------------------------------------------------------------------------*/
void Queues::vAddToGeneralQueue(Task* pTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);
  
  m_pGeneralQueue->vGrabLock();
  m_pGeneralQueue->vAddTask(pTask);
  m_pGeneralQueue->vDropLock();
}

/*----------------------------------------------------------------------------*/
/* add task to running queue                                                  */
/*----------------------------------------------------------------------------*/
void Queues::vAddToRunningQueue(Task* pTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);
  
  m_pRunningQueue->vGrabLock();
  m_pRunningQueue->vAddTask(pTask);
  m_pRunningQueue->vDropLock();
}

/*----------------------------------------------------------------------------*/
/* add task to sleeping queue                                                 */
/*----------------------------------------------------------------------------*/
void Queues::vAddToSleepingQueue(Task* pTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);
  
  m_pSleepingQueue->vGrabLock();
  m_pSleepingQueue->vAddTask(pTask);
  m_pSleepingQueue->vDropLock();
}

/*----------------------------------------------------------------------------*/
/* add task to suspended queue                                                */
/*----------------------------------------------------------------------------*/
void Queues::vAddToSuspendedQueue(Task* pTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);
  
  m_pSuspendedQueue->vGrabLock();
  m_pSuspendedQueue->vAddTask(pTask);
  m_pSuspendedQueue->vDropLock();
}

/*----------------------------------------------------------------------------*/
/* add task to a slot queue                                                   */
/*----------------------------------------------------------------------------*/
void Queues::vAddToSlotQueue(Task* pTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);
  
  // NOTE: Task has a m_iPartition... not being set
  // this needs to set for partition tasks
  
  // get slot no of the task
  i8 iSlot = Partition::iPartitionToSlot(pTask->iPartition());
                                                  
  m_pSlotQueues[iSlot]->vGrabLock();
  m_pSlotQueues[iSlot]->vAddTask(pTask);
  m_pSlotQueues[iSlot]->vDropLock();
}

/*----------------------------------------------------------------------------*/
/* pop next task in round robin fashion                                       */
/*----------------------------------------------------------------------------*/
bool Queues::bPopQueuedTask(Task** ppPoppedTask, Thread* pCallingThread)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);
  
  Task* pPeekedTask;
  Task* pPoppedTask;
  
  Since xNow;
  
  // set popped task to be null to start with
  *ppPoppedTask = NULL;
  
  // save last RR queue we started with
  i8 iStartRRQueue = pCallingThread->m_iLastRRqueue;
  
  // round robin at most once through the queues looking for a task to run
  while (true) {
    
    // move to the next queue
    pCallingThread->m_iLastRRqueue++;
    
    // have we round robined around the queues once ?
    if (iStartRRQueue == pCallingThread->m_iLastRRqueue)
      return false;
    
    // have we reached the end of the queues
    if (pCallingThread->m_iLastRRqueue == gpAttachMap->m_pGrid->m_nSlots) {
      
      // start all over again at the first queue
      pCallingThread->m_iLastRRqueue = QR_NONE;
      continue;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // inspect the sleeping queue
    
    if (QR_SLEEPING_QUEUE == pCallingThread->m_iLastRRqueue) {
      
      // attempt to grab a write lock on the sleeping queue
      if (! m_pSleepingQueue->bGrabLock()) {
        // unable to lock queue (another thread must be processing it)
        // so move on to the next queue
        continue;
      }
      
      // peek at the first task in the sleeping queue
      pPeekedTask = m_pSleepingQueue->pPeekTask();
      
      // check if task wakeup time is now (or before)
      xNow.vSet();
      i8 iTimeNow = xNow.iGetAsMicroseconds();
      
      if (pPeekedTask->iWakeupTime() > iTimeNow) {
        
        // - peeked task is to be woken up in the future
        
        // drop lock on sleeping queue
        m_pSleepingQueue->vDropLock();
        
        // didn't find a task, move to next queue
        ASSERT( ! m_pSleepingQueue->bIsLocked());
        continue;
      }

      // - peeked task can be be woken up
      
      // pop the task off the sleeping queue
      Task* pPoppedTask = m_pSleepingQueue->pPopTask();
      ASSERT(pPoppedTask == pPeekedTask);
      
      // drop lock on sleeping queue
      m_pSleepingQueue->vDropLock();
      
      // clear wakeup time of the task
      pPoppedTask->vWakeupTask();
      
      // return the task popped from the sleeping queue
      *ppPoppedTask = pPoppedTask;
      
      // thread has a task to run
      ASSERT( ! m_pSleepingQueue->bIsLocked());
      return true;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // inspect the general queue
    
    if (QR_GENERAL_QUEUE == pCallingThread->m_iLastRRqueue) {
      
      // attempt to grab a write lock on the general queue
      if (! m_pGeneralQueue->bGrabLock()) {
        // unable to lock queue (another thread must be processing it)
        // so move on to the next queue
        continue;
      }
      
      // pop task off the general queue
      pPoppedTask = m_pGeneralQueue->pPopTask();
      
      // drop the lock
      m_pGeneralQueue->vDropLock();
      
      // is there a task to run?
      if (! pPoppedTask) {
        // didn't find a task, move to next queue
        ASSERT( ! m_pGeneralQueue->bIsLocked());
        continue;
      }
      
      // return the task popped from the general queue
      *ppPoppedTask = pPoppedTask;
      
      // thread has a task to run
      ASSERT( ! m_pGeneralQueue->bIsLocked());
      return true;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // inspect a slot queue
    
    // attempt to grab a write lock on the queue
    if (! m_pSlotQueues[pCallingThread->m_iLastRRqueue]->bGrabLock()) {
      // unable to lock queue (another thread must be processing it)
      // so move on to the next queue
      continue;
    }
    
    // pop a task off the partition queues
    pPoppedTask = m_pSlotQueues[pCallingThread->m_iLastRRqueue]->pPopTask();
    
    // drop lock on the queue
    m_pSlotQueues[pCallingThread->m_iLastRRqueue]->vDropLock();
    
    // is there a task to run?
    if (! pPoppedTask) {
      // didn't find a task to run, move to next queue
      ASSERT( ! m_pSlotQueues[pCallingThread->m_iLastRRqueue]->bIsLocked());
      continue;
    }
    
    // return the task popped from the slot queue
    *ppPoppedTask = pPoppedTask;

    // thread has a task to run
    ASSERT( ! m_pSlotQueues[pCallingThread->m_iLastRRqueue]->bIsLocked());
    return true;
  }
  
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
/* remove task from running queue                                             */
/*----------------------------------------------------------------------------*/
void Queues::vRemoveFromRunningQueue(Task* pTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);
  
  m_pRunningQueue->vGrabLock();
  m_pRunningQueue->vRemoveTask(pTask);
  m_pRunningQueue->vDropLock();
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
