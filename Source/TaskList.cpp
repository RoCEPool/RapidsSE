/*============================================================================*/
/* TaskList.cpp - task List                                                   */
/*============================================================================*/

#include "Tasks.h"

extern Heap*        gpSmallHeap;
extern Heap*        gpMediumHeap;
extern TaskList*    gpTaskList;

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
TaskList* Tasks::_pTaskList(void* pAddr)
{
  TaskList* pTaskList = reinterpret_cast<TaskList*>(pAddr);
  
  return pTaskList;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* TaskList::operator new(size_t iSize)
{
  RASSERT(iSize);

  void* pObject = Heap::pAllocate(CN_TASKLIST);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* TaskList::operator new(size_t iSize, TaskList* pTaskList)
{
  RASSERT(iSize);
  return pTaskList;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void TaskList::operator delete(void* pTaskListAddress)
{
  TaskList* pTaskList = reinterpret_cast<TaskList*>(pTaskListAddress);
  
  Heap::vFree(pTaskList);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
TaskList::TaskList(void)
: Object(CN_TASKLIST)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASKLIST,_FOOTER_);
  
  _checkInitialised_(CN_TASKLIST);

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
  
  pTaskList->List_t<Task>::vDump("TaskList::", pTab);
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