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
  Object* pObject = gpSmallHeap->pAllocObject(CN_TASKLIST);
  
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
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASKLIST,_FOOTER_);
  
  Memory::vCheckInitialised(this,sizeof(TaskList));

  _postcondition_(_ADDRESS_,_MAGIC_,CN_TASKLIST,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
TaskList::~TaskList(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASKLIST,_FOOTER_);
  
  ASSERT( ! gpTaskList );
  
  gpTaskList = this;
}

/*----------------------------------------------------------------------------*/
/* allocate a task                                                            */
/*----------------------------------------------------------------------------*/
Task* TaskList::pAllocTask(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASKLIST,_FOOTER_);
  
  Task* pTask = reinterpret_cast<Task*>(gpMediumHeap->pAllocObject(CN_TASK));
  
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
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASKLIST,_FOOTER_);
  
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
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASKLIST);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/