/*============================================================================*/
/* Steps_Plan.cpp - plan step group                                           */
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
  vAddPrimitive("plan.delete"_Str,         &vPlanDelete,             false);
  vAddPrimitive("plan.dic"_Str,            &vPlanDic,                false);
  vAddPrimitive("plan.executingTask@"_Str, &vPlanExecutingTaskFetch, false);
  vAddPrimitive("plan.executingTask!"_Str, &vPlanExecutingTaskStore, false);
  vAddPrimitive("plan.finished"_Str,       &vPlanFinished,           false);
  vAddPrimitive("plan.id@"_Str,            &vPlanIdFetch,            false);
  vAddPrimitive("plan.lambda+"_Str,        &vPlanLambdaAppend,       false);
  vAddPrimitive("plan.lambda@"_Str,        &vPlanLambdaFetch,        false);
  vAddPrimitive("plan.lambda!"_Str,        &vPlanLambdaStore,        false);
  vAddPrimitive("plan.name@"_Str,          &vPlanNameFetch,          false);
  vAddPrimitive("plan.name!"_Str,          &vPlanNameStore,          false);
  vAddPrimitive("plan::new"_Str,           &vPlanNew,                false);
  vAddPrimitive("plan.opFrame++"_Str,      &vPlanOpFrameAdd,         false);
  vAddPrimitive("plan.opFrame?"_Str,       &vPlanOpFrameFind,        false);
  vAddPrimitive("plan.opFrame0"_Str,       &vPlanOpFrameTop,         false);
  vAddPrimitive("plan.show"_Str,           &vPlanShow,               false);
}

/*----------------------------------------------------------------------------*/
/* plan.delete -- delete a plan ( plan -- )                                   */
/*----------------------------------------------------------------------------*/
void Task::vPlanDelete(NNP_Task nnpTask)
{
  ENTRY("vPlanDelete"_Str);
  
  Object* pPtrPlan;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrPlan) )
    THROW_INVALID_DSTACK("plan.delete"_Str);
  
  Plan* pPlan = _pPlan(pPtrPlan);
  
  delete pPlan;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* plan.dic -- get plan dic ( plan -- dic )                                   */
/*----------------------------------------------------------------------------*/
void Task::vPlanDic(NNP_Task nnpTask)
{
  ENTRY("vPlanDic"_Str);
  
  Object* pPtrPlan;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrPlan) )
    THROW_INVALID_DSTACK("plan.dic"_Str);
  
  Plan* pPlan = _pPlan(pPtrPlan);
  
  Dic* pPlanDic = pPlan->pDic();
  
  nnpTask->m_pDataStack->vPushObject(pPlanDic);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* plan.executingTask@ -- fetch task executing the plan                       */
/*----------------------------------------------------------------------------*/
void Task::vPlanExecutingTaskFetch(NNP_Task nnpTask)
{
  ENTRY("vPlanExecutingTaskFetch"_Str);
  
  Object* pPtrPlan;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrPlan) )
    THROW_INVALID_DSTACK("plan.executingTask@"_Str);
  
  Plan* pPlan = _pPlan(pPtrPlan);
  
  Task* pTask = pPlan->pExecutingTask();
  
  nnpTask->m_pDataStack->vPushObject(pTask);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* plan.executingTask! -- store the executingTask in the plan                 */
/*----------------------------------------------------------------------------*/
void Task::vPlanExecutingTaskStore(NNP_Task nnpTask)
{
  ENTRY("vPlanExecuteTaskStore"_Str);
  
  Object* pPtrPlan;
  Object* pPtrTask;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrPlan) )
    THROW_INVALID_DSTACK("plan.executingTask"_Str);
  Plan* pPlan = _pPlan(pPtrPlan);
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrTask) )
    THROW_INVALID_DSTACK("plan.executingTask"_Str);
  Task* pTask = _pTask(pPtrTask);
  
  pPlan->vExecutingTask(pTask);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* plan.finished -- planning has finished ( plan -- )                         */
/*----------------------------------------------------------------------------*/
void Task::vPlanFinished(NNP_Task nnpTask)
{
  ENTRY("vPlanFinished"_Str);
  
  Object* pPtrPlan;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrPlan) )
    THROW_INVALID_DSTACK("plan.dic"_Str);
  
  Plan* pPlan = _pPlan(pPtrPlan);
  
  pPlan->vFinished();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* plan.id@ -- fetch plan id ( plan-- id )                                    */
/*----------------------------------------------------------------------------*/
void Task::vPlanIdFetch(NNP_Task nnpTask)
{
  ENTRY("vPlanIdFetch"_Str);
  
  Object* pPtrPlan;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrPlan) )
    THROW_INVALID_DSTACK("plan.id@"_Str);
  
  Plan* pPlan = _pPlan(pPtrPlan);
  
  i8 iId = pPlan->iPlanId();
  
  nnpTask->m_pDataStack->vPushInt(iId);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* plan.lambda+ -- append to the lambda of the plan                           */
/*----------------------------------------------------------------------------*/
void Task::vPlanLambdaAppend(NNP_Task nnpTask)
{
  ENTRY("vPlanLambdaAppend"_Str);
  
  Object* pPtrPlan;
  s48K sCode;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrPlan) )
    THROW_INVALID_DSTACK("plan.lambda+"_Str);
  
  if ( ! nnpTask->m_pDataStack->bPop(sCode) )
    THROW_INVALID_DSTACK("plan.lambda+"_Str);
  
  Plan* pPlan = _pPlan(pPtrPlan);
  pPlan->vAppendToLambda(sCode);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* plan.lambda@ -- fetch lambda from the plan                                 */
/*----------------------------------------------------------------------------*/
void Task::vPlanLambdaFetch(NNP_Task nnpTask)
{
  ENTRY("vPlanLambdaFetch"_Str);
  
  Object* pPtrPlan;
  s48K sCode;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrPlan) )
    THROW_INVALID_DSTACK("plan.lambda!"_Str);
  
  Plan* pPlan = _pPlan(pPtrPlan);
  s48K sLambda = pPlan->sLambda();
  
  nnpTask->m_pDataStack->vPushS48K(sLambda);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* plan.lambda! -- store lambda of the plan                                   */
/*----------------------------------------------------------------------------*/
void Task::vPlanLambdaStore(NNP_Task nnpTask)
{
  ENTRY("vPlanLambdaStore"_Str);
  
  Object* pPtrPlan;
  s48K sCode;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrPlan) )
    THROW_INVALID_DSTACK("plan.lambda!"_Str);
  
  if ( ! nnpTask->m_pDataStack->bPop(sCode) )
    THROW_INVALID_DSTACK("plan.lambda!"_Str);
  
  Plan* pPlan = _pPlan(pPtrPlan);
  pPlan->vLambda(sCode);
  
  EXIT();
}
/*----------------------------------------------------------------------------*/
/* plan.name@ -- fetch plan name ( plan-- "<planNm>" )                        */
/*----------------------------------------------------------------------------*/
void Task::vPlanNameFetch(NNP_Task nnpTask)
{
  ENTRY("vPlanNameFetch"_Str);
  
  Object* pPtrPlan;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrPlan) )
    THROW_INVALID_DSTACK("plan.name@"_Str);
  
  Plan* pPlan = _pPlan(pPtrPlan);
  
  s32 sName = pPlan->sName();
  
  nnpTask->m_pDataStack->vPushS32(sName);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* plan.name! -- store plan name ( "<planNm>" plan -- )                       */
/*----------------------------------------------------------------------------*/
void Task::vPlanNameStore(NNP_Task nnpTask)
{
  ENTRY("vPlanNameStore"_Str);
  
  Object* pPtrPlan;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrPlan) )
    THROW_INVALID_DSTACK("plan.name!"_Str);
  
  Plan* pPlan = _pPlan(pPtrPlan);
  
  s32 sName;
  
  if ( ! nnpTask->m_pDataStack->bPop(sName) )
    THROW_INVALID_DSTACK("plan.name!"_Str);
  
  pPlan->vName(sName);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* plan::new -- new a plan ( -- plan )                                         */
/*----------------------------------------------------------------------------*/
void Task::vPlanNew(NNP_Task nnpTask)
{
  ENTRY("vPlanNew"_Str);
  
  Plan* pPlan = new Plan;
  
  nnpTask->m_pDataStack->vPushObject(pPlan);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* plan.opFrame++ -- add named op frame into plan ( plan -- opFrame )         */
/*----------------------------------------------------------------------------*/
void Task::vPlanOpFrameAdd(NNP_Task nnpTask)
{
  ENTRY("vPlanOpFrameAdd"_Str);
  
  Object* pPtrPlan;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrPlan) )
    THROW_INVALID_DSTACK("plan.opFrame++"_Str);
  Plan* pPlan = _pPlan(pPtrPlan);
  
  s32 sNameFrame;
  if ( ! nnpTask->m_pDataStack->bPop(sNameFrame) )
    THROW_INVALID_DSTACK("plan.opFrame++"_Str);
  
  RegFrame* pRegFrame = pPlan->pRegFrameFind(sNameFrame);
  if ( pRegFrame )
    THROW("plan.opFrame++: op frame %s already exists"_Str,sNameFrame.s());
  
  pRegFrame = new RegFrame;
  pRegFrame->vName(sNameFrame);
  pPlan->vRegFramePush(pRegFrame);
  
  // create a op dictionary
  s32 sNameDic;
  sNameDic.vSprint("<%s>"_Str,sNameFrame.s());
  
  Dic* pOpDic = new Dic;
  pOpDic->vName(sNameDic);
  pRegFrame->vOpDic(pOpDic);
  
  // push regFrame onto dstack
  nnpTask->m_pDataStack->vPushObject(pRegFrame);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* plan.opFrame -- find named op frame in plan ( plan -- opFrame )            */
/*----------------------------------------------------------------------------*/
void Task::vPlanOpFrameFind(NNP_Task nnpTask)
{
  ENTRY("vPlanOpFrameFind"_Str);
  
  
  Object* pPtrPlan;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrPlan) )
    THROW_INVALID_DSTACK("plan.opFrame?"_Str);
  Plan* pPlan = _pPlan(pPtrPlan);
  
  s32 sName;
  if ( ! nnpTask->m_pDataStack->bPop(sName) )
    THROW_INVALID_DSTACK("plan.opFrame?"_Str);
  
  if (nnpTask->m_iTracing) {
    vTrace("Find frame %s\n"_Str,sName.s());
    i8 i = 0;
  }
  
  RegFrame* pRegFrame = pPlan->pRegFrameFind(sName);
  
  nnpTask->m_pDataStack->vPushObject(pRegFrame);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* plan.opFrame0 -- push top op frame in plan ( plan -- opFrame )             */
/*----------------------------------------------------------------------------*/
void Task::vPlanOpFrameTop(NNP_Task nnpTask)
{
  ENTRY("vPlanOpFrameTop"_Str);
  
  Object* pPtrPlan;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrPlan) )
    THROW_INVALID_DSTACK("plan.opFrame0"_Str);
  Plan* pPlan = _pPlan(pPtrPlan);
    
  RegFrame* pRegFrame = pPlan->pRegFrameTop();
  
  nnpTask->m_pDataStack->vPushObject(pRegFrame);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* plan.show -- show plan ( -- plan )                                         */
/*----------------------------------------------------------------------------*/
void Task::vPlanShow(NNP_Task nnpTask)
{
  ENTRY("vPlanShow"_Str);
  
  Object* pPtrPlan;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrPlan) )
    THROW_INVALID_DSTACK("plan.show"_Str);
  
  Plan* pPlan = _pPlan(pPtrPlan);
  
  pPlan->vShow();
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
