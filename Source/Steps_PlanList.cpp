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
  vAddPrimitive("planList::addPlan"_Str,    &vPlanListAddPlan,    false);
  vAddPrimitive("planList::findPlan"_Str,   &vPlanListFindPlan,   false);
  vAddPrimitive("planList::removePlan"_Str, &vPlanListRemovePlan, false);
  vAddPrimitive("planList::show"_Str,       &vPlanListShow,       false);
}

/*----------------------------------------------------------------------------*/
/* planList.addPlan -- add a plan to plan list ( plan -- )                    */
/*----------------------------------------------------------------------------*/
void Task::vPlanListAddPlan(NNP_Task nnpTask)
{
  ENTRY("vPlanListAddPlan"_Str);

  Object* pPtrPlan;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrPlan) )
    THROW_INVALID_DSTACK("planList.addPlan"_Str);
  
  Plan* pPlan = _pPlan(pPtrPlan);
  
  gpPlanList->vAppend(pPlan);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* planList.findPlan -- find plan in list ( planId -- plan true | false )     */
/*----------------------------------------------------------------------------*/
void Task::vPlanListFindPlan(NNP_Task nnpTask)
{
  ENTRY("vPlanListFindPlan"_Str);
  
  i8 iPlanId;
  
  if ( ! nnpTask->m_pDataStack->bPop(iPlanId) )
    THROW_INVALID_DSTACK("planList.findPlan"_Str);
  
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
  ENTRY("vPlanListRemovePlan"_Str);
  
  Object* pPtrPlan;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrPlan) )
    THROW_INVALID_DSTACK("planList.removePlan"_Str);
  
  Plan* pPlan = _pPlan(pPtrPlan);
  
  gpPlanList->vRemove(pPlan);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* planList.show -- show plan list ( plan -- )                                */
/*----------------------------------------------------------------------------*/
void Task::vPlanListShow(NNP_Task nnpTask)
{
  ENTRY("vPlanListShow"_Str);
  
  gpPlanList->vShow();
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
