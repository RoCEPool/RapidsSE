/*============================================================================*/
/* TaskList.cpp - task list                                                   */
/*============================================================================*/

#include "BASE.h"

#include "SP_Recurring.hpp"

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
TaskList* BASE::_pTaskList(void* aAddr)
{
  TaskList* pTaskQueue = reinterpret_cast<TaskList*>(aAddr);
  
  RASSERT_TYPE(pTaskQueue,CN_TASKLIST);
  
  return pTaskQueue;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* TaskList::operator new(size_t)
{
  void* pObject = Heaps::pAllocate(CN_TASKLIST);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* TaskList::operator new(size_t, TaskList* pTaskQueue)
{
  return pTaskQueue;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void TaskList::operator delete(void* pTaskQueueAddress)
{
  TaskList* pTaskQueue = reinterpret_cast<TaskList*>(pTaskQueueAddress);
  
  Heaps::vFree(pTaskQueue);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
TaskList::TaskList(eQK eKind)
: ListNL_t<Task>(CN_TASKLIST)
, m_okLock(this)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASKLIST,_FOOTER_);
  
  m_eKind = eKind;
  
  _checkInitialised_(CN_TASKLIST);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_TASKLIST,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
TaskList::~TaskList(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASKLIST,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* add task to list                                                           */
/*----------------------------------------------------------------------------*/
void TaskList::vAddTask(Task* pTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASKLIST,_FOOTER_);
  
  ListNL_t<Task>::vAppend(pTask);
}

/*----------------------------------------------------------------------------*/
/* drop lock for write                                                        */
/*----------------------------------------------------------------------------*/
void TaskList::vDropLock(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASKLIST,_FOOTER_);
  
  m_okLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* grab lock for write                                                        */
/*----------------------------------------------------------------------------*/
void TaskList::vGrabLock(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASKLIST,_FOOTER_);
  
  m_okLock.vGrabWrite();
}

/*----------------------------------------------------------------------------*/
/* grab lock for write                                                        */
/*----------------------------------------------------------------------------*/
bool TaskList::bGrabLock(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASKLIST,_FOOTER_);
  
  return m_okLock.bGrabWrite();
}

/*----------------------------------------------------------------------------*/
/* is there a task in the list with given job class?                          */
/*----------------------------------------------------------------------------*/
bool TaskList::bHasJobClass(eCN eJobClass, bool bWasReceived)
{
  //vDump(CN_TASKLIST, this, Stz::szTab(1));
  
  bool bHasJob = false;
  
  vGrabLock();
  
  for (Task* pTask = ListNL_t<Task>::pHead();
       pTask;
       pTask = pTask->pNextItem()) {
    
    if (! (pTask->pJob_task() && AM::eJobClass(pTask->pJob_task()) == eJobClass))
      continue;

    if (! bWasReceived) {
      bHasJob = true;
      break;
    }
    
    if (AM::bJobWasReceived(pTask->pJob_task())) {
      bHasJob = true;
      break;
    }
 
  }

  vDropLock();

  return bHasJob;
}

/*----------------------------------------------------------------------------*/
/* is locked ?                                                                */
/*----------------------------------------------------------------------------*/
bool TaskList::bIsLocked(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASKLIST,_FOOTER_);
  
  return m_okLock.bIsLocked();
}

/*----------------------------------------------------------------------------*/
/* peek at task at top of list                                                */
/*----------------------------------------------------------------------------*/
Task* TaskList::pPeekTask(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASKLIST,_FOOTER_);
  
 
  ASSERT(m_okLock.bIsLocked());
  
  Task* pTask = ListNL_t<Task>::pHead();

  return pTask;
}

/*----------------------------------------------------------------------------*/
/* pop task from top of list                                                  */
/*----------------------------------------------------------------------------*/
Task* TaskList::pPopTask(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASKLIST,_FOOTER_);
  
  ASSERT(m_okLock.bIsLocked());
  
  Task* pTask = ListNL_t<Task>::pPop();

  return pTask;
}

/*----------------------------------------------------------------------------*/
/* remove task from list                                                      */
/*----------------------------------------------------------------------------*/
void TaskList::vRemoveTask(Task* pTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASKLIST,_FOOTER_);
  
  ListNL_t<Task>::vRemove(pTask);
}

/*----------------------------------------------------------------------------*/
/* get number of tasks in list                                                */
/*----------------------------------------------------------------------------*/
i8 TaskList::nTasks(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASKLIST,_FOOTER_);

  i8 nTasks = ListNL_t<Task>::nItems();

  return nTasks;
}

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void TaskList::vContentDump(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASKLIST,_FOOTER_);

  ASSERT(m_okLock.bIsLocked());

  SP_Recurring spRecurring;

  i8 i_Task = 0;
  for (Task* pTask = ListNL_t<Task>::pHead();
       pTask;
       pTask = pTask->pNextItem()) {
    
    vTrace("  [%02lld] %s\n"_Stz
           ,i_Task++
           ,spRecurring.sz(pTask));

  }
}

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void TaskList::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  UNUSED(eClass);
  UNUSED(pObject);
  UNUSED(aTab);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* TaskList::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASKLIST);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
