/*============================================================================*/
/* Steps_TaskList.cpp - tasklist step group                                   */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"
#include "Job.hpp"
#include "LogicUnit.hpp"
#include "TaskList.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_TaskList(void)
{
  vAddStep("TaskList[]"_Stz,           &Step::vTaskListGet);
  vAddStep("TaskList[].get"_Stz,       &Step::vTaskListGet);
  vAddStep("TaskList[].set"_Stz,       &Step::vTaskListSet);

  vAddStep("tasklist.delete"_Stz ,     &Step::vTaskListDelete);
  vAddStep("tasklist.isEmpty"_Stz,     &Step::vTaskListIsEmpty);
  vAddStep("tasklist.pop"_Stz,         &Step::vTaskListPop);

}

/*----------------------------------------------------------------------------*/
/* TaskList[] / TaskList[].get -- get the tasklist register                   */
/*----------------------------------------------------------------------------*/
void Step::vTaskListGet(void)
{
  ENTRY();

  m_pDataStack->vPushPtr(*m_pLogicUnit->m_xpTaskList);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* TaskList[].set -- set the tasklist register                                */
/*----------------------------------------------------------------------------*/
void Step::vTaskListSet(void)
{
  ENTRY();

  TaskList* pTaskList = _pTaskList(m_pDataStack->pPopPtr(__func__));
  ASSERT(pTaskList);

  ASSERT(*m_pLogicUnit->m_xpTaskList);
  m_pLogicUnit->m_xpTaskList = pTaskList;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tasklist.delete -- delete the tasklist on the stack                        */
/*----------------------------------------------------------------------------*/
void Step::vTaskListDelete(void)
{
  ENTRY();

  TaskList* pTaskList = _pTaskList(m_pDataStack->pPopPtr(__func__));
  ASSERT(pTaskList);
  delete pTaskList;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tasklist.isEmpty -- return true if the task list is empty                  */
/*----------------------------------------------------------------------------*/
void Step::vTaskListIsEmpty(void)
{
  ENTRY();

  TaskList* pTaskList = _pTaskList(m_pDataStack->pPopPtr(__func__));
  ASSERT(pTaskList);

  bool bIsEmpty = (pTaskList->pPeekTask() != nullptr);
  m_pDataStack->vPushBol(bIsEmpty);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tasklist.pop -- pop a task off the tasklist                                */
/*----------------------------------------------------------------------------*/
void Step::vTaskListPop(void)
{
  ENTRY();

  TaskList* pTaskList = _pTaskList(m_pDataStack->pPopPtr(__func__));
  ASSERT(pTaskList);

  Task* pTask = pTaskList->pPopTask();
  m_pDataStack->vPushPtr(pTask);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
