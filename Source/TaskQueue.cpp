/*============================================================================*/
/* TaskQueue.cpp - task queue                                                 */
/*============================================================================*/

#include "Tasks.h"
#include "Threads.h"


/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
TaskQueue* Tasks::_pTaskQueue(void* aAddr)
{
  TaskQueue* pTaskQueue = reinterpret_cast<TaskQueue*>(aAddr);
  
  RASSERT_TYPE(pTaskQueue,CN_TASKQUEUE);
  
  return pTaskQueue;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* TaskQueue::operator new(size_t)
{
  void* pObject = HeapMem::pAllocate(CN_TASKQUEUE);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* TaskQueue::operator new(size_t, TaskQueue* pTaskQueue)
{
  return pTaskQueue;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void TaskQueue::operator delete(void* pTaskQueueAddress)
{
  TaskQueue* pTaskQueue = reinterpret_cast<TaskQueue*>(pTaskQueueAddress);
  
  HeapMem::vFree(pTaskQueue);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
TaskQueue::TaskQueue(eQK eKind, bool bUniqueKey, CompareFp fpCompare)
: Object(CN_TASKQUEUE)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASKQUEUE,_FOOTER_);
  
  m_eKind = eKind;
  m_okLock.vInitLock(LK_TASK_QUEUE);
  m_pTaskSkipList = new SkipListNL(bUniqueKey, fpCompare);
  
  _checkInitialised_(CN_TASKQUEUE);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_TASKQUEUE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
TaskQueue::~TaskQueue(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASKQUEUE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add task to the queue                                                      */
/*----------------------------------------------------------------------------*/
void TaskQueue::vAddTask(Task* pTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASKQUEUE,_FOOTER_);
  
  ASSERT(m_okLock.bIsLocked());

  bool bInserted = m_pTaskSkipList->bInsert(pTask);
  ASSERT(bInserted);
}

/*----------------------------------------------------------------------------*/
/* assert number of items in task queue                                       */
/*----------------------------------------------------------------------------*/
void TaskQueue::vAssertItems(i8 nItems)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASKQUEUE,_FOOTER_);
  
  ASSERT(m_okLock.bIsLocked());
  
  ASSERT(nItems == m_pTaskSkipList->nObjects());
  
  ASSERT(nItems == m_pTaskSkipList->nRecount());
}

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void TaskQueue::vContentDump(void)
{
  //  vTrace("HEIGHT %lld HEAD 0x%llx ITEMS %lld\n"_Stz,
  //         pSkipList->m_iHeight,pSkipList->m_pHead, pSkipList->m_nObjects);
  
  for (SkipNode* pNode = m_pTaskSkipList->pFirst(0);
       pNode;
       pNode = m_pTaskSkipList->pNext(pNode,0)) {
    
    Task* pTask = _pTask(pNode->m_pObject);
 
    pTask->vRenderPortrait();
    
    vTrace("%s\n"_Stz,pTask->sPortrait().aChar());
  }
}

/*----------------------------------------------------------------------------*/
/* drop lock for write                                                        */
/*----------------------------------------------------------------------------*/
void TaskQueue::vDropLock(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASKQUEUE,_FOOTER_);
  
  m_okLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* grab lock for write                                                        */
/*----------------------------------------------------------------------------*/
void TaskQueue::vGrabLock(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASKQUEUE,_FOOTER_);
  
  m_okLock.vGrabWrite();
}

/*----------------------------------------------------------------------------*/
/* grab lock for write                                                        */
/*----------------------------------------------------------------------------*/
bool TaskQueue::bGrabLock(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASKQUEUE,_FOOTER_);
  
  return m_okLock.bGrabWrite();
}

/*----------------------------------------------------------------------------*/
/* is locked ?                                                                */
/*----------------------------------------------------------------------------*/
bool TaskQueue::bIsLocked(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASKQUEUE,_FOOTER_);
  
  return m_okLock.bIsLocked();
}

/*----------------------------------------------------------------------------*/
/* peek at a task at the front of the queue                                   */
/*----------------------------------------------------------------------------*/
Task* TaskQueue::pPeekTask(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASKQUEUE,_FOOTER_);
  
  Task* pTask = nullptr;
  
  ASSERT(m_okLock.bIsLocked());
  
  SkipNode* pNode = m_pTaskSkipList->pFirst(0);
  if (pNode)
    pTask = _pTask(pNode->m_pObject);
  
  
  return pTask;
}

/*----------------------------------------------------------------------------*/
/* pop a task from the front of the queue                                     */
/*----------------------------------------------------------------------------*/
Task* TaskQueue::pPopTask(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASKQUEUE,_FOOTER_);
  
  //RJH vTrace("pPopTask\n"_Stz);
  
  bool bRemoved;
  Task* pTask = nullptr;
  
  ASSERT(m_okLock.bIsLocked());
  
  SkipNode* pNode = m_pTaskSkipList->pFirst(0);
  if (pNode) {
    pTask = _pTask(pNode->m_pObject);
    RASSERT(pTask);
    bRemoved = m_pTaskSkipList->bRemove(pTask);
    RASSERT(bRemoved);
  }

  return pTask;
}

/*----------------------------------------------------------------------------*/
/* get the first queue                                                        */
/*----------------------------------------------------------------------------*/
void TaskQueue::vPortrait(s48K& sPortrait)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASKQUEUE);
  
  sPortrait.vSprint("%15 Portrait\n"_Stz,aQK[m_eKind]);
  
  vGrabLock();
  
  sPortrait.vSprintCat("%lld tasks\n"_Stz,nTasks());
  
  vDropLock();
  
  
  s48K sTaskPortrait;
  
  sPortrait.vSprint("%15s"_Stz,aQK[m_eKind]);
  
  vGrabLock();
  
  sPortrait.vSprintCat(" %2lld "_Stz,m_pTaskSkipList->nObjects());
  
  eCN eJobPrev = CN_NONE;
  i8  nPrev = 0;
  
  for (SkipNode* pNode = m_pTaskSkipList->pFirst(0);
       pNode;
       pNode = m_pTaskSkipList->pNext(pNode,0)) {
    
    Task* pTask = _pTask(pNode->m_pObject);
    
    if (pTask == _pTask(m_pTaskSkipList->pFirst(0)->m_pObject)) {
      sPortrait.vConcat("- "_Stz);
      if (QK_SLEEPING_QUEUE == m_eKind) {
        i8 iInUs = pTask->iWakeupInUsec();
        sPortrait.vCatSprint("in %lldus "_Stz,iInUs);
      }
      pTask->vMonitorPortrait(sTaskPortrait);
      sPortrait.vConcat(sTaskPortrait);
      eJobPrev = pTask->pJob()->eClass();
      nPrev = 0;
      continue;
    }
    
    if (pTask->pJob()->eClass() == eJobPrev) {
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
/* remove a task from queue                                                   */
/*----------------------------------------------------------------------------*/
void TaskQueue::vRemoveTask(Task* pTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASKQUEUE,_FOOTER_);
  
  bool bRemoved;
  
  ASSERT(m_okLock.bIsLocked());
  
  bRemoved = m_pTaskSkipList->bRemove(pTask);
}

/*----------------------------------------------------------------------------*/
/* get number of tasks in queue                                               */
/*----------------------------------------------------------------------------*/
i8 TaskQueue::nTasks(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASKQUEUE,_FOOTER_);

  i8 nTasks = m_pTaskSkipList->nObjects();
  
  return nTasks;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* TaskQueue::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASKQUEUE);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
