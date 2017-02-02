/*============================================================================*/
/* Steps_Plan.cpp - plan step group                                           */
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

// plan: ///////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Plan(void)
{
  vAddPrimitive("plan.delete",       &vPlanDelete,       false);
  vAddPrimitive("plan.dic",          &vPlanDic,          false);
  vAddPrimitive("plan.lambda+",      &vPlanLambdaAppend, false);
  vAddPrimitive("plan.lambda@",      &vPlanLambdaFetch,  false);
  vAddPrimitive("plan.lambda!",      &vPlanLambdaStore,  false);
  vAddPrimitive("plan.name@",        &vPlanNameFetch,    false);
  vAddPrimitive("plan.name!",        &vPlanNameStore,    false);
  vAddPrimitive("plan::new",         &vPlanNew,          false);
  vAddPrimitive("plan.regFrame++",   &vPlanRegFrameAdd,  false);
  vAddPrimitive("plan.regFrame?",    &vPlanRegFrameFind, false);
  vAddPrimitive("plan.show",         &vPlanShow,         false);
}

/*----------------------------------------------------------------------------*/
/* plan.delete -- delete a plan ( plan -- )                                   */
/*----------------------------------------------------------------------------*/
void Task::vPlanDelete(NNP_Task nnpTask)
{
  ENTRY("vPlanDelete");

  Object* pPtrPlan;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrPlan) )
    THROW_INVALID_DSTACK("plan.delete");
  
  Plan* pPlan = _pPlan(pPtrPlan);
  
  delete pPlan;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* plan.dic -- get plan dic ( plan -- dic )                                   */
/*----------------------------------------------------------------------------*/
void Task::vPlanDic(NNP_Task nnpTask)
{
  ENTRY("vPlanDic");
  
  Object* pPtrPlan;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrPlan) )
    THROW_INVALID_DSTACK("plan.dic");
  
  Plan* pPlan = _pPlan(pPtrPlan);
  
  Dic* pPlanDic = pPlan->pDic();
  
  nnpTask->m_pDataStack->vPush(pPlanDic);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* plan.lambda+ -- append to the lambda of the plan                           */
/*----------------------------------------------------------------------------*/
void Task::vPlanLambdaAppend(NNP_Task nnpTask)
{
  ENTRY("vPlanLambdaAppend");
  
  Object* pPtrPlan;
  s48K sCode;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrPlan) )
    THROW_INVALID_DSTACK("plan.lambda+");
  
  if ( ! nnpTask->m_pDataStack->bPop(sCode) )
    THROW_INVALID_DSTACK("plan.lambda+");
  
  Plan* pPlan = _pPlan(pPtrPlan);
  pPlan->vAppendToLambda(sCode);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* plan.lambda@ -- fetch lambda from the plan                                 */
/*----------------------------------------------------------------------------*/
void Task::vPlanLambdaFetch(NNP_Task nnpTask)
{
  ENTRY("vPlanLambdaFetch");
  
  Object* pPtrPlan;
  s48K sCode;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrPlan) )
    THROW_INVALID_DSTACK("plan.lambda!");

  Plan* pPlan = _pPlan(pPtrPlan);
  s48K sLambda = pPlan->sLambda();
  
  nnpTask->m_pDataStack->vPush(sLambda);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* plan.lambda! -- store lambda of the plan                                   */
/*----------------------------------------------------------------------------*/
void Task::vPlanLambdaStore(NNP_Task nnpTask)
{
  ENTRY("vPlanLambdaStore");
  
  Object* pPtrPlan;
  s48K sCode;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrPlan) )
    THROW_INVALID_DSTACK("plan.lambda!");
  
  if ( ! nnpTask->m_pDataStack->bPop(sCode) )
    THROW_INVALID_DSTACK("plan.lambda!");
  
  Plan* pPlan = _pPlan(pPtrPlan);
  pPlan->vLambda(sCode);
  
  EXIT();
}
/*----------------------------------------------------------------------------*/
/* plan.name@ -- fetch plan name ( plan-- "<planNm>" )                        */
/*----------------------------------------------------------------------------*/
void Task::vPlanNameFetch(NNP_Task nnpTask)
{
  ENTRY("vPlanNameFetch");
  
  Object* pPtrPlan;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrPlan) )
    THROW_INVALID_DSTACK("plan.name@");
  
  Plan* pPlan = _pPlan(pPtrPlan);
  
  s32 sName = pPlan->sName();
  
  nnpTask->m_pDataStack->vPush(sName);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* plan.name! -- store plan name ( "<planNm>" plan -- )                       */
/*----------------------------------------------------------------------------*/
void Task::vPlanNameStore(NNP_Task nnpTask)
{
  ENTRY("vPlanNameStore");
  
  Object* pPtrPlan;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrPlan) )
    THROW_INVALID_DSTACK("plan.name!");
  
  Plan* pPlan = _pPlan(pPtrPlan);
  
  s32 sName;
  
  if ( ! nnpTask->m_pDataStack->bPop(sName) )
    THROW_INVALID_DSTACK("plan.name!");
  
  pPlan->vName(sName);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* plan.new -- new a plan ( -- plan )                                         */
/*----------------------------------------------------------------------------*/
void Task::vPlanNew(NNP_Task nnpTask)
{
  ENTRY("vPlanNew");
  
  Plan* pPlan = new Plan;
    
  nnpTask->m_pDataStack->vPush(pPlan);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* plan.frame++ -- find/create reg frame in plan ( plan -- regFrame )         */
/*----------------------------------------------------------------------------*/
void Task::vPlanRegFrameAdd(NNP_Task nnpTask)
{
  ENTRY("vPlanRegFrameAdd");
  
  Object* pPtrPlan;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrPlan) )
    THROW_INVALID_DSTACK("plan.regFrame++");
  Plan* pPlan = _pPlan(pPtrPlan);
  
  s32 sName;
  if ( ! nnpTask->m_pDataStack->bPop(sName) )
    THROW_INVALID_DSTACK("plan.regFrame++");
  
  RegFrame* pRegFrame = pPlan->pRegFrameFind(sName);
  if ( pRegFrame )
  THROW("plan.regFrame++: op frame %s already exists",sName.a());

  pRegFrame = new RegFrame;
  pRegFrame->vName(sName);
  pPlan->vRegFramePush(pRegFrame);

  nnpTask->m_pDataStack->vPush(pRegFrame);
 
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* plan.regFrame -- find reg frame in plan ( plan -- regFrame )               */
/*----------------------------------------------------------------------------*/
void Task::vPlanRegFrameFind(NNP_Task nnpTask)
{
  ENTRY("vPlanRegFrameFind");
  
  Object* pPtrPlan;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrPlan) )
  THROW_INVALID_DSTACK("plan.regFrame?");
  Plan* pPlan = _pPlan(pPtrPlan);
  
  s32 sName;
  if ( ! nnpTask->m_pDataStack->bPop(sName) )
  THROW_INVALID_DSTACK("plan.regFrame?");
  
  RegFrame* pRegFrame = pPlan->pRegFrameFind(sName);
  
  nnpTask->m_pDataStack->vPush(pRegFrame);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* plan.show -- show plan ( -- plan )                                         */
/*----------------------------------------------------------------------------*/
void Task::vPlanShow(NNP_Task nnpTask)
{
  ENTRY("vPlanShow");
  
  Object* pPtrPlan;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrPlan) )
    THROW_INVALID_DSTACK("plan.show");
  
  Plan* pPlan = _pPlan(pPtrPlan);
  
  pPlan->vShow();
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
