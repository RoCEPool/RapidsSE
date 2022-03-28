/*============================================================================*/
/* Steps_SubPipe.cpp -- SubPipe step group                                    */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "eYX.h"

#include "Grid.hpp"
#include "Op.hpp"
#include "Stream.hpp"
#include "Stage.hpp"
#include "PipeWork.hpp"
#include "Pipeline.hpp"
#include "PipeSpec.hpp"
#include "Job.hpp"
#include "Node_I.hpp"
#include "LogicUnit.hpp"

#include "Marker.hpp"

#include "SP_Pipeline.hpp"
#include "SP_PipeSpec.hpp"

#include "Dynamo.hpp"
#include "TagConstants.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_SubPipe(void)
{
  vAddStep("SubPipe[]"_Stz,                &Step::vSubPipeGet);
  vAddStep("SubPipe[].set"_Stz,            &Step::vSubPipeSet);

  vAddStep("SubPipe[].addAccum"_Stz,       &Step::vSubPipeAddAccum);
  vAddStep("SubPipe[].cancelled@"_Stz,     &Step::vSubPipeCancelledFetch);
  vAddStep("SubPipe[].cancelled!"_Stz,     &Step::vSubPipeCancelledStore);
  vAddStep("SubPipe[].create"_Stz,         &Step::vSubPipeCreate);
  vAddStep("SubPipe[].dropDebugLock"_Stz,  &Step::vSubPipeDropDebugLock);
  vAddStep("SubPipe[].dump"_Stz,           &Step::vSubPipeDump);
  vAddStep("SubPipe[].earlyExit@"_Stz,     &Step::vSubPipeEarlyExitFetch);
  vAddStep("SubPipe[].earlyExit!"_Stz,     &Step::vSubPipeEarlyExitStore);
  vAddStep("SubPipe[].enqueueStart?"_Stz,  &Step::vSubPipeEnqueueStart);
  vAddStep("SubPipe[].errored@"_Stz,       &Step::vSubPipeErroredFetch);
  vAddStep("SubPipe[].errored!"_Stz,       &Step::vSubPipeErroredStore);
  vAddStep("SubPipe[].getAccum"_Stz,       &Step::vSubPipeGetAccum);
  vAddStep("SubPipe[].grabDebugLock"_Stz,  &Step::vSubPipeGrabDebugLock);
  vAddStep("SubPipe[].op0@"_Stz,           &Step::vSubPipeOp0Fetch);
  vAddStep("SubPipe[].opsOnNode@"_Stz,     &Step::vSubPipeOpsOnNodeFetch);
  vAddStep("SubPipe[].pulsed@"_Stz,        &Step::vSubPipePulsedFetch);
  vAddStep("SubPipe[].pulsed!"_Stz,        &Step::vSubPipePulsedStore);
  vAddStep("SubPipe[].setInvoker"_Stz,     &Step::vSubPipeSetInvoker);
  vAddStep("SubPipe[].spawnOpTasks"_Stz,   &Step::vSubPipeSpawnOpTasks);
}

/*----------------------------------------------------------------------------*/
/* SubPipe[].addAccum -- add to an accumulator in this pipeline               */
/*----------------------------------------------------------------------------*/
void Step::vSubPipeAddAccum(void)
{
  ENTRY();
    
  i8 i_Accum = m_pDataStack->iPopInt(__func__);
  
  i8 iValue  = m_pDataStack->iPopInt(__func__);
  
  m_pLogicUnit->m_xpSubPipe->vAddAccum(i_Accum,iValue);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* SubPipe[].cancelled@ -- has this pipeline been cancelled?                  */
/*----------------------------------------------------------------------------*/
void Step::vSubPipeCancelledFetch(void)
{
  ENTRY();
  
  // return if pipeline has been cancelled
  m_pDataStack->vPushInt(m_pLogicUnit->m_xpSubPipe->bCancelled());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* SubPipe[].cancelled! -- fetch this pipeline's cancelled indicator          */
/*----------------------------------------------------------------------------*/
void Step::vSubPipeCancelledStore(void)
{
  ENTRY();
  
  m_pLogicUnit->m_xpSubPipe->vCancelled(m_pDataStack->iPopInt(__func__));
  
  EXIT();
}


/*----------------------------------------------------------------------------*/
/* SubPipe[].create -- create the pipeline, stages, streams and ops           */
/*----------------------------------------------------------------------------*/
void Step::vSubPipeCreate(void)
{
  ENTRY();
    
  i8 iThisNode = AM::pGrid()->iThisNodeNo();

  // get pipeline spec
  
  PipeSpec* pPipeSpec = _pPipeSpec(m_pDataStack->pPopPtr(__func__));
  
  // create subpipeline
  
  Pipeline* pSubPipeline =
    new Pipeline(m_pJob,pPipeSpec,m_pJob->iOriginatorNode());
  
  m_pLogicUnit->m_xpSubPipe = pSubPipeline;
    
  // dump pipeline
  
  if (AM::TRACE::bVerbose(C_VB_PIPELINE)) {
    if (iThisNode == 0) {
      SP_Pipeline spPipeline(pSubPipeline,true);
      vTraceM("%s\n"_Stz,spPipeline.sz());
      vDebugPoint();
    }
  }
    
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* SubPipe[].dropDebugLock -- drop this pipeline's debug lock                 */
/*----------------------------------------------------------------------------*/
void Step::vSubPipeDropDebugLock(void)
{
  ENTRY();
    
  m_pLogicUnit->m_xpSubPipe->vDropDebugLock();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* SubPipe[].dump -- dump this subpipeline only                               */
/*----------------------------------------------------------------------------*/
void Step::vSubPipeDump(void)
{
  ENTRY();
      
  SP_Pipeline spSubPipe;
  
  vTrace("%s\n"_Stz,spSubPipe.sz(* m_pLogicUnit->m_xpSubPipe,true));

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* SubPipe[].earlyExit@ -- fetch this pipelines earlyExit indicator           */
/*----------------------------------------------------------------------------*/
void Step::vSubPipeEarlyExitFetch(void)
{
  ENTRY();

  // return if pipeline has been errored
  m_pDataStack->vPushInt(m_pLogicUnit->m_xpSubPipe->bEarlyExit());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* SubPipe[].earlyExit! -- store this pipeline's earlyExit indicator          */
/*----------------------------------------------------------------------------*/
void Step::vSubPipeEarlyExitStore(void)
{
  ENTRY();

  m_pLogicUnit->m_xpSubPipe->vEarlyExit(m_pDataStack->iPopIntBol(__func__));

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* SubPipe[].enqueueStart? -- test for issuing a start marker                 */
/*----------------------------------------------------------------------------*/
void Step::vSubPipeEnqueueStart(void)
{
  ENTRY();
  
  PipeWork* pFormWork = m_pLogicUnit->m_xpSubPipe->pPipeWork();

  Stage*    pStage0   = pFormWork->pGet(0);

  if (pStage0->bStartPipeline()) {
    
    m_pDataStack->vPushInt(true);
  }
  else {
    m_pDataStack->vPushInt(false);
  }

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* SubPipe[].errored@ -- fetch this pipeline errored indicator                */
/*----------------------------------------------------------------------------*/
void Step::vSubPipeErroredFetch(void)
{
  ENTRY();
    
  // return if pipeline has been errored
  m_pDataStack->vPushInt(m_pLogicUnit->m_xpSubPipe->bErrored());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* SubPipe[].errored! -- store this pipeline errored indicator                */
/*----------------------------------------------------------------------------*/
void Step::vSubPipeErroredStore(void)
{
  ENTRY();
  
  m_pLogicUnit->m_xpSubPipe->vErrored(m_pDataStack->iPopBol(__func__));
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* SubPipe[].get -- get subpipeline                                           */
/*----------------------------------------------------------------------------*/
void Step::vSubPipeGet(void)
{
  ENTRY();
   
  if (m_pLogicUnit->m_xpSubPipe.bIsNull())
    m_pDataStack->vPushPtr(nullptr);
  else
    m_pDataStack->vPushPtr(* m_pLogicUnit->m_xpSubPipe);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* SubPipe[].getAccum -- get an accumulator from this pipeline                */
/*----------------------------------------------------------------------------*/
void Step::vSubPipeGetAccum(void)
{
  ENTRY();
   
  i8 i_Accum = m_pDataStack->iPopInt(__func__);
  
  i8 iValue = m_pLogicUnit->m_xpSubPipe->iGetAccum(i_Accum);
  
  m_pDataStack->vPushInt(iValue);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* SubPipe[].grabDebugLock -- grab this pipeline's debug lock                 */
/*----------------------------------------------------------------------------*/
void Step::vSubPipeGrabDebugLock(void)
{
  ENTRY();
    
  m_pLogicUnit->m_xpSubPipe->vGrabDebugLock();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* SubPipe[].pulsed@ -- fetch this pipeline's pulsed indicator                */
/*----------------------------------------------------------------------------*/
void Step::vSubPipePulsedFetch(void)
{
  ENTRY();
    
  // return if pipeline has been pulsed
  m_pDataStack->vPushInt(m_pLogicUnit->m_xpSubPipe->bPulsed());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* SubPipe[].pulsed! -- store this pipeline's pulsed indicator                */
/*----------------------------------------------------------------------------*/
void Step::vSubPipePulsedStore(void)
{
  ENTRY();
    
  m_pLogicUnit->m_xpSubPipe->vPulsed(m_pDataStack->iPopBol(__func__));
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* SubPipe[].op0@ -- fetch op0 of this pipeline                               */
/*----------------------------------------------------------------------------*/
void Step::vSubPipeOp0Fetch(void)
{
  ENTRY();
    
  Op* pOp0 = m_pLogicUnit->m_xpSubPipe->pOp0();
    
  m_pDataStack->vPushPtr(pOp0);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* SubPipe[].opsOnNode@ -- get no of ops executing on this node               */
/*----------------------------------------------------------------------------*/
void Step::vSubPipeOpsOnNodeFetch(void)
{
  ENTRY();
    
  i8 iThisNode = AM::pGrid()->iThisNodeNo();

  i8 nOps = m_pLogicUnit->m_xpSubPipe->nOpsOnNode(iThisNode);

  m_pDataStack->vPushInt(nOps);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* SunPipeline[].set -- set subpipeline                                       */
/*----------------------------------------------------------------------------*/
void Step::vSubPipeSet(void)
{
  ENTRY();
  
  m_pLogicUnit->m_xpSubPipe =
    _pPipeline(m_pDataStack->pPopPtr(__func__));

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* SubPipe[].setInvoker -- set invoker for all tasks in sub-pipeline          */
/*----------------------------------------------------------------------------*/
void Step::vSubPipeSetInvoker(void)
{
  ENTRY();

  Task* pInvoker = _pTask(m_pDataStack->pPopPtr(__func__));
  
  m_pLogicUnit->m_xpSubPipe->vSetInvoker(pInvoker);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* SubPipe[].spawnOpTasks -- spawn tasks for all ops of pipeline for this node*/
/*----------------------------------------------------------------------------*/
void Step::vSubPipeSpawnOpTasks(void)
{
  ENTRY();

  if (AM::TRACE::bVerbose(C_VB_PIPELINE)) {
    i8 iThisNodeNo = MY::iNodeNo();
    vTraceM("Spawning operator jobs for this node %lld\n"_Stz,iThisNodeNo);
  }
    
  m_pLogicUnit->m_xpSubPipe->vSpawnOpTasks();
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
