/*============================================================================*/
/* TaskList.cpp - task List                                                   */
/*============================================================================*/

#include "Tasking.h"

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
/* free a task                                                                */
/*----------------------------------------------------------------------------*/
void TaskList::vFreeTask(Task* pTask)
{
  _poolAddress_(this);
  
  vRemove(pTask);
  
  gpMediumHeap->vFreeObject(pTask);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/