/*============================================================================*/
/* Scheduler.cpp - schedule tasks to threads                                  */
/*============================================================================*/

#include "Schedulers.h"

extern BufferPool*  gpBufferPool;

#include "Thread.hpp"

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Scheduler* Schedulers::_pScheduler(void* aAddr)
{
  Scheduler* pScheduler = reinterpret_cast<Scheduler*>(aAddr);
  
  RASSERT( !pScheduler || CN_SCHEDULER == pScheduler->eClass() );
  
  return pScheduler;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Scheduler::operator new(size_t iSize, Scheduler* pScheduler)
{
  RASSERT(iSize);
  return pScheduler;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void Scheduler::operator delete(void* aAddress)
{
  RASSERT(aAddress);
  vFatal("fatal: Scheduler class cannot be deleted"_Str);
  
  RASSERT(0);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Scheduler::Scheduler(void)
: Object(CN_SCHEDULER)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SCHEDULER,_FOOTER_);
  
  m_xLock.vInit("Scheduler::m_xLock"_Str);
  
  
  _checkInitialised_(CN_SCHEDULER);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_SCHEDULER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* grab write lock on this task queue                                         */
/*----------------------------------------------------------------------------*/
void Scheduler::vGrabLock(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SCHEDULER,_FOOTER_);

  m_xLock.vGrabWrite();
}

/*----------------------------------------------------------------------------*/
/* drop write lock on this task queue                                         */
/*----------------------------------------------------------------------------*/
void Scheduler::vDropLock(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SCHEDULER,_FOOTER_);
  
  m_xLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* queue task onto this schedulers task queue                                 */
/*----------------------------------------------------------------------------*/
void Scheduler::vEnqueueTask(Task* pTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SCHEDULER,_FOOTER_);
  
  ASSERT(m_xLock.bIsLocked());
  
  m_xTaskQueue.vAppend(pTask);
}

/*----------------------------------------------------------------------------*/
/* how many tasks are currently queued?                                       */
/*----------------------------------------------------------------------------*/
i8 Scheduler::iQueuedTasks(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SCHEDULER,_FOOTER_);

  i8 nTasks = m_xTaskQueue.iCount();
  
  return nTasks;
}

/*----------------------------------------------------------------------------*/
/* queue thread onto this task queue                                          */
/*----------------------------------------------------------------------------*/
void Scheduler::vEnqueueThread(Threads::Thread* npThread)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SCHEDULER,_FOOTER_);
  
  ASSERT(m_xLock.bIsLocked());
  
  m_xThreadQueue.vAppend(npThread);
}

/*----------------------------------------------------------------------------*/
/* how many threads are currently queued?                                     */
/*----------------------------------------------------------------------------*/
i8 Scheduler::iQueuedThreads(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SCHEDULER,_FOOTER_);
  
  i8 iCount = m_xThreadQueue.iCount();
  
  return iCount;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Scheduler::pFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SCHEDULER);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
