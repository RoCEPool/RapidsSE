/*============================================================================*/
/* Steps_PlanList.cpp - plan list step group                                  */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

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
void Task::vPlanListAddPlan(MP_Task mpTask)
{
  ENTRY("vPlanListAddPlan"_Str);

  Object* pPtrPlan;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrPlan) )
    THROW_INVALID_DSTACK("planList.addPlan"_Str);
  
  Plan* pPlan = _pPlan(pPtrPlan);
  
  gpPlanList->vAppend(pPlan);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* planList.findPlan -- find plan in list ( planId -- plan true | false )     */
/*----------------------------------------------------------------------------*/
void Task::vPlanListFindPlan(MP_Task mpTask)
{
  ENTRY("vPlanListFindPlan"_Str);
  
  i8 iPlanId;
  
  if ( ! mpTask->m_pDataStack->bPop(iPlanId) )
    THROW_INVALID_DSTACK("planList.findPlan"_Str);
  
  Plan* pPlan = gpPlanList->pFind((u8)iPlanId);
  
  if (pPlan) {
    mpTask->m_pDataStack->vPushObject(pPlan);
    mpTask->m_pDataStack->vPushInt((i8)1);
  }
  else {
    mpTask->m_pDataStack->vPushInt((i8)0);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* planList.removePlan -- remove a plan from plan list ( plan -- )            */
/*----------------------------------------------------------------------------*/
void Task::vPlanListRemovePlan(MP_Task mpTask)
{
  ENTRY("vPlanListRemovePlan"_Str);
  
  Object* pPtrPlan;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrPlan) )
    THROW_INVALID_DSTACK("planList.removePlan"_Str);
  
  Plan* pPlan = _pPlan(pPtrPlan);
  
  gpPlanList->vRemove(pPlan);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* planList.show -- show plan list ( plan -- )                                */
/*----------------------------------------------------------------------------*/
void Task::vPlanListShow(MP_Task mpTask)
{
  ENTRY("vPlanListShow"_Str);
  
  gpPlanList->vShow();
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
