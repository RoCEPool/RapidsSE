/*============================================================================*/
/* Steps_Plan.cpp - plan step group                                           */
/*============================================================================*/

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Plan(void)
{
  vAddStep("plan::new"_Stz,           &Step::vPlanNew);
  
  vAddStep("plan.delete"_Stz,         &Step::vPlanDelete);
  vAddStep("plan.dic"_Stz,            &Step::vPlanDic);
  vAddStep("plan.executingTask@"_Stz, &Step::vPlanExecutingTaskFetch);
  vAddStep("plan.executingTask!"_Stz, &Step::vPlanExecutingTaskStore);
  vAddStep("plan.finished"_Stz,       &Step::vPlanFinished);
  vAddStep("plan.id@"_Stz,            &Step::vPlanIdFetch);
  vAddStep("plan.lambda+"_Stz,        &Step::vPlanLambdaAppend);
  vAddStep("plan.lambda@"_Stz,        &Step::vPlanLambdaFetch);
  vAddStep("plan.lambda!"_Stz,        &Step::vPlanLambdaStore);
  vAddStep("plan.name@"_Stz,          &Step::vPlanNameFetch);
  vAddStep("plan.name!"_Stz,          &Step::vPlanNameStore);
  vAddStep("plan.opFrame++"_Stz,      &Step::vPlanOpFrameAdd);
  vAddStep("plan.opFrame?"_Stz,       &Step::vPlanOpFrameFind);
  vAddStep("plan.opFrame0"_Stz,       &Step::vPlanOpFrameTop);
  vAddStep("plan.show"_Stz,           &Step::vPlanShow);
}

/*----------------------------------------------------------------------------*/
/* plan.delete -- delete a plan ( plan -- )                                   */
/*----------------------------------------------------------------------------*/
void Step::vPlanDelete(MP_Task mpTask)
{
  ENTRY("vPlanDelete"_Stz);
  
  Object* pPtrPlan;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrPlan) )
    THROW_INVALID_DSTACK("plan.delete"_Stz);
  
  Plan* pPlan = _pPlan(pPtrPlan);
  
  delete pPlan;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* plan.dic -- get plan dic ( plan -- dic )                                   */
/*----------------------------------------------------------------------------*/
void Step::vPlanDic(MP_Task mpTask)
{
  ENTRY("vPlanDic"_Stz);
  
  Object* pPtrPlan;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrPlan) )
    THROW_INVALID_DSTACK("plan.dic"_Stz);
  
  Plan* pPlan = _pPlan(pPtrPlan);
  
  Dic* pPlanDic = pPlan->pDic();
  
  mpTask->m_pDataStack->vPushPointer(pPlanDic);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* plan.executingTask@ -- fetch task executing the plan                       */
/*----------------------------------------------------------------------------*/
void Step::vPlanExecutingTaskFetch(MP_Task mpTask)
{
  ENTRY("vPlanExecutingTaskFetch"_Stz);
  
  Object* pPtrPlan;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrPlan) )
    THROW_INVALID_DSTACK("plan.executingTask@"_Stz);
  
  Plan* pPlan = _pPlan(pPtrPlan);
  
  Task* pTask = pPlan->pExecutingTask();
  
  mpTask->m_pDataStack->vPushPointer(pTask);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* plan.executingTask! -- store the executingTask in the plan                 */
/*----------------------------------------------------------------------------*/
void Step::vPlanExecutingTaskStore(MP_Task mpTask)
{
  ENTRY("vPlanExecuteTaskStore"_Stz);
  
  Object* pPtrPlan;
  Object* pPtrTask;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrPlan) )
    THROW_INVALID_DSTACK("plan.executingTask"_Stz);
  Plan* pPlan = _pPlan(pPtrPlan);
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrTask) )
    THROW_INVALID_DSTACK("plan.executingTask"_Stz);
  Task* pTask = _pTask(pPtrTask);
  
  pPlan->vExecutingTask(pTask);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* plan.finished -- planning has finished ( plan -- )                         */
/*----------------------------------------------------------------------------*/
void Step::vPlanFinished(MP_Task mpTask)
{
  ENTRY("vPlanFinished"_Stz);
  
  Object* pPtrPlan;
  if ( ! mpTask->m_pDataStack->bPop(pPtrPlan) )
    THROW_INVALID_DSTACK("plan.dic"_Stz);
  
  Plan* pPlan = _pPlan(pPtrPlan);
  
  pPlan->vFinished();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* plan.id@ -- fetch plan id ( plan-- id )                                    */
/*----------------------------------------------------------------------------*/
void Step::vPlanIdFetch(MP_Task mpTask)
{
  ENTRY("vPlanIdFetch"_Stz);
  
  Object* pPtrPlan;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrPlan) )
    THROW_INVALID_DSTACK("plan.id@"_Stz);
  
  Plan* pPlan = _pPlan(pPtrPlan);
  
  i8 iId = pPlan->iPlanId();
  
  mpTask->m_pDataStack->vPushI8(iId);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* plan.lambda+ -- append to the lambda of the plan                           */
/*----------------------------------------------------------------------------*/
void Step::vPlanLambdaAppend(MP_Task mpTask)
{
  ENTRY("vPlanLambdaAppend"_Stz);
  
  Object* pPtrPlan;
  s48K sCode;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrPlan) )
    THROW_INVALID_DSTACK("plan.lambda+"_Stz);
  
  if ( ! mpTask->m_pDataStack->bPop(sCode) )
    THROW_INVALID_DSTACK("plan.lambda+"_Stz);
  
  Plan* pPlan = _pPlan(pPtrPlan);
  pPlan->vAppendToLambda(sCode);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* plan.lambda@ -- fetch lambda from the plan                                 */
/*----------------------------------------------------------------------------*/
void Step::vPlanLambdaFetch(MP_Task mpTask)
{
  ENTRY("vPlanLambdaFetch"_Stz);
  
  Object* pPtrPlan;
  s48K sCode;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrPlan) )
    THROW_INVALID_DSTACK("plan.lambda!"_Stz);
  
  Plan* pPlan = _pPlan(pPtrPlan);
  s48K sLambda = pPlan->sLambda();
  
  mpTask->m_pDataStack->vPushS48K(sLambda);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* plan.lambda! -- store lambda of the plan                                   */
/*----------------------------------------------------------------------------*/
void Step::vPlanLambdaStore(MP_Task mpTask)
{
  ENTRY("vPlanLambdaStore"_Stz);
  
  Object* pPtrPlan;
  s48K sCode;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrPlan) )
    THROW_INVALID_DSTACK("plan.lambda!"_Stz);
  
  if ( ! mpTask->m_pDataStack->bPop(sCode) )
    THROW_INVALID_DSTACK("plan.lambda!"_Stz);
  
  Plan* pPlan = _pPlan(pPtrPlan);
  pPlan->vLambda(sCode);
  
  EXIT();
}
/*----------------------------------------------------------------------------*/
/* plan.name@ -- fetch plan name ( plan-- "<planNm>" )                        */
/*----------------------------------------------------------------------------*/
void Step::vPlanNameFetch(MP_Task mpTask)
{
  ENTRY("vPlanNameFetch"_Stz);
  
  Object* pPtrPlan;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrPlan) )
    THROW_INVALID_DSTACK("plan.name@"_Stz);
  
  Plan* pPlan = _pPlan(pPtrPlan);
  
  s32 sName = pPlan->sName();
  
  mpTask->m_pDataStack->vPushS32(sName);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* plan.name! -- store plan name ( "<planNm>" plan -- )                       */
/*----------------------------------------------------------------------------*/
void Step::vPlanNameStore(MP_Task mpTask)
{
  ENTRY("vPlanNameStore"_Stz);
  
  Object* pPtrPlan;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrPlan) )
    THROW_INVALID_DSTACK("plan.name!"_Stz);
  
  Plan* pPlan = _pPlan(pPtrPlan);
  
  s32 sName;
  
  if ( ! mpTask->m_pDataStack->bPop(sName) )
    THROW_INVALID_DSTACK("plan.name!"_Stz);
  
  pPlan->vName(sName);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* plan::new -- new a plan ( -- plan )                                         */
/*----------------------------------------------------------------------------*/
void Step::vPlanNew(MP_Task mpTask)
{
  ENTRY("vPlanNew"_Stz);
  
  Plan* pPlan = new Plan;
  
  mpTask->m_pDataStack->vPushPointer(pPlan);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* plan.opFrame++ -- add named op frame into plan ( plan -- opFrame )         */
/*----------------------------------------------------------------------------*/
void Step::vPlanOpFrameAdd(MP_Task mpTask)
{
  ENTRY("vPlanOpFrameAdd"_Stz);
  
  Object* pPtrPlan;
  if ( ! mpTask->m_pDataStack->bPop(pPtrPlan) )
    THROW_INVALID_DSTACK("plan.opFrame++"_Stz);
  Plan* pPlan = _pPlan(pPtrPlan);
  
  s32 sNameFrame;
  if ( ! mpTask->m_pDataStack->bPop(sNameFrame) )
    THROW_INVALID_DSTACK("plan.opFrame++"_Stz);
  
  RegFrame* pRegFrame = pPlan->pRegFrameFind(sNameFrame);
  if ( pRegFrame )
    THROW("plan.opFrame++: op frame %s already exists"_Stz,sNameFrame.aChar());
  
  pRegFrame = new RegFrame;
  pRegFrame->vName(sNameFrame);
  pPlan->vRegFramePush(pRegFrame);
  
  // create a op dictionary
  s32 sNameDic;
  sNameDic.vSprint("<%s>"_Stz,sNameFrame.aChar());
  
  Dic* pOpDic = new Dic;
  pOpDic->vName(sNameDic);
  pRegFrame->vOpDic(pOpDic);
  
  // push regFrame onto dstack
  mpTask->m_pDataStack->vPushPointer(pRegFrame);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* plan.opFrame -- find named op frame in plan ( plan -- opFrame )            */
/*----------------------------------------------------------------------------*/
void Step::vPlanOpFrameFind(MP_Task mpTask)
{
  ENTRY("vPlanOpFrameFind"_Stz);
  
  
  Object* pPtrPlan;
  if ( ! mpTask->m_pDataStack->bPop(pPtrPlan) )
    THROW_INVALID_DSTACK("plan.opFrame?"_Stz);
  Plan* pPlan = _pPlan(pPtrPlan);
  
  s32 sName;
  if ( ! mpTask->m_pDataStack->bPop(sName) )
    THROW_INVALID_DSTACK("plan.opFrame?"_Stz);
  
  if (mpTask->m_iTracing) {
    vTrace("Find frame %s\n"_Stz,sName.aChar());
    i8 i = 0;
  }
  
  RegFrame* pRegFrame = pPlan->pRegFrameFind(sName);
  
  mpTask->m_pDataStack->vPushPointer(pRegFrame);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* plan.opFrame0 -- push top op frame in plan ( plan -- opFrame )             */
/*----------------------------------------------------------------------------*/
void Step::vPlanOpFrameTop(MP_Task mpTask)
{
  ENTRY("vPlanOpFrameTop"_Stz);
  
  Object* pPtrPlan;
  if ( ! mpTask->m_pDataStack->bPop(pPtrPlan) )
    THROW_INVALID_DSTACK("plan.opFrame0"_Stz);
  Plan* pPlan = _pPlan(pPtrPlan);
    
  RegFrame* pRegFrame = pPlan->pRegFrameTop();
  
  mpTask->m_pDataStack->vPushPointer(pRegFrame);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* plan.show -- show plan ( -- plan )                                         */
/*----------------------------------------------------------------------------*/
void Step::vPlanShow(MP_Task mpTask)
{
  ENTRY("vPlanShow"_Stz);
  
  Object* pPtrPlan;
  
  if ( ! mpTask->m_pDataStack->bPop(pPtrPlan) )
    THROW_INVALID_DSTACK("plan.show"_Stz);
  
  Plan* pPlan = _pPlan(pPtrPlan);
  
  pPlan->vShow();
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
