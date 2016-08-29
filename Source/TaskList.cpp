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
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void TaskList::vDump(const char* pTab)
{
  _poolAddress_(this);
  /*
   vTrace("%sTask %p: \n", pSpace, this);
   vTrace("%s  name: %16s \n", pSpace, m_sName.a());
   vTrace("%s  taskNo %02lld\n", pSpace,m_iTaskNo);
   vTrace("%s  state %s\n", pSpace, pTS[m_eState]);
   vTrace("%s  txId %lld \n\n", pSpace, m_iTxId);
   */
  vTrace("%sTaskList %p: \n", pTab, this);
  vTrace("%s  count: %lld\n", pTab, iCount());
  vTrace("%s  head %p\n", pTab,pHead());
  vTrace("%s  tail %p\n\n", pTab, pTail());
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