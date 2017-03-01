/*============================================================================*/
/* TaskCursor.cpp - Task Cursor                                               */
/*============================================================================*/

#include "Tasks.h"

extern Heap*       gpSmallHeap;
extern TaskList*   gpTaskList;

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
TaskCursor* Tasks::_pTaskCursor(void* aAddr)
{
  TaskCursor* pTaskCursor = reinterpret_cast<TaskCursor*>(aAddr);
  
  RASSERT( !pTaskCursor || CN_TASKCURSOR == pTaskCursor->eClass() );
  
  return pTaskCursor;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* TaskCursor::operator new(size_t iSize, TaskCursor* pTaskCursor)
{
  RASSERT(iSize);
  return pTaskCursor;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void TaskCursor::operator delete(void* pTaskCursorAddress)
{
  RASSERT(pTaskCursorAddress);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
TaskCursor::TaskCursor(List_t<Task>* pList)
: Object(CN_TASKCURSOR),
  ListCursor_t<Task>(pList)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASKCURSOR,_FOOTER_);
  
  _checkInitialised_(CN_TASKCURSOR);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_TASKCURSOR,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
TaskCursor::~TaskCursor(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASKCURSOR,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* TaskCursor::pFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASKCURSOR);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
