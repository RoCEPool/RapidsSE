/*============================================================================*/
/* TaskList.cpp - task list                                                   */
/*============================================================================*/

#include "Tasks.h"
#include "Threads.h"

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
TaskList* Tasks::_pTaskList(void* aAddr)
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
  void* pObject = HeapMem::pAllocate(CN_TASKLIST);
  
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
  
  HeapMem::vFree(pTaskQueue);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
TaskList::TaskList(eQK eKind)
: ListNL_t<Task>(CN_TASKLIST)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASKLIST,_FOOTER_);
  
  m_eKind = eKind;
  
  m_oLock.vInitLock(LK_TASK_QUEUE);
  
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
  
  m_oLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* grab lock for write                                                        */
/*----------------------------------------------------------------------------*/
void TaskList::vGrabLock(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASKLIST,_FOOTER_);
  
  m_oLock.vGrabWrite();
}

/*----------------------------------------------------------------------------*/
/* grab lock for write                                                        */
/*----------------------------------------------------------------------------*/
bool TaskList::bGrabLock(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASKLIST,_FOOTER_);
  
  return m_oLock.bGrabWrite();
}

/*----------------------------------------------------------------------------*/
/* is locked ?                                                                */
/*----------------------------------------------------------------------------*/
bool TaskList::bIsLocked(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASKLIST,_FOOTER_);
  
  return m_oLock.bIsLocked();
}

/*----------------------------------------------------------------------------*/
/* peek at task at top of list                                                */
/*----------------------------------------------------------------------------*/
Task* TaskList::pPeekTask(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASKLIST,_FOOTER_);
  
 
  ASSERT(m_oLock.bIsLocked());
  
  Task* pTask = ListNL_t<Task>::pHead();

  return pTask;
}

/*----------------------------------------------------------------------------*/
/* pop task from top of list                                                  */
/*----------------------------------------------------------------------------*/
Task* TaskList::pPopTask(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASKLIST,_FOOTER_);
  
  ASSERT(m_oLock.bIsLocked());
  
  Task* pTask = ListNL_t<Task>::pPop();

  return pTask;
}

/*----------------------------------------------------------------------------*/
/* get the first queue                                                        */
/*----------------------------------------------------------------------------*/
void TaskList::vPortrait(s48K& sPortrait)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASKLIST);

  s48K sTaskPortrait;
  
  sPortrait.vSprint("%15s"_Stz,aQK[m_eKind]);
  
  vGrabLock();
  
  sPortrait.vSprintCat(" %lld "_Stz,nObjects());
  
  for (Task* pTask = pHead(); pTask; pTask = pTask->pNextNode()) {
    
    if (pTask == pHead())
      sPortrait.vConcat(": "_Stz);
    else
      sPortrait.vConcat(" "_Stz);
    
    pTask->vMonitorPortrait(sTaskPortrait);
    
    sPortrait.vConcat(sTaskPortrait);
  }
  sPortrait.vConcat("\n"_Stz);

  vDropLock();
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

  i8 nTasks = ListNL_t<Task>::nObjects();

  return nTasks;
}

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void TaskList::vContentDump(void)
{
  //vDump(CN_TASKLIST, this, aSingleTab());
  
  ASSERT(m_oLock.bIsLocked());

  vTrace("ITEMS %lld\n"_Stz,ListNL_t<Task>::nObjects());
  
  for (Task* pTask = ListNL_t<Task>::pHead();
       pTask;
       pTask = pTask->pNextNode()) {
    
    vTrace("Task %p %s %s order %lld\n"_Stz,
           pTask,
           aCN[pTask->pJob()->eClass()],
           aTS[pTask->eState()],
           pTask->iQueueOrder()
           );

  }
}

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void TaskList::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  
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
