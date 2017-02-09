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
  vAddPrimitive("plan.delete",         &vPlanDelete,             false);
  vAddPrimitive("plan.dic",            &vPlanDic,                false);
  vAddPrimitive("plan.executingTask@", &vPlanExecutingTaskFetch, false);
  vAddPrimitive("plan.executingTask!", &vPlanExecutingTaskStore, false);
  vAddPrimitive("plan.finished",       &vPlanFinished,           false);
  vAddPrimitive("plan.id@",            &vPlanIdFetch,            false);
  vAddPrimitive("plan.lambda+",        &vPlanLambdaAppend,       false);
  vAddPrimitive("plan.lambda@",        &vPlanLambdaFetch,        false);
  vAddPrimitive("plan.lambda!",        &vPlanLambdaStore,        false);
  vAddPrimitive("plan.name@",          &vPlanNameFetch,          false);
  vAddPrimitive("plan.name!",          &vPlanNameStore,          false);
  vAddPrimitive("plan::new",           &vPlanNew,                false);
  vAddPrimitive("plan.opFrame++",      &vPlanOpFrameAdd,         false);
  vAddPrimitive("plan.opFrame?",       &vPlanOpFrameFind,        false);
  vAddPrimitive("plan.opFrame0",       &vPlanOpFrameTop,         false);
  vAddPrimitive("plan.show",           &vPlanShow,               false);
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
/* plan.executingTask@ -- fetch task executing the plan                       */
/*----------------------------------------------------------------------------*/
void Task::vPlanExecutingTaskFetch(NNP_Task nnpTask)
{
  ENTRY("vPlanExecutingTaskFetch");
  
  Object* pPtrPlan;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrPlan) )
    THROW_INVALID_DSTACK("plan.executingTask@");
  
  Plan* pPlan = _pPlan(pPtrPlan);
  
  Task* pTask = pPlan->pExecutingTask();
  
  nnpTask->m_pDataStack->vPush(pTask);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* plan.executingTask! -- store the executingTask in the plan                 */
/*----------------------------------------------------------------------------*/
void Task::vPlanExecutingTaskStore(NNP_Task nnpTask)
{
  ENTRY("vPlanExecuteTaskStore");
  
  Object* pPtrPlan;
  Object* pPtrTask;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrPlan) )
    THROW_INVALID_DSTACK("plan.executingTask");
  Plan* pPlan = _pPlan(pPtrPlan);
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrTask) )
    THROW_INVALID_DSTACK("plan.executingTask");
  Task* pTask = _pTask(pPtrTask);
  
  pPlan->vExecutingTask(pTask);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* plan.finished -- planning has finished ( plan -- )                         */
/*----------------------------------------------------------------------------*/
void Task::vPlanFinished(NNP_Task nnpTask)
{
  ENTRY("vPlanFinished");
  
  Object* pPtrPlan;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrPlan) )
    THROW_INVALID_DSTACK("plan.dic");
  
  Plan* pPlan = _pPlan(pPtrPlan);
  
  pPlan->vFinished();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* plan.id@ -- fetch plan id ( plan-- id )                                    */
/*----------------------------------------------------------------------------*/
void Task::vPlanIdFetch(NNP_Task nnpTask)
{
  ENTRY("vPlanIdFetch");
  
  Object* pPtrPlan;
  
  if ( ! nnpTask->m_pDataStack->bPop(pPtrPlan) )
    THROW_INVALID_DSTACK("plan.id@");
  
  Plan* pPlan = _pPlan(pPtrPlan);
  
  u8 uId = pPlan->uPlanId();
  
  nnpTask->m_pDataStack->vPush(uId);
  
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
/* plan.opFrame++ -- add named op frame into plan ( plan -- opFrame )         */
/*----------------------------------------------------------------------------*/
void Task::vPlanOpFrameAdd(NNP_Task nnpTask)
{
  ENTRY("vPlanOpFrameAdd");
  
  Object* pPtrPlan;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrPlan) )
    THROW_INVALID_DSTACK("plan.opFrame++");
  Plan* pPlan = _pPlan(pPtrPlan);
  
  s32 sNameFrame;
  if ( ! nnpTask->m_pDataStack->bPop(sNameFrame) )
    THROW_INVALID_DSTACK("plan.opFrame++");
  
  RegFrame* pRegFrame = pPlan->pRegFrameFind(sNameFrame);
  if ( pRegFrame )
    THROW("plan.opFrame++: op frame %s already exists",sNameFrame.a());
  
  pRegFrame = new RegFrame;
  pRegFrame->vName(sNameFrame);
  pPlan->vRegFramePush(pRegFrame);
  
  // create a op dictionary
  s32 sNameDic;
  String::vSprint(sNameDic.a(),"<%s>",sNameFrame.a());
  
  Dic* pOpDic = new Dic;
  pOpDic->vName(sNameDic.a());
  pRegFrame->vOpDic(pOpDic);
  
  // push regFrame onto dstack
  nnpTask->m_pDataStack->vPush(pRegFrame);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* plan.opFrame -- find named op frame in plan ( plan -- opFrame )            */
/*----------------------------------------------------------------------------*/
void Task::vPlanOpFrameFind(NNP_Task nnpTask)
{
  ENTRY("vPlanOpFrameFind");
  
  
  Object* pPtrPlan;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrPlan) )
    THROW_INVALID_DSTACK("plan.opFrame?");
  Plan* pPlan = _pPlan(pPtrPlan);
  
  s32 sName;
  if ( ! nnpTask->m_pDataStack->bPop(sName) )
    THROW_INVALID_DSTACK("plan.opFrame?");
  
  if (nnpTask->m_iTracing) {
    vTrace("Find frame %s\n",sName.a());
    i8 i = 0;
  }
  
  RegFrame* pRegFrame = pPlan->pRegFrameFind(sName);
  
  nnpTask->m_pDataStack->vPush(pRegFrame);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* plan.opFrame0 -- push top op frame in plan ( plan -- opFrame )             */
/*----------------------------------------------------------------------------*/
void Task::vPlanOpFrameTop(NNP_Task nnpTask)
{
  ENTRY("vPlanOpFrameTop");
  
  Object* pPtrPlan;
  if ( ! nnpTask->m_pDataStack->bPop(pPtrPlan) )
    THROW_INVALID_DSTACK("plan.opFrame0");
  Plan* pPlan = _pPlan(pPtrPlan);
    
  RegFrame* pRegFrame = pPlan->pRegFrameTop();
  
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
