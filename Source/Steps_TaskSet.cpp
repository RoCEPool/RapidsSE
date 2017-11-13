/*============================================================================*/
/* Steps_TaskSet.cpp - task set step group                                    */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern Dic*            gpMainDic;
extern Word*           gpWord;

extern Grid*           gpGrid;
extern StepArray*      gpStepArray;
extern ThreadList*     gpThreadList;
extern TaskList*       gpTaskList;

extern NP_Task         gnpBootTask;
extern Lock            gxDdlLock;
extern Lock            gxDicLock;
extern SEitfList*      gpSEitfList;

// taskset: ////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_TaskSet(void)
{
  vAddPrimitive("taskSet.append"_Stz,  &vTaskSetAppend,    false);
  vAddPrimitive("taskSet.dump"_Stz,    &vTaskSetDump,      false);
  vAddPrimitive("taskSet.init"_Stz,    &vTaskSetInit,      false);
  vAddPrimitive("taskSet::new"_Stz,    &vTaskSetNew,       false);
}

/*----------------------------------------------------------------------------*/
/* taskset.append -- taskset append                                           */
/*----------------------------------------------------------------------------*/
void Task::vTaskSetAppend(MP_Task mpTask)
{
ENTRY("vTaskSetAppend"_Stz);
  
  Object* pObject;
  
  if ( ! mpTask->m_pDataStack->bPop(pObject) )
    THROW_INVALID_DSTACK("taskset.append"_Stz);
  
  ASSERT(CN_TASKSET == pObject->eClass());
  
  TaskSet* pTaskSet = static_cast<TaskSet*>(pObject);
  
  Object* pTaskObject;
  if ( ! mpTask->m_pDataStack->bPop(pTaskObject) )
    THROW_INVALID_DSTACK("taskset.append"_Stz);
  Task* pThatTask = _pTask(pTaskObject);
  
  pTaskSet->vAppend(pThatTask);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* taskset.dump -- taskset dump                                               */
/*----------------------------------------------------------------------------*/
void Task::vTaskSetDump(MP_Task mpTask)
{
ENTRY("vTaskSetDump"_Stz);
  
  Object* pObject;
  
  if ( ! mpTask->m_pDataStack->bPop(pObject) )
    THROW_INVALID_DSTACK("taskset.dump"_Stz);
  
  ASSERT(CN_TASKSET == pObject->eClass());
  
  TaskSet* pTaskSet = static_cast<TaskSet*>(pObject);
  
  pTaskSet->vContentDump();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* taskset.init -- taskset init                                               */
/*----------------------------------------------------------------------------*/
void Task::vTaskSetInit(MP_Task mpTask)
{
ENTRY("vTaskSetInit"_Stz);
  
  Object* pObject;
  
  if ( ! mpTask->m_pDataStack->bPop(pObject) )
    THROW_INVALID_DSTACK("taskset.init"_Stz);
  
  ASSERT(CN_TASKSET == pObject->eClass());
  
  TaskSet* pTaskSet = static_cast<TaskSet*>(pObject);
  
  new(pTaskSet) TaskSet;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* taskset::new -- create new taskset                                         */
/*----------------------------------------------------------------------------*/
void Task::vTaskSetNew(MP_Task mpTask)
{
  ENTRY("vTaskSetNew"_Stz);

  TaskSet* mpTaskSet = new TaskSet;
  
  mpTask->m_pDataStack->vPushObject(mpTaskSet);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
