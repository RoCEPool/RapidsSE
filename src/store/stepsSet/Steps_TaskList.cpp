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
  vAddStep("TaskList[].isNull"_Stz,    &Step::vTaskListIsNull);
  vAddStep("TaskList[].delete"_Stz ,   &Step::vTaskListDelete);
  vAddStep("TaskList[].isEmpty"_Stz,   &Step::vTaskListIsEmpty);
  vAddStep("TaskList[].pop"_Stz,       &Step::vTaskListPop);

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

  ASSERT(m_pLogicUnit->m_xpTaskList.bIsNull());
  m_pLogicUnit->m_xpTaskList = pTaskList;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* TaskList[].delete -- delete the tasklist register                          */
/*----------------------------------------------------------------------------*/
void Step::vTaskListDelete(void)
{
  ENTRY();

  if (m_pLogicUnit->m_xpTaskList.bIsNull()) {
    return;
  }

  TaskList* pTaskList = *m_pLogicUnit->m_xpTaskList;
  Task* pTask;

  pTaskList->vGrabLock();
  while ((pTask = pTaskList->pPopTask()) != nullptr) {
    delete pTask;
  }
  pTaskList->vDropLock();

  delete pTaskList;

  m_pLogicUnit->m_xpTaskList = nullptr;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tasklist.isEmpty -- return true if the task list is empty                  */
/*----------------------------------------------------------------------------*/
void Step::vTaskListIsEmpty(void)
{
  ENTRY();

  TaskList* pTaskList = * m_pLogicUnit->m_xpTaskList;

  pTaskList->vGrabLock();
  m_pDataStack->vPushBol(m_pLogicUnit->m_xpTaskList->pPeekTask() == nullptr);
  pTaskList->vDropLock();

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tasklist.isNull -- return true if the task list register is null           */
/*----------------------------------------------------------------------------*/
void Step::vTaskListIsNull(void)
{
  ENTRY();

  m_pDataStack->vPushBol(m_pLogicUnit->m_xpTaskList.bIsNull());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tasklist.pop -- pop a task off the tasklist                                */
/*----------------------------------------------------------------------------*/
void Step::vTaskListPop(void)
{
  ENTRY();

  TaskList* pTaskList = *m_pLogicUnit->m_xpTaskList;
  ASSERT(pTaskList);

  pTaskList->vGrabLock();
  Task* pTask = pTaskList->pPopTask();
  pTaskList->vDropLock();

  m_pDataStack->vPushPtr(pTask);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
