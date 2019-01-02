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
TaskQueue::TaskQueue(cmpFun fCompare)
: Object(CN_TASKQUEUE)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASKQUEUE,_FOOTER_);
  
  m_oLock.vInitLock(LK_TASK_QUEUE);
  m_pTaskSkipList = new SkipList(true, fCompare);
  
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
/* add task to the queue                                                      */
/*----------------------------------------------------------------------------*/
void TaskQueue::vAddTask(Task* pTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASKQUEUE,_FOOTER_);
  
  ASSERT(m_pTaskSkipList->eClass() == CN_SKIPLIST);
  m_pTaskSkipList->bInsert(pTask);
}

/*----------------------------------------------------------------------------*/
/* drop lock for write                                                        */
/*----------------------------------------------------------------------------*/
void TaskQueue::vDropLock(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASKQUEUE,_FOOTER_);
  
  m_oLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* grab lock for write                                                        */
/*----------------------------------------------------------------------------*/
void TaskQueue::vGrabLock(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASKQUEUE,_FOOTER_);
  
  m_oLock.vGrabWrite();
}

/*----------------------------------------------------------------------------*/
/* grab lock for write                                                        */
/*----------------------------------------------------------------------------*/
bool TaskQueue::bGrabLock(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASKQUEUE,_FOOTER_);
  
  return m_oLock.bGrabWrite();
}

/*----------------------------------------------------------------------------*/
/* is locked ?                                                                */
/*----------------------------------------------------------------------------*/
bool TaskQueue::bIsLocked(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASKQUEUE,_FOOTER_);
  
  return m_oLock.bIsLocked();
}

/*----------------------------------------------------------------------------*/
/* peek at a task at the front of the queue                                   */
/*----------------------------------------------------------------------------*/
Task* TaskQueue::pPeekTask(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASKQUEUE,_FOOTER_);
  
  Task* pTask = nullptr;
  
   ASSERT(m_oLock.bIsLocked());
  
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
  
  ASSERT(m_oLock.bIsLocked());
  
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
/* remove a task from queue                                                   */
/*----------------------------------------------------------------------------*/
void TaskQueue::vRemoveTask(Task* pTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASKQUEUE,_FOOTER_);
  
  bool bRemoved;
  
  ASSERT(m_oLock.bIsLocked());
  
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
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void TaskQueue::vContentDump(void)
{
  vDump(CN_TASKQUEUE, this, aSingleTab());
}

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void TaskQueue::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  TaskQueue* pTaskQueue = _pTaskQueue(pObject);
  
  SkipList* pSkipList = pTaskQueue->m_pTaskSkipList;
  vTrace("HEIGHT %lld HEAD 0x%llx ITEMS %lld\n"_Stz,pSkipList->m_iHeight,pSkipList->m_pHead, pSkipList->m_nObjects);
  i8 iLevel = pSkipList->m_iHeight;
  do {
    //vTrace("LEVEL %lld\n"_Stz, iLevel);
    
    SkipNode* pNode = pSkipList->pFirst(iLevel);
    while (pNode) {
      i8 i = (i8)pNode->m_pObject;
      Task* pTask = (Task*)(pNode->m_pObject);
      vTrace("Task %p %s id %lld (L%lld) Node %p\n"_Stz,
             pTask,
             aCN[pTask->eClass()],
             pTask->iQueueOrder(),
             pNode->m_nRight,
             pNode);
      
      pNode = pSkipList->pNext(pNode, iLevel);
    }
  } while (--iLevel >= 0);
  
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
