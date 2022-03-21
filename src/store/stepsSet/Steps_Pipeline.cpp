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
void Step::vAddSteps_Pipeline(void)
{
  vAddStep("[pipeline].addAccum"_Stz,      &Step::vPipelineAddAccum);
  vAddStep("[pipeline].cancelled@"_Stz,    &Step::vPipelineCancelledFetch);
  vAddStep("[pipeline].cancelled!"_Stz,    &Step::vPipelineCancelledStore);
  vAddStep("[pipeline].create"_Stz,        &Step::vPipelineCreate);
  vAddStep("[pipeline].dropDebugLock"_Stz, &Step::vPipelineDropDebugLock);
  vAddStep("[pipeline].earlyExit@"_Stz,    &Step::vPipelineEarlyExitFetch);
  vAddStep("[pipeline].earlyExit!"_Stz,    &Step::vPipelineEarlyExitStore);
  vAddStep("[pipeline].errored@"_Stz,      &Step::vPipelineErroredFetch);
  vAddStep("[pipeline].errored!"_Stz,      &Step::vPipelineErroredStore);
  vAddStep("[pipeline].getAccum"_Stz,      &Step::vPipelineGetAccum);
  vAddStep("[pipeline].grabDebugLock"_Stz, &Step::vPipelineGrabDebugLock);
  vAddStep("[pipeline].op0@"_Stz,          &Step::vPipelineOp0Fetch);
  vAddStep("[pipeline].opsOnNode@"_Stz,    &Step::vPipelineOpsOnNodeFetch);
  vAddStep("[pipeline].pulsed@"_Stz,       &Step::vPipelinePulsedFetch);
  vAddStep("[pipeline].pulsed!"_Stz,       &Step::vPipelinePulsedStore);
  vAddStep("[pipeline].spawnOpTasks"_Stz,  &Step::vPipelineSpawnOpTasks);
  vAddStep("[pipeline].startPipeline"_Stz, &Step::vPipelineStartPipeline);
}

/*----------------------------------------------------------------------------*/
/* [pipeline].addAccum -- add to an accumulator in this pipeline              */
/*----------------------------------------------------------------------------*/
void Step::vPipelineAddAccum(void)
{
  ENTRY();
  
  Pipeline* pPipeline = m_pOp->pPipeline();
  
  i8 i_Accum = m_pDataStack->iPopInt(__func__);
  
  i8 iValue  = m_pDataStack->iPopInt(__func__);
  
  pPipeline->vAddAccum(i_Accum,iValue);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [pipeline].cancelled@ -- has this pipeline been cancelled?                 */
/*----------------------------------------------------------------------------*/
void Step::vPipelineCancelledFetch(void)
{
  ENTRY();
  
  Pipeline* pPipeline = m_pOp->pPipeline();
  
  // return if pipeline has been cancelled
  m_pDataStack->vPushInt(pPipeline->bCancelled());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [pipeline].cancelled! -- fetch this pipeline's cancelled indicator         */
/*----------------------------------------------------------------------------*/
void Step::vPipelineCancelledStore(void)
{
  ENTRY();

  Pipeline* pPipeline = m_pOp->pPipeline();
  
  pPipeline->vCancelled(m_pDataStack->iPopInt(__func__));
  
  EXIT();
}


/*----------------------------------------------------------------------------*/
/* [pipeline].create -- create the pipeline, stages, streams and ops          */
/*----------------------------------------------------------------------------*/
void Step::vPipelineCreate(void)
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
/* [pipeline].dropDebugLock -- drop this pipeline's debug lock                */
/*----------------------------------------------------------------------------*/
void Step::vPipelineDropDebugLock(void)
{
  ENTRY();
  
  Pipeline* pPipeline = m_pOp->pPipeline();
  
  pPipeline->vDropDebugLock();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [pipeline].earlyExit@ -- fetch this pipelines earlyExit indicator          */
/*----------------------------------------------------------------------------*/
void Step::vPipelineEarlyExitFetch(void)
{
  ENTRY();

  Pipeline* pPipeline = m_pOp->pPipeline();

  // return if pipeline has been errored
  m_pDataStack->vPushInt(pPipeline->bEarlyExit());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [pipeline].earlyExit! -- store this pipeline's earlyExit indicator         */
/*----------------------------------------------------------------------------*/
void Step::vPipelineEarlyExitStore(void)
{
  ENTRY();

  Pipeline* pPipeline = m_pOp->pPipeline();

  pPipeline->vEarlyExit(m_pDataStack->iPopIntBol(__func__));

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [pipeline].errored@ -- fetch this pipeline errored indicator               */
/*----------------------------------------------------------------------------*/
void Step::vPipelineErroredFetch(void)
{
  ENTRY();
  
  Pipeline* pPipeline = m_pOp->pPipeline();
  
  // return if pipeline has been errored
  m_pDataStack->vPushInt(pPipeline->bErrored());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [pipeline].errored! -- store this pipeline errored indicator               */
/*----------------------------------------------------------------------------*/
void Step::vPipelineErroredStore(void)
{
  ENTRY();
  
  Pipeline* pPipeline = m_pOp->pPipeline();
  
  pPipeline->vErrored(m_pDataStack->iPopBol(__func__));
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [pipeline].getAccum -- get an accumulator from this pipeline               */
/*----------------------------------------------------------------------------*/
void Step::vPipelineGetAccum(void)
{
  ENTRY();
  
  Pipeline* pPipeline = m_pOp->pPipeline();
  
  i8 i_Accum = m_pDataStack->iPopInt(__func__);
  
  i8 iValue = pPipeline->iGetAccum(i_Accum);
  
  m_pDataStack->vPushInt(iValue);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [pipeline].grabDebugLock -- grab this pipeline's debug lock                */
/*----------------------------------------------------------------------------*/
void Step::vPipelineGrabDebugLock(void)
{
  ENTRY();
  
  Pipeline* pPipeline = m_pOp->pPipeline();
  
  pPipeline->vGrabDebugLock();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [pipeline].pulsed@ -- fetch this pipeline's pulsed indicator               */
/*----------------------------------------------------------------------------*/
void Step::vPipelinePulsedFetch(void)
{
  ENTRY();
  
  Pipeline* pPipeline = m_pOp->pPipeline();
  
  // return if pipeline has been pulsed
  m_pDataStack->vPushInt(pPipeline->bPulsed());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [pipeline].pulsed! -- store this pipeline's pulsed indicator               */
/*----------------------------------------------------------------------------*/
void Step::vPipelinePulsedStore(void)
{
  ENTRY();
  
  Pipeline* pPipeline = m_pOp->pPipeline();
  
  pPipeline->vPulsed(m_pDataStack->iPopBol(__func__));
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [pipeline].op0@ -- fetch op0 of this pipeline                              */
/*----------------------------------------------------------------------------*/
void Step::vPipelineOp0Fetch(void)
{
  ENTRY();
  
  Pipeline* pPipeline = (* m_pLogicUnit->m_xpPipeline);
  
  Op* pOp0 = pPipeline->pOp0();
    
  m_pDataStack->vPushPtr(pOp0);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [pipeline].opsOnNode@ -- get no of ops executing on this node              */
/*----------------------------------------------------------------------------*/
void Step::vPipelineOpsOnNodeFetch(void)
{
  ENTRY();
    
  Pipeline* pPipeline = (* m_pLogicUnit->m_xpPipeline);

  i8 iThisNode = AM::pGrid()->iThisNodeNo();

  i8 nOps = pPipeline->nOpsOnNode(iThisNode);

  m_pDataStack->vPushInt(nOps);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [pipeline].spawnOpTasks -- spawn tasks for all ops of pipeline for this node*/
/*----------------------------------------------------------------------------*/
void Step::vPipelineSpawnOpTasks(void)
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
/* [pipeline].startPipeline -- test for issueing a start marker               */
/*----------------------------------------------------------------------------*/
void Step::vPipelineStartPipeline(void)
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
