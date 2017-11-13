/*============================================================================*/
/* Steps_TaskList.cpp - task list step groups                                 */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern Dic*            gpMainDic;

extern Grid*           gpGrid;
extern StepArray*      gpStepArray;
extern ThreadList*     gpThreadList;
extern TaskList*       gpTaskList;

extern NP_Task         gnpBootTask;
extern Lock            gxDdlLock;
extern Lock            gxDicLock;
extern SEitfList*      gpSEitfList;

// tasklist: ///////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_TaskList(void)
{
  vAddPrimitive("taskList"_Stz,        &vTaskList,         false);
  vAddPrimitive("taskList.append"_Stz, &vTaskListAppend,   false);
  vAddPrimitive("taskList.dump"_Stz,   &vTaskListDump,     false);
  vAddPrimitive("taskList.init"_Stz,   &vTaskListInit,     false);
}

/*----------------------------------------------------------------------------*/
/* tasklist -- tasklist                                                       */
/*----------------------------------------------------------------------------*/
void Task::vTaskList(MP_Task mpTask)
{
ENTRY("vTaskList"_Stz);
  
  mpTask->m_pDataStack->vPushObject(gpTaskList);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tasklist.append -- tasklist append                                         */
/*----------------------------------------------------------------------------*/
void Task::vTaskListAppend(MP_Task mpTask)
{
ENTRY("vTaskListAppend"_Stz);
  
  Object* pPtrTaskList;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTaskList) )
    THROW_INVALID_DSTACK("tasklist.append"_Stz);
  
  TaskList* pTaskList = _pTaskList(pPtrTaskList);
  ASSERT(CN_TASKLIST == pTaskList->eClass());
  
  Object* pTaskObject;
  if ( ! mpTask->m_pDataStack->bPop(pTaskObject) )
    THROW_INVALID_DSTACK("task.dump"_Stz);
  Task* pThatTask = _pTask(pTaskObject);
  
  pTaskList->vAppend(pThatTask);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tasklist.dump -- tasklist dump                                             */
/*----------------------------------------------------------------------------*/
void Task::vTaskListDump(MP_Task mpTask)
{
ENTRY("vTaskListDump"_Stz);
  
  Object* pPtrTaskList;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTaskList) )
    THROW_INVALID_DSTACK("tasklist.dump"_Stz);
  
  TaskList* pTaskList = _pTaskList(pPtrTaskList);
  ASSERT(CN_TASKLIST == pTaskList->eClass());
 
  pTaskList->vContentDump();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tasklist.init -- tasklist init                                             */
/*----------------------------------------------------------------------------*/
void Task::vTaskListInit(MP_Task mpTask)
{
ENTRY("vTaskListInit"_Stz);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
