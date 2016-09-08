/*============================================================================*/
/* TaskList.cpp - task List                                                   */
/*============================================================================*/

#include "Tasks.h"

extern Heap*        gpSmallHeap;
extern Heap*        gpMediumHeap;
extern TaskList*    gpTaskList;

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* TaskList::operator new(size_t iSize)
{
  Object* pObject = gpSmallHeap->pAllocObject();
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* TaskList::operator new(size_t iSize, TaskList* pTaskList)
{
  return pTaskList;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void TaskList::operator delete(void* pTaskListAddress)
{
  TaskList* pTaskList = reinterpret_cast<TaskList*>(pTaskListAddress);
  
  gpSmallHeap->vFreeObject(pTaskList);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
TaskList::TaskList(void)
: Object(CN_TASKLIST)
{
  Memory::vCheckInitialised(this,sizeof(TaskList));
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
TaskList::~TaskList(void)
{
  _poolAddress_(this);
  
  ASSERT( ! gpTaskList );
  
  gpTaskList = this;
}

/*----------------------------------------------------------------------------*/
/* allocate a task                                                            */
/*----------------------------------------------------------------------------*/
Task* TaskList::pAllocTask(void)
{
  _poolAddress_(this);
  
  Task* pTask = reinterpret_cast<Task*>(gpMediumHeap->pAllocObject());
  
  vAppend(pTask);
  
  return pTask;
}


/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void TaskList::vDump(void)
{
  vDump(CN_TASKLIST, this, pSingleTab());
}

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void TaskList::vDump(eCN eClass, Object* pObject, const char* pTab)
{
  ASSERT(eClass == CN_TASKLIST);
  ASSERT(pObject->eClass() == CN_TASKLIST);
  
  TaskList* pTaskList = reinterpret_cast<TaskList*>(pObject);
  
  vTrace("%sTaskList %p: \n", pTab, pTaskList);
  vTrace("%s  count: %lld\n", pTab, pTaskList->iCount());
  vTrace("%s  head %p\n", pTab, pTaskList->pHead());
  vTrace("%s  tail %p\n\n", pTab, pTaskList->pTail());
}

/*----------------------------------------------------------------------------*/
/* free a task                                                                */
/*----------------------------------------------------------------------------*/
void TaskList::vFreeTask(Task* pTask)
{
  _poolAddress_(this);
  
  vRemove(pTask);
  
  gpMediumHeap->vFreeObject(pTask);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* TaskList::pFooter(void)
{
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/