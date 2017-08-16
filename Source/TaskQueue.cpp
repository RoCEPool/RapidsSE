/*============================================================================*/
/* TaskQueue.cpp - task queue                                                 */
/*============================================================================*/

#include "Tasks.h"

extern Heap*        gpSmallHeap;
extern Heap*        gpMediumHeap;

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
TaskQueue* Tasks::_pTaskQueue(void* aAddr)
{
  TaskQueue* pTaskQueue = reinterpret_cast<TaskQueue*>(aAddr);
  
  RASSERT( !pTaskQueue || CN_TASKQUEUE == pTaskQueue->eClass() );
  
  return pTaskQueue;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* TaskQueue::operator new(size_t iSize)
{
  RASSERT(iSize);

  void* pObject = Heap::pAllocate(CN_TASKQUEUE);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* TaskQueue::operator new(size_t iSize, TaskQueue* pTaskQueue)
{
  RASSERT(iSize);
  return pTaskQueue;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void TaskQueue::operator delete(void* pTaskQueueAddress)
{
  TaskQueue* pTaskQueue = reinterpret_cast<TaskQueue*>(pTaskQueueAddress);
  
  Heap::vFree(pTaskQueue);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
TaskQueue::TaskQueue(void)
: Object(CN_TASKQUEUE)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASKQUEUE,_FOOTER_);
  
  _checkInitialised_(CN_TASKQUEUE);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_TASKQUEUE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
TaskQueue::~TaskQueue(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASKQUEUE,_FOOTER_);
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
  ASSERT(eClass == CN_TASKQUEUE);
  ASSERT(pObject->eClass() == CN_TASKQUEUE);
  
  TaskQueue* pTaskQueue = reinterpret_cast<TaskQueue*>(pObject);
  
  vTrace("\n"_Stz);
  //pTaskQueue->List_t<Task>::vContentDump("TaskQueue::"_Stz, aTab);
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
