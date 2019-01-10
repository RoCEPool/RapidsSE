/*============================================================================*/
/* Steps_PlanList.cpp - plan list step group                                  */
/*============================================================================*/

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_PlanList(void)
{
  vAddStep("planList::addPlan"_Stz,    &Step::vPlanListAddPlan);
  vAddStep("planList::findPlan"_Stz,   &Step::vPlanListFindPlan);
  vAddStep("planList::removePlan"_Stz, &Step::vPlanListRemovePlan);
  vAddStep("planList::show"_Stz,       &Step::vPlanListShow);
}

/*----------------------------------------------------------------------------*/
/* planList::addPlan -- add a plan to plan list ( plan -- )                    */
/*----------------------------------------------------------------------------*/
void Step::vPlanListAddPlan(MP_Task mpTask)
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
void Step::vPlanListFindPlan(MP_Task mpTask)
{
  ENTRY("vPlanListFindPlan"_Stz);
  
  i8 iPlanId;
  
  if ( ! mpTask->m_pDataStack->bPop(iPlanId) )
    THROW_INVALID_DSTACK("planList.findPlan"_Stz);
  
  Plan* pPlan = gpAttachMap->m_pPlanList->pFind((u8)iPlanId);
  
  if (pPlan) {
    mpTask->m_pDataStack->vPushObject(pPlan);
    mpTask->m_pDataStack->vPushBool(true);
  }
  else {
    mpTask->m_pDataStack->vPushBool(false);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* planList::removePlan -- remove a plan from plan list ( plan -- )            */
/*----------------------------------------------------------------------------*/
void Step::vPlanListRemovePlan(MP_Task mpTask)
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
void Step::vPlanListShow(MP_Task mpTask)
{
  ENTRY("vPlanListShow"_Stz);
  
  gpAttachMap->m_pPlanList->vShow();
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
