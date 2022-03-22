/*============================================================================*/
/* Steps_Pipeline.cpp -- Pipeline step group                                  */
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
#include "Pipework.hpp"
#include "Pipeline.hpp"
#include "PipelineSpec.hpp"
#include "Job.hpp"
#include "Node_I.hpp"
#include "LogicUnit.hpp"

#include "Marker.hpp"

#include "SP_Pipeline.hpp"
#include "SP_PipelineSpec.hpp"

#include "Dynamo.hpp"
#include "TagConstants.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_PipelineOfOp(void)
{
  vAddStep("[pipelineOfOp].addAccum"_Stz,       &Step::vPipelineOfOpAddAccum);
  vAddStep("[pipelineOfOp].cancelled@"_Stz,     &Step::vPipelineOfOpCancelledFetch);
  vAddStep("[pipelineOfOp].cancelled!"_Stz,     &Step::vPipelineOfOpCancelledStore);
  vAddStep("[pipelineOfOp].create"_Stz,         &Step::vPipelineOfOpCreate);
  vAddStep("[pipelineOfOp].dropDebugLock"_Stz,  &Step::vPipelineOfOpDropDebugLock);
  vAddStep("[pipelineOfOp].earlyExit@"_Stz,     &Step::vPipelineOfOpEarlyExitFetch);
  vAddStep("[pipelineOfOp].earlyExit!"_Stz,     &Step::vPipelineOfOpEarlyExitStore);
  vAddStep("[pipelineOfOp].errored@"_Stz,       &Step::vPipelineOfOpErroredFetch);
  vAddStep("[pipelineOfOp].errored!"_Stz,       &Step::vPipelineOfOpErroredStore);
  vAddStep("[pipelineOfOp].getAccum"_Stz,       &Step::vPipelineOfOpGetAccum);
  vAddStep("[pipelineOfOp].grabDebugLock"_Stz,  &Step::vPipelineOfOpGrabDebugLock);
  vAddStep("[pipelineOfOp].op0@"_Stz,           &Step::vPipelineOfOpOp0Fetch);
  vAddStep("[pipelineOfOp].opsOnNode@"_Stz,     &Step::vPipelineOfOpOpsOnNodeFetch);
  vAddStep("[pipelineOfOp].pulsed@"_Stz,        &Step::vPipelineOfOpPulsedFetch);
  vAddStep("[pipelineOfOp].pulsed!"_Stz,        &Step::vPipelineOfOpPulsedStore);
  vAddStep("[pipelineOfOp].spawnOpTasks"_Stz,   &Step::vPipelineOfOpSpawnOpTasks);
  vAddStep("[pipelineOfOp].startPipeline?"_Stz, &Step::vPipelineOfOpStartPipeline);
}

/*----------------------------------------------------------------------------*/
/* [pipelineOfOp].addAccum -- add to an accumulator in this pipeline          */
/*----------------------------------------------------------------------------*/
void Step::vPipelineOfOpAddAccum(void)
{
  ENTRY();
  
  Pipeline* pPipeline = m_pOp->pPipeline();
  
  i8 i_Accum = m_pDataStack->iPopInt(__func__);
  
  i8 iValue  = m_pDataStack->iPopInt(__func__);
  
  pPipeline->vAddAccum(i_Accum,iValue);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [pipelineOfOp].cancelled@ -- has this pipeline been cancelled?             */
/*----------------------------------------------------------------------------*/
void Step::vPipelineOfOpCancelledFetch(void)
{
  ENTRY();
  
  Pipeline* pPipeline = m_pOp->pPipeline();
  
  // return if pipeline has been cancelled
  m_pDataStack->vPushInt(pPipeline->bCancelled());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [pipelineOfOp].cancelled! -- fetch this pipeline's cancelled indicator     */
/*----------------------------------------------------------------------------*/
void Step::vPipelineOfOpCancelledStore(void)
{
  ENTRY();

  Pipeline* pPipeline = m_pOp->pPipeline();
  
  pPipeline->vCancelled(m_pDataStack->iPopInt(__func__));
  
  EXIT();
}


/*----------------------------------------------------------------------------*/
/* [pipelineOfOp].create -- create the pipeline, stages, streams and ops      */
/*----------------------------------------------------------------------------*/
void Step::vPipelineOfOpCreate(void)
{
  ENTRY();
    
  i8 iThisNode = AM::pGrid()->iThisNodeNo();

  // get pipeline spec
  
  PipelineSpec* pPipelineSpec = _pPipelineSpec(m_pDataStack->pPopPtr(__func__));
  
  // create pipeline
  
  Pipeline* pPipeline =
    new Pipeline(m_pJob,pPipelineSpec,m_pJob->iOriginatorNode());
  
  m_pLogicUnit->m_xpPipeline = pPipeline;
    
  // dump pipeline
  
  if (AM::TRACE::bVerbose(C_VB_PIPELINE)) {
    if (iThisNode == 0) {
      SP_Pipeline spPipeline(pPipeline,true);
      vTraceM("%s\n"_Stz,spPipeline.sz());
      vDebugPoint();
    }
  }
    
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [pipelineOfOp].dropDebugLock -- drop this pipeline's debug lock            */
/*----------------------------------------------------------------------------*/
void Step::vPipelineOfOpDropDebugLock(void)
{
  ENTRY();
  
  Pipeline* pPipeline = m_pOp->pPipeline();
  
  pPipeline->vDropDebugLock();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [pipelineOfOp].earlyExit@ -- fetch this pipelines earlyExit indicator      */
/*----------------------------------------------------------------------------*/
void Step::vPipelineOfOpEarlyExitFetch(void)
{
  ENTRY();

  Pipeline* pPipeline = m_pOp->pPipeline();

  // return if pipeline has been errored
  m_pDataStack->vPushInt(pPipeline->bEarlyExit());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [pipelineOfOp].earlyExit! -- store this pipeline's earlyExit indicator     */
/*----------------------------------------------------------------------------*/
void Step::vPipelineOfOpEarlyExitStore(void)
{
  ENTRY();

  Pipeline* pPipeline = m_pOp->pPipeline();

  pPipeline->vEarlyExit(m_pDataStack->iPopIntBol(__func__));

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [pipelineOfOp].errored@ -- fetch this pipeline errored indicator           */
/*----------------------------------------------------------------------------*/
void Step::vPipelineOfOpErroredFetch(void)
{
  ENTRY();
  
  Pipeline* pPipeline = m_pOp->pPipeline();
  
  // return if pipeline has been errored
  m_pDataStack->vPushInt(pPipeline->bErrored());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [pipelineOfOp].errored! -- store this pipeline errored indicator           */
/*----------------------------------------------------------------------------*/
void Step::vPipelineOfOpErroredStore(void)
{
  ENTRY();
  
  Pipeline* pPipeline = m_pOp->pPipeline();
  
  pPipeline->vErrored(m_pDataStack->iPopBol(__func__));
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [pipelineOfOp].getAccum -- get an accumulator from this pipeline           */
/*----------------------------------------------------------------------------*/
void Step::vPipelineOfOpGetAccum(void)
{
  ENTRY();
  
  Pipeline* pPipeline = m_pOp->pPipeline();
  
  i8 i_Accum = m_pDataStack->iPopInt(__func__);
  
  i8 iValue = pPipeline->iGetAccum(i_Accum);
  
  m_pDataStack->vPushInt(iValue);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [pipelineOfOp].grabDebugLock -- grab this pipeline's debug lock            */
/*----------------------------------------------------------------------------*/
void Step::vPipelineOfOpGrabDebugLock(void)
{
  ENTRY();
  
  Pipeline* pPipeline = m_pOp->pPipeline();
  
  pPipeline->vGrabDebugLock();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [pipelineOfOp].pulsed@ -- fetch this pipeline's pulsed indicator           */
/*----------------------------------------------------------------------------*/
void Step::vPipelineOfOpPulsedFetch(void)
{
  ENTRY();
  
  Pipeline* pPipeline = m_pOp->pPipeline();
  
  // return if pipeline has been pulsed
  m_pDataStack->vPushInt(pPipeline->bPulsed());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [pipelineOfOp].pulsed! -- store this pipeline's pulsed indicator           */
/*----------------------------------------------------------------------------*/
void Step::vPipelineOfOpPulsedStore(void)
{
  ENTRY();
  
  Pipeline* pPipeline = m_pOp->pPipeline();
  
  pPipeline->vPulsed(m_pDataStack->iPopBol(__func__));
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [pipelineOfOp].op0@ -- fetch op0 of this pipeline                          */
/*----------------------------------------------------------------------------*/
void Step::vPipelineOfOpOp0Fetch(void)
{
  ENTRY();
  
  Pipeline* pPipeline = (* m_pLogicUnit->m_xpPipeline);
  
  Op* pOp0 = pPipeline->pOp0();
    
  m_pDataStack->vPushPtr(pOp0);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [pipelineOfOp].opsOnNode@ -- get no of ops executing on this node          */
/*----------------------------------------------------------------------------*/
void Step::vPipelineOfOpOpsOnNodeFetch(void)
{
  ENTRY();
    
  Pipeline* pPipeline = (* m_pLogicUnit->m_xpPipeline);

  i8 iThisNode = AM::pGrid()->iThisNodeNo();

  i8 nOps = pPipeline->nOpsOnNode(iThisNode);

  m_pDataStack->vPushInt(nOps);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [pipelineOfOp].spawnOpTasks -- spawn tasks for all ops of pipeline for this node*/
/*----------------------------------------------------------------------------*/
void Step::vPipelineOfOpSpawnOpTasks(void)
{
  ENTRY();

  if (AM::TRACE::bVerbose(C_VB_PIPELINE)) {
    i8 iThisNodeNo = MY::iNodeNo();
    vTraceM("Spawning operator jobs for this node %lld\n"_Stz,iThisNodeNo);
  }
  
  Pipeline* pPipeline = (* m_pLogicUnit->m_xpPipeline);
  
  pPipeline->vSpawnOpTasks();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [pipelineOfOp].startPipeline? -- test for issueing a start marker          */
/*----------------------------------------------------------------------------*/
void Step::vPipelineOfOpStartPipeline(void)
{
  ENTRY();
  
  Pipeline* pPipeline = (* m_pLogicUnit->m_xpPipeline);
  Pipework* pFormWork = pPipeline->pPipework();

  Stage*    pStage0   = pFormWork->pGet(0);

  if (pStage0->bStartPipeline()) {
    
    m_pDataStack->vPushInt(true);
  }
  else {
    m_pDataStack->vPushInt(false);
  }

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
