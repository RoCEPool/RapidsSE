/*============================================================================*/
/* Steps_PlanList.cpp - plan list step group                                  */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern AttachMap*      gpAttachMap;

// plan list: //////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddSteps_PlanList(void)
{
  vAddStep("planList::addPlan"_Stz,    &vPlanListAddPlan,    false);
  vAddStep("planList::findPlan"_Stz,   &vPlanListFindPlan,   false);
  vAddStep("planList::removePlan"_Stz, &vPlanListRemovePlan, false);
  vAddStep("planList::show"_Stz,       &vPlanListShow,       false);
}

/*----------------------------------------------------------------------------*/
/* planList::addPlan -- add a plan to plan list ( plan -- )                    */
/*----------------------------------------------------------------------------*/
void Task::vPlanListAddPlan(MP_Task mpTask)
{
  ENTRY("vPlanListAddPlan"_Stz);

  Object* pPtrPlan;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrPlan) )
    THROW_INVALID_DSTACK("planList.addPlan"_Stz);
  
  Plan* pPlan = _pPlan(pPtrPlan);
  
  gpAttachMap->m_pPlanList->vAppend(pPlan);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* planList::findPlan -- find plan in list ( planId -- plan true | false )     */
/*----------------------------------------------------------------------------*/
void Task::vPlanListFindPlan(MP_Task mpTask)
{
  ENTRY("vPlanListFindPlan"_Stz);
  
  i8 iPlanId;
  
  if ( ! mpTask->m_pDataStack->bPop(iPlanId) )
    THROW_INVALID_DSTACK("planList.findPlan"_Stz);
  
  Plan* pPlan = gpAttachMap->m_pPlanList->pFind((u8)iPlanId);
  
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
/* planList::removePlan -- remove a plan from plan list ( plan -- )            */
/*----------------------------------------------------------------------------*/
void Task::vPlanListRemovePlan(MP_Task mpTask)
{
  ENTRY("vPlanListRemovePlan"_Stz);
  
  Object* pPtrPlan;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrPlan) )
    THROW_INVALID_DSTACK("planList.removePlan"_Stz);
  
  Plan* pPlan = _pPlan(pPtrPlan);
  
  gpAttachMap->m_pPlanList->vRemove(pPlan);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* planList::show -- show plan list ( plan -- )                                */
/*----------------------------------------------------------------------------*/
void Task::vPlanListShow(MP_Task mpTask)
{
  ENTRY("vPlanListShow"_Stz);
  
  gpAttachMap->m_pPlanList->vShow();
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
