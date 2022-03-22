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
void Step::vAddSteps_PipelineInOp(void)
{
  vAddStep("[pipelineInOp].addAccum"_Stz,       &Step::vPipelineInOpAddAccum);
  vAddStep("[pipelineInOp].cancelled@"_Stz,     &Step::vPipelineInOpCancelledFetch);
  vAddStep("[pipelineInOp].cancelled!"_Stz,     &Step::vPipelineInOpCancelledStore);
  vAddStep("[pipelineInOp].create"_Stz,         &Step::vPipelineInOpCreate);
  vAddStep("[pipelineInOp].dropDebugLock"_Stz,  &Step::vPipelineInOpDropDebugLock);
  vAddStep("[pipelineInOp].earlyExit@"_Stz,     &Step::vPipelineInOpEarlyExitFetch);
  vAddStep("[pipelineInOp].earlyExit!"_Stz,     &Step::vPipelineInOpEarlyExitStore);
  vAddStep("[pipelineInOp].errored@"_Stz,       &Step::vPipelineInOpErroredFetch);
  vAddStep("[pipelineInOp].errored!"_Stz,       &Step::vPipelineInOpErroredStore);
  vAddStep("[pipelineInOp].getAccum"_Stz,       &Step::vPipelineInOpGetAccum);
  vAddStep("[pipelineInOp].grabDebugLock"_Stz,  &Step::vPipelineInOpGrabDebugLock);
  vAddStep("[pipelineInOp].op0@"_Stz,           &Step::vPipelineInOpOp0Fetch);
  vAddStep("[pipelineInOp].opsOnNode@"_Stz,     &Step::vPipelineInOpOpsOnNodeFetch);
  vAddStep("[pipelineInOp].pulsed@"_Stz,        &Step::vPipelineInOpPulsedFetch);
  vAddStep("[pipelineInOp].pulsed!"_Stz,        &Step::vPipelineInOpPulsedStore);
  vAddStep("[pipelineInOp].spawnOpTasks"_Stz,   &Step::vPipelineInOpSpawnOpTasks);
  vAddStep("[pipelineInOp].startPipeline?"_Stz, &Step::vPipelineInOpStartPipeline);
}

/*----------------------------------------------------------------------------*/
/* [pipelineInOp].addAccum -- add to an accumulator in this pipeline          */
/*----------------------------------------------------------------------------*/
void Step::vPipelineInOpAddAccum(void)
{
  ENTRY();
  
  Pipeline* pPipeline = m_pOp->pPipeline();
  
  i8 i_Accum = m_pDataStack->iPopInt(__func__);
  
  i8 iValue  = m_pDataStack->iPopInt(__func__);
  
  pPipeline->vAddAccum(i_Accum,iValue);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [pipelineInOp].cancelled@ -- has this pipeline been cancelled?             */
/*----------------------------------------------------------------------------*/
void Step::vPipelineInOpCancelledFetch(void)
{
  ENTRY();
  
  Pipeline* pPipeline = m_pOp->pPipeline();
  
  // return if pipeline has been cancelled
  m_pDataStack->vPushInt(pPipeline->bCancelled());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [pipelineInOp].cancelled! -- fetch this pipeline's cancelled indicator     */
/*----------------------------------------------------------------------------*/
void Step::vPipelineInOpCancelledStore(void)
{
  ENTRY();

  Pipeline* pPipeline = m_pOp->pPipeline();
  
  pPipeline->vCancelled(m_pDataStack->iPopInt(__func__));
  
  EXIT();
}


/*----------------------------------------------------------------------------*/
/* [pipelineInOp].create -- create the pipeline, stages, streams and ops      */
/*----------------------------------------------------------------------------*/
void Step::vPipelineInOpCreate(void)
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
/* [pipelineInOp].dropDebugLock -- drop this pipeline's debug lock            */
/*----------------------------------------------------------------------------*/
void Step::vPipelineInOpDropDebugLock(void)
{
  ENTRY();
  
  Pipeline* pPipeline = m_pOp->pPipeline();
  
  pPipeline->vDropDebugLock();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [pipelineInOp].earlyExit@ -- fetch this pipelines earlyExit indicator      */
/*----------------------------------------------------------------------------*/
void Step::vPipelineInOpEarlyExitFetch(void)
{
  ENTRY();

  Pipeline* pPipeline = m_pOp->pPipeline();

  // return if pipeline has been errored
  m_pDataStack->vPushInt(pPipeline->bEarlyExit());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [pipelineInOp].earlyExit! -- store this pipeline's earlyExit indicator     */
/*----------------------------------------------------------------------------*/
void Step::vPipelineInOpEarlyExitStore(void)
{
  ENTRY();

  Pipeline* pPipeline = m_pOp->pPipeline();

  pPipeline->vEarlyExit(m_pDataStack->iPopIntBol(__func__));

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [pipelineInOp].errored@ -- fetch this pipeline errored indicator           */
/*----------------------------------------------------------------------------*/
void Step::vPipelineInOpErroredFetch(void)
{
  ENTRY();
  
  Pipeline* pPipeline = m_pOp->pPipeline();
  
  // return if pipeline has been errored
  m_pDataStack->vPushInt(pPipeline->bErrored());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [pipelineInOp].errored! -- store this pipeline errored indicator           */
/*----------------------------------------------------------------------------*/
void Step::vPipelineInOpErroredStore(void)
{
  ENTRY();
  
  Pipeline* pPipeline = m_pOp->pPipeline();
  
  pPipeline->vErrored(m_pDataStack->iPopBol(__func__));
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [pipelineInOp].getAccum -- get an accumulator from this pipeline           */
/*----------------------------------------------------------------------------*/
void Step::vPipelineInOpGetAccum(void)
{
  ENTRY();
  
  Pipeline* pPipeline = m_pOp->pPipeline();
  
  i8 i_Accum = m_pDataStack->iPopInt(__func__);
  
  i8 iValue = pPipeline->iGetAccum(i_Accum);
  
  m_pDataStack->vPushInt(iValue);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [pipelineInOp].grabDebugLock -- grab this pipeline's debug lock            */
/*----------------------------------------------------------------------------*/
void Step::vPipelineInOpGrabDebugLock(void)
{
  ENTRY();
  
  Pipeline* pPipeline = m_pOp->pPipeline();
  
  pPipeline->vGrabDebugLock();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [pipelineInOp].pulsed@ -- fetch this pipeline's pulsed indicator           */
/*----------------------------------------------------------------------------*/
void Step::vPipelineInOpPulsedFetch(void)
{
  ENTRY();
  
  Pipeline* pPipeline = m_pOp->pPipeline();
  
  // return if pipeline has been pulsed
  m_pDataStack->vPushInt(pPipeline->bPulsed());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [pipelineInOp].pulsed! -- store this pipeline's pulsed indicator           */
/*----------------------------------------------------------------------------*/
void Step::vPipelineInOpPulsedStore(void)
{
  ENTRY();
  
  Pipeline* pPipeline = m_pOp->pPipeline();
  
  pPipeline->vPulsed(m_pDataStack->iPopBol(__func__));
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [pipelineInOp].op0@ -- fetch op0 of this pipeline                          */
/*----------------------------------------------------------------------------*/
void Step::vPipelineInOpOp0Fetch(void)
{
  ENTRY();
  
  Pipeline* pPipeline = (* m_pLogicUnit->m_xpPipeline);
  
  Op* pOp0 = pPipeline->pOp0();
    
  m_pDataStack->vPushPtr(pOp0);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [pipelineInOp].opsOnNode@ -- get no of ops executing on this node          */
/*----------------------------------------------------------------------------*/
void Step::vPipelineInOpOpsOnNodeFetch(void)
{
  ENTRY();
    
  Pipeline* pPipeline = (* m_pLogicUnit->m_xpPipeline);

  i8 iThisNode = AM::pGrid()->iThisNodeNo();

  i8 nOps = pPipeline->nOpsOnNode(iThisNode);

  m_pDataStack->vPushInt(nOps);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [pipelineInOp].spawnOpTasks -- spawn tasks for all ops of pipeline for this node*/
/*----------------------------------------------------------------------------*/
void Step::vPipelineInOpSpawnOpTasks(void)
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
/* [pipelineInOp].startPipeline? -- test for issueing a start marker          */
/*----------------------------------------------------------------------------*/
void Step::vPipelineInOpStartPipeline(void)
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
