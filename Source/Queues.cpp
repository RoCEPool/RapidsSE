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
  
  m_pBlockedQueue   = new TaskQueue(bTaskCompareTaskNo);
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
/* pop next task in round robin fashion                                       */
/*----------------------------------------------------------------------------*/
bool Queues::bPopQueuedTask(Task** ppPoppedTask, Thread* pCallingThread)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUES);
  
  Task* pTask;
  
  while (true) {
    
    // move to the next queue
    pCallingThread->m_iLastRRqueue++;
    
    // have we reached the end of the queues
    if (pCallingThread->m_iLastRRqueue == gpAttachMap->m_pGrid->m_nSlots) {
      
      // start all over
      pCallingThread->m_iLastRRqueue = QR_NONE;
      continue;
    }
    
    //--------------------------------------------------------------------------
    // QR_SLEEPING_QUEUE
    
    if (pCallingThread->m_iLastRRqueue == QR_SLEEPING_QUEUE) {
      
      // peek at the next task in the queue
      pTask = m_pSleepingQueue->pPeekTask();
      
      // check if task wakeup time is now
      Since xNow;
      xNow.vSet();
      i8 iMicrosections = xNow.iMicroseconds() + xNow.iSeconds()*1000;
      if (pTask->iWakeupTime() == iMicrosections) {
        
        // clear wakeup time of the task
        pTask->vWakeupTask();
        
        // pop the task off the queue
        Task* pPoppedTask = m_pSleepingQueue->pPopTask();
        ASSERT(pPoppedTask == pTask);
        
        // return the popped task
        *ppPoppedTask = pTask;
        
        // thread has a task to run
        return true;
      }
      
      // didn't find a task, move to next queue
      continue;
    
    }
    
    //--------------------------------------------------------------------------
    // QR_GENERAL_QUEUE
    
    if (pCallingThread->m_iLastRRqueue == QR_GENERAL_QUEUE) {
      // pop task off the general queue
      pTask = m_pGeneralQueue->pPopTask();
      if (pTask) {
        *ppPoppedTask = pTask;
        return true;
      }
      
      // didn't find a task, move to next queue
      continue;
    }
    
    //--------------------------------------------------------------------------
    // SLOT QUEUES
    
    // pop a task off the partition queues
    pTask = m_pSlotQueues[pCallingThread->m_iLastRRqueue]->pPopTask();
    if (pTask) {
      *ppPoppedTask = pTask;
      return true;
    }
    
    // didn't find a task to run, move to next queue
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
