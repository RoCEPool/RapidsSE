/*============================================================================*/
/* Steps_PlanList.cpp - plan list step group                                  */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern Arguments       gxArguments;

extern AttachMap*      gpAttachMap;
extern BufferPool*     gpBufferPool;
extern Heap*           gpMediumHeap;
extern BPA*            gpBPA;
extern Dic*            gpMainDic;
extern Word*           gpWord;

extern Grid*           gpGrid;
extern StepArray*      gpStepArray;
extern ThreadList*     gpThreadList;
extern TaskList*       gpTaskList;
extern TxCollection*   gpTxCollection;

extern NP_Task         gnpRootTask;
extern StepBoard*      gpStepBoard;
extern Lock            gxDdlLock;
extern Lock            gxDicLock;
extern SEitfList*      gpSEitfList;
extern PlanList*       gpPlanList;

// plan list: //////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_PlanList(void)
{
  vAddPrimitive("planList::addPlan",    &vPlanListAddPlan,           false);
  vAddPrimitive("planList::findPlan",   &vPlanListFindPlan,          false);
  vAddPrimitive("planList::removePlan", &vPlanListRemovePlan,        false);
}

/*----------------------------------------------------------------------------*/
/* planList.addPlan -- add a plan to plan list ( plan -- )                    */
/*----------------------------------------------------------------------------*/
void Task::vPlanListAddPlan(NNP_Task nnpTask)
{
  ENTRY("vPlanListAddPlan");

  Object* pPtrPlan;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrPlan) )
    THROW_INVALID_DSTACK("planList.addPlan");
  
  Plan* pPlan = _pPlan(pPtrPlan);
  
  gpPlanList->vAppend(pPlan);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* planList.findPlan -- find plan in list ( planId -- plan true | false )     */
/*----------------------------------------------------------------------------*/
void Task::vPlanListFindPlan(NNP_Task nnpTask)
{
  ENTRY("vPlanListFindPlan");
  
  i8 iPlanId;
  
  if ( ! nnpTask->m_pDataStack->bPop(iPlanId) )
    THROW_INVALID_DSTACK("planList.findPlan");
  
  Plan* pPlan = gpPlanList->pFind((u8)iPlanId);
  
  if (pPlan) {
    nnpTask->m_pDataStack->vPush(pPlan);
    nnpTask->m_pDataStack->vPush((i8)1);
  }
  else {
    nnpTask->m_pDataStack->vPush((i8)0);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* planList.removePlan -- remove a plan from plan list ( plan -- )            */
/*----------------------------------------------------------------------------*/
void Task::vPlanListRemovePlan(NNP_Task nnpTask)
{
  ENTRY("vPlanListRemovePlan");
  
  Object* pPtrPlan;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrPlan) )
    THROW_INVALID_DSTACK("planList.removePlan");
  
  Plan* pPlan = _pPlan(pPtrPlan);
  
  gpPlanList->vRemove(pPlan);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
