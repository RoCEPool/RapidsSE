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
  
  RASSERT( !pTaskQueue || CN_TASKLIST == pTaskQueue->eClass() );
  
  return pTaskQueue;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* TaskList::operator new(size_t iSize)
{
  RASSERT(iSize);

  void* pObject = HeapMem::pAllocate(CN_TASKLIST);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* TaskList::operator new(size_t iSize, TaskList* pTaskQueue)
{
  RASSERT(iSize);
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
TaskList::TaskList(cmpFun fCompare)
: Object(CN_TASKLIST)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASKLIST,_FOOTER_);
  
  m_xLock.vInitLock(LK_TASK_QUEUE);
  //m_pTaskSkipList = new SkipList(true, fCompare);
  
  m_pListTasks = new ListNL_t<Task>;
  
  _checkInitialised_(CN_TASKLIST);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_TASKLIST,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
TaskList::~TaskList(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASKLIST,_FOOTER_);
  
  delete m_pListTasks;
}

/*----------------------------------------------------------------------------*/
/* add task to the queue                                                      */
/*----------------------------------------------------------------------------*/
void TaskList::vAddTask(Task* pTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASKLIST,_FOOTER_);
  
  //ASSERT(m_pTaskSkipList->eClass() == CN_SKIPLIST);
  //m_pTaskSkipList->bInsert(pTask);
  
  m_pListTasks->vAppend(pTask);
}

/*----------------------------------------------------------------------------*/
/* drop lock for write                                                        */
/*----------------------------------------------------------------------------*/
void TaskList::vDropLock(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASKLIST,_FOOTER_);
  
  m_xLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* grab lock for write                                                        */
/*----------------------------------------------------------------------------*/
void TaskList::vGrabLock(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASKLIST,_FOOTER_);
  
  m_xLock.vGrabWrite();
}

/*----------------------------------------------------------------------------*/
/* grab lock for write                                                        */
/*----------------------------------------------------------------------------*/
bool TaskList::bGrabLock(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASKLIST,_FOOTER_);
  
  return m_xLock.bGrabWrite();
}

/*----------------------------------------------------------------------------*/
/* is locked ?                                                                */
/*----------------------------------------------------------------------------*/
bool TaskList::bIsLocked(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASKLIST,_FOOTER_);
  
  return m_xLock.bIsLocked();
}

/*----------------------------------------------------------------------------*/
/* peek at a task at the front of the queue                                   */
/*----------------------------------------------------------------------------*/
Task* TaskList::pPeekTask(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASKLIST,_FOOTER_);
  
  Task* pTask = nullptr;
  
  ASSERT(m_xLock.bIsLocked());
  
  //SkipNode* pNode = m_pTaskSkipList->pFirst(0);
  //if (pNode)
  //  pTask = _pTask(pNode->m_pObject);
  
  pTask = m_pListTasks->pHead();
  
  return pTask;
}

/*----------------------------------------------------------------------------*/
/* pop a task from the front of the queue                                     */
/*----------------------------------------------------------------------------*/
Task* TaskList::pPopTask(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASKLIST,_FOOTER_);
  
  //RJH vTrace("pPopTask\n"_Stz);
  
  //bool bRemoved;
  Task* pTask = nullptr;
  
  ASSERT(m_xLock.bIsLocked());
  
//  SkipNode* pNode = m_pTaskSkipList->pFirst(0);
//  if (pNode) {
//    pTask = _pTask(pNode->m_pObject);
//    RASSERT(pTask);
//    bRemoved = m_pTaskSkipList->bRemove(pTask);
//    RASSERT(bRemoved);
//  }

  pTask = m_pListTasks->pPop();
  
  return pTask;
}

/*----------------------------------------------------------------------------*/
/* get number of tasks in queue                                               */
/*----------------------------------------------------------------------------*/
i8 TaskList::nTasks(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASKLIST,_FOOTER_);

  //i8 nTasks = m_pTaskSkipList->nObjects();
  i8 nTasks = m_pListTasks->nObjects();

  return nTasks;
}

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void TaskList::vContentDump(void)
{
  vDump(CN_TASKLIST, this, aSingleTab());
}

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void TaskList::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  RASSERT(eClass == CN_TASKLIST);
  RASSERT(pObject->eClass() == CN_TASKLIST);
  
  /*
  TaskList* pTaskQueue = reinterpret_cast<TaskList*>(pObject);
  
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
  
  vTrace("====================================\n"_Stz);
  vTrace("\n"_Stz);
  
  vTrace("\n"_Stz);
  //pTaskQueue->List_t<Task>::vContentDump("TaskList::"_Stz, aTab);
   */
  
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
