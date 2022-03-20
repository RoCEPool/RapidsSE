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
  vAddStep("Pipeline::addAccum"_Stz,      &Step::vPipelineAddAccum);
  vAddStep("Pipeline::cancelled@"_Stz,    &Step::vPipelineCancelledFetch);
  vAddStep("Pipeline::cancelled!"_Stz,    &Step::vPipelineCancelledStore);
  vAddStep("Pipeline::dropDebugLock"_Stz, &Step::vPipelineDropDebugLock);
  vAddStep("Pipeline::earlyExit@"_Stz,    &Step::vPipelineEarlyExitFetch);
  vAddStep("Pipeline::earlyExit!"_Stz,    &Step::vPipelineEarlyExitStore);
  vAddStep("Pipeline::errored@"_Stz,      &Step::vPipelineErroredFetch);
  vAddStep("Pipeline::errored!"_Stz,      &Step::vPipelineErroredStore);
  vAddStep("Pipeline::getAccum"_Stz,      &Step::vPipelineGetAccum);
  vAddStep("Pipeline::grabDebugLock"_Stz, &Step::vPipelineGrabDebugLock);
  vAddStep("Pipeline::pulsed@"_Stz,       &Step::vPipelinePulsedFetch);
  vAddStep("Pipeline::pulsed!"_Stz,       &Step::vPipelinePulsedStore);
  
    
  vAddStep("Pipeline[].create"_Stz,       &Step::vPipelineCreate);
  vAddStep("Pipeline[].op0@"_Stz,         &Step::vPipelineOp0Fetch);
  vAddStep("Pipeline[].opsOnNode@"_Stz,   &Step::vPipelineOpsOnNodeFetch);
  vAddStep("Pipeline[].spawnOpTasks"_Stz, &Step::vPipelineSpawnOpTasks);
  vAddStep("Pipeline[].startPipeline"_Stz,&Step::vPipelineStartPipeline);
}

/*----------------------------------------------------------------------------*/
/* Pipeline::addAccum -- add to an accumulator in this pipeline               */
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
/* Pipeline::cancelled@ -- has this pipeline been cancelled?                  */
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
/* Pipeline::cancelled! -- fetch this pipeline's cancelled indicator          */
/*----------------------------------------------------------------------------*/
void Step::vPipelineCancelledStore(void)
{
  ENTRY();

  Pipeline* pPipeline = m_pOp->pPipeline();
  
  pPipeline->vCancelled(m_pDataStack->iPopInt(__func__));
  
  EXIT();
}


/*----------------------------------------------------------------------------*/
/* Pipeline[].create -- create the pipeline, stages, streams and ops          */
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
/* Pipeline::dropDebugLock -- drop this pipeline's debug lock                 */
/*----------------------------------------------------------------------------*/
void Step::vPipelineDropDebugLock(void)
{
  ENTRY();
  
  Pipeline* pPipeline = m_pOp->pPipeline();
  
  pPipeline->vDropDebugLock();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Pipeline::earlyExit@ -- fetch this pipelines earlyExit indicator          */
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
/* Pipeline::earlyExit! -- store this pipeline's earlyExit indicator          */
/*----------------------------------------------------------------------------*/
void Step::vPipelineEarlyExitStore(void)
{
  ENTRY();

  Pipeline* pPipeline = m_pOp->pPipeline();

  pPipeline->vEarlyExit(m_pDataStack->iPopIntBol(__func__));

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Pipeline::errored@ -- fetch this pipeline errored indicator                */
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
/* Pipeline::errored! -- store this pipeline errored indicator                */
/*----------------------------------------------------------------------------*/
void Step::vPipelineErroredStore(void)
{
  ENTRY();
  
  Pipeline* pPipeline = m_pOp->pPipeline();
  
  pPipeline->vErrored(m_pDataStack->iPopBol(__func__));
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Pipeline::getAccum -- get an accumulator from this pipeline                */
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
/* Pipeline::grabDebugLock -- grab this pipeline's debug lock                 */
/*----------------------------------------------------------------------------*/
void Step::vPipelineGrabDebugLock(void)
{
  ENTRY();
  
  Pipeline* pPipeline = m_pOp->pPipeline();
  
  pPipeline->vGrabDebugLock();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Pipeline::pulsed@ -- fetch this pipeline's pulsed indicator                */
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
/* Pipeline::pulsed! -- store this pipeline's pulsed indicator                */
/*----------------------------------------------------------------------------*/
void Step::vPipelinePulsedStore(void)
{
  ENTRY();
  
  Pipeline* pPipeline = m_pOp->pPipeline();
  
  pPipeline->vPulsed(m_pDataStack->iPopBol(__func__));
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Pipeline[].op0@ -- fetch op0 of this pipeline                              */
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
/* Pipeline[].opsOnNode@ -- get no of ops executing on this node              */
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
/* Pipeline[].spawnOpTasks -- spawn tasks for all ops of pipeline for this node*/
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
/* Pipeline[].startPipeline -- test for issueing a start marker               */
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

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
