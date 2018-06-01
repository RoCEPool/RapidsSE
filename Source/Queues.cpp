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
  
  ASSERT(m_pGeneralQueue->bIsLocked());
  m_pGeneralQueue->vAddTask(pTask);
}

/*----------------------------------------------------------------------------*/
/* add task to running queue                                                  */
/*----------------------------------------------------------------------------*/
void Queues::vAddToRunningQueue(Task* pTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);
  
  ASSERT(m_pRunningQueue->bIsLocked());
  m_pRunningQueue->vAddTask(pTask);
}

/*----------------------------------------------------------------------------*/
/* add task to sleeping queue                                                 */
/*----------------------------------------------------------------------------*/
void Queues::vAddToSleepingQueue(Task* pTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);
  
  ASSERT(m_pSleepingQueue->bIsLocked());
  m_pSleepingQueue->vAddTask(pTask);
}

/*----------------------------------------------------------------------------*/
/* add task to suspended queue                                                */
/*----------------------------------------------------------------------------*/
void Queues::vAddToSuspendedQueue(Task* pTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);
  
  ASSERT(m_pSuspendedQueue->bIsLocked());
  m_pSuspendedQueue->vAddTask(pTask);
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
                                                  
  ASSERT(m_pSlotQueues[iSlot]->bIsLocked());
  m_pSlotQueues[iSlot]->vAddTask(pTask);
}

/*----------------------------------------------------------------------------*/
/* pop next task in round robin fashion                                       */
/*----------------------------------------------------------------------------*/
bool Queues::bPopQueuedTask(Task** ppPoppedTask, Thread* pCallingThread)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);
  
  Task* pTask;
  Since xNow;
  
  // set popped task to be null to start with
  *ppPoppedTask = NULL;
  
  // save lastQueue we started with
  i8 iStartRRQueue = pCallingThread->m_iLastRRqueue;
  
  while (true) {
    
    // move to the next queue
    pCallingThread->m_iLastRRqueue++;
    
    // have we round robined around the queues once ?
    if (iStartRRQueue == pCallingThread->m_iLastRRqueue)
      return false;
    
    // have we reached the end of the queues
    if (pCallingThread->m_iLastRRqueue == gpAttachMap->m_pGrid->m_nSlots) {
      
      // start all over
      pCallingThread->m_iLastRRqueue = QR_NONE;
      continue;
    }
    
    //--------------------------------------------------------------------------
    // QR_SLEEPING_QUEUE
    
    if (pCallingThread->m_iLastRRqueue == QR_SLEEPING_QUEUE) {
      
      // grab a write lock on the queue
      if ( ! m_pSleepingQueue->bGrabWrite()) {
        // unable to lock queue
        // another thread must be processing it
        // move on to the next queue
        continue;
      }
      
      // peek at the next task in the queue
      pTask = m_pSleepingQueue->pPeekTask();
      
      // check if task wakeup time is now
      xNow.vSet();
      i8 iMicroseconds = xNow.iGetAsMicroseconds();
      if (pTask->iWakeupTime() == iMicroseconds) {
        
        // pop the task off the queue
        Task* pPoppedTask = m_pSleepingQueue->pPopTask();
        
        // drop lock on queue
        m_pSleepingQueue->vDropWrite();
        
        ASSERT(pPoppedTask == pTask);
        
        // clear wakeup time of the task
        pTask->vWakeupTask();
        
        // return the popped task
        *ppPoppedTask = pTask;
        
        // thread has a task to run
        ASSERT( ! m_pSleepingQueue->bIsLocked());
        return true;
      }
      
      // drop lock on queue
      m_pSleepingQueue->vDropWrite();
      
      // didn't find a task, move to next queue
      ASSERT( ! m_pSleepingQueue->bIsLocked());
      continue;
    
    }
    
    //--------------------------------------------------------------------------
    // QR_GENERAL_QUEUE
    
    if (pCallingThread->m_iLastRRqueue == QR_GENERAL_QUEUE) {
      
      // grab a write lock on the queue
      if (!m_pGeneralQueue->bGrabWrite()) {
        // unable to lock queue
        // another thread must be processing it
        // move on to the next queue
        continue;
      }
      // pop task off the general queue
      pTask = m_pGeneralQueue->pPopTask();
      
      // drop the lock
      m_pGeneralQueue->vDropWrite();
      
      if (pTask) {
        *ppPoppedTask = pTask;
        ASSERT( ! m_pGeneralQueue->bIsLocked());
        return true;
      }
      
      // didn't find a task, move to next queue
      ASSERT( ! m_pGeneralQueue->bIsLocked());
      continue;
    }
    
    //--------------------------------------------------------------------------
    // SLOT QUEUES
    
    // grab a write lock on the queue
    if (!m_pSlotQueues[pCallingThread->m_iLastRRqueue]->bGrabWrite()) {
      // unable to lock queue
      // another thread must be processing it
      // move on to the next queue
      continue;
    }
    
    // pop a task off the partition queues
    pTask = m_pSlotQueues[pCallingThread->m_iLastRRqueue]->pPopTask();
    
    // drop lock on the queue
    m_pSlotQueues[pCallingThread->m_iLastRRqueue]->vDropWrite();
    
    if (pTask) {
      *ppPoppedTask = pTask;
      ASSERT( ! m_pSlotQueues[pCallingThread->m_iLastRRqueue]->bIsLocked());
      return true;
    }
    
    // didn't find a task to run, move to next queue
    ASSERT( ! m_pSlotQueues[pCallingThread->m_iLastRRqueue]->bIsLocked());
    continue;
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
