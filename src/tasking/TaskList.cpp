/*============================================================================*/
/* TaskList.cpp - task list                                                   */
/*============================================================================*/

#include "Tasking.h"

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
TaskList* Tasking::_pTaskList(void* aAddr)
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
, m_okLock(LK_TASK_QUEUE,this)
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
    
    if (! (pTask->pJob() && pTask->pJob()->eClass() == eJobClass))
      continue;

    if (! bWasReceived) {
      bHasJob = true;
      break;
    }
    
    if (pTask->pJob()->bWasReceived()) {
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
/* get the first queue                                                        */
/*----------------------------------------------------------------------------*/
void TaskList::vPortrait(s48K& sPortrait)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASKLIST);

  s48K sTaskPortrait;
  
  sPortrait.vSprint("%15s"_Stz,aQK[m_eKind]);
  
  vGrabLock();
  
  sPortrait.vSprintCat(" %2lld "_Stz,nItems());
  
  eCN eJobPrev = CN_NONE;
  i8  nPrev = 0;
  
  for (Task* pTask = pHead(); pTask; pTask = pTask->pNextItem()) {
    
    if (! pTask->pJob())
      continue;
    
    if (pTask == pHead()) {
      sPortrait.vConcat("- "_Stz);
      pTask->vMonitorPortrait(sTaskPortrait);
      sPortrait.vConcat(sTaskPortrait);
      eJobPrev = pTask->pJob()->eClass();
      nPrev = 0;
      continue;
    }

    if (pTask->pJob() && pTask->pJob()->eClass() == eJobPrev) {
      nPrev++;
      continue;
    }
    
    if (nPrev) {
      sPortrait.vSprintCat("+%lld"_Stz,nPrev);
      nPrev = 0;
    }
    
    sPortrait.vConcat(" "_Stz);
    pTask->vMonitorPortrait(sTaskPortrait);
    sPortrait.vConcat(sTaskPortrait);
    eJobPrev = pTask->pJob()->eClass();
  }
  
  if (nPrev)
    sPortrait.vSprintCat("+%lld"_Stz,nPrev);

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

  i8 nTasks = ListNL_t<Task>::nItems();

  return nTasks;
}

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void TaskList::vContentDump(void)
{
  //vDump(CN_TASKLIST, this, Stz::szTab(1));
  
  ASSERT(m_okLock.bIsLocked());

  //vTrace("ITEMS %lld\n"_Stz,nItems());

  for (Task* pTask = ListNL_t<Task>::pHead();
       pTask;
       pTask = pTask->pNextItem()) {
    
    Job* pJob = pTask->pJob();
    
    vTrace("Task %p %s Job %p %s"_Stz,
           pTask,
           aTS[pTask->eState()],
           
           pJob,
           aCN[pJob->eClass()]
           );
    
    if (void* aThread = pTask->pvThread_DONT_DEREF()) {
     vTrace(" Thread %p"_Stz,
            aThread);

    }

    vTrace("\n"_Stz);
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
