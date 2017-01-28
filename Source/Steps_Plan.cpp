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
  vAddPrimitive("plan.delete",    &vPlanDelete,       false);
  vAddPrimitive("plan.name@",     &vPlanNameFetch,    false);
  vAddPrimitive("plan.name!",     &vPlanNameStore,    false);
  vAddPrimitive("plan::new",      &vPlanNew,          false);
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

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
