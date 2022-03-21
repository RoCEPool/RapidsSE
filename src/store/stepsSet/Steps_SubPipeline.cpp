/*============================================================================*/
/* Steps_SubPipeline.cpp -- SubPipeline step group                            */
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
void Step::vAddSteps_SubPipeline(void)
{
  vAddStep("SubPipeline[].addAccum"_Stz,      &Step::vSubPipelineAddAccum);
  vAddStep("SubPipeline[].cancelled@"_Stz,    &Step::vSubPipelineCancelledFetch);
  vAddStep("SubPipeline[].cancelled!"_Stz,    &Step::vSubPipelineCancelledStore);
  vAddStep("SubPipeline[].create"_Stz,        &Step::vSubPipelineCreate);
  vAddStep("SubPipeline[].dropDebugLock"_Stz, &Step::vSubPipelineDropDebugLock);
  vAddStep("SubPipeline[].earlyExit@"_Stz,    &Step::vSubPipelineEarlyExitFetch);
  vAddStep("SubPipeline[].earlyExit!"_Stz,    &Step::vSubPipelineEarlyExitStore);
  vAddStep("SubPipeline[].errored@"_Stz,      &Step::vSubPipelineErroredFetch);
  vAddStep("SubPipeline[].errored!"_Stz,      &Step::vSubPipelineErroredStore);
  vAddStep("SubPipeline[].getAccum"_Stz,      &Step::vSubPipelineGetAccum);
  vAddStep("SubPipeline[].grabDebugLock"_Stz, &Step::vSubPipelineGrabDebugLock);
  vAddStep("SubPipeline[].op0@"_Stz,          &Step::vSubPipelineOp0Fetch);
  vAddStep("SubPipeline[].opsOnNode@"_Stz,    &Step::vSubPipelineOpsOnNodeFetch);
  vAddStep("SubPipeline[].pulsed@"_Stz,       &Step::vSubPipelinePulsedFetch);
  vAddStep("SubPipeline[].pulsed!"_Stz,       &Step::vSubPipelinePulsedStore);
  vAddStep("SubPipeline[].spawnOpTasks"_Stz,  &Step::vSubPipelineSpawnOpTasks);
  vAddStep("SubPipeline[].startPipeline"_Stz, &Step::vSubPipelineStartPipeline);
}

/*----------------------------------------------------------------------------*/
/* SubPipeline[].addAccum -- add to an accumulator in this pipeline           */
/*----------------------------------------------------------------------------*/
void Step::vSubPipelineAddAccum(void)
{
  ENTRY();
    
  i8 i_Accum = m_pDataStack->iPopInt(__func__);
  
  i8 iValue  = m_pDataStack->iPopInt(__func__);
  
  m_pLogicUnit->m_xpSubPipeline->vAddAccum(i_Accum,iValue);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* SubPipeline[].cancelled@ -- has this pipeline been cancelled?              */
/*----------------------------------------------------------------------------*/
void Step::vSubPipelineCancelledFetch(void)
{
  ENTRY();
  
  // return if pipeline has been cancelled
  m_pDataStack->vPushInt(m_pLogicUnit->m_xpSubPipeline->bCancelled());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* SubPipeline[].cancelled! -- fetch this pipeline's cancelled indicator      */
/*----------------------------------------------------------------------------*/
void Step::vSubPipelineCancelledStore(void)
{
  ENTRY();
  
  m_pLogicUnit->m_xpSubPipeline->vCancelled(m_pDataStack->iPopInt(__func__));
  
  EXIT();
}


/*----------------------------------------------------------------------------*/
/* SubPipeline[].create -- create the pipeline, stages, streams and ops       */
/*----------------------------------------------------------------------------*/
void Step::vSubPipelineCreate(void)
{
  ENTRY();
    
  i8 iThisNode = AM::pGrid()->iThisNodeNo();

  // get pipeline spec
  
  PipelineSpec* pPipelineSpec = _pPipelineSpec(m_pDataStack->pPopPtr(__func__));
  
  // create subpipeline
  
  Pipeline* pSubPipeline =
    new Pipeline(m_pJob,pPipelineSpec,m_pJob->iOriginatorNode());
  
  m_pLogicUnit->m_xpSubPipeline = pSubPipeline;
    
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
/* SubPipeline[].dropDebugLock -- drop this pipeline's debug lock             */
/*----------------------------------------------------------------------------*/
void Step::vSubPipelineDropDebugLock(void)
{
  ENTRY();
    
  m_pLogicUnit->m_xpSubPipeline->vDropDebugLock();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* SubPipeline[].earlyExit@ -- fetch this pipelines earlyExit indicator       */
/*----------------------------------------------------------------------------*/
void Step::vSubPipelineEarlyExitFetch(void)
{
  ENTRY();

  // return if pipeline has been errored
  m_pDataStack->vPushInt(m_pLogicUnit->m_xpSubPipeline->bEarlyExit());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* SubPipeline[].earlyExit! -- store this pipeline's earlyExit indicator      */
/*----------------------------------------------------------------------------*/
void Step::vSubPipelineEarlyExitStore(void)
{
  ENTRY();

  m_pLogicUnit->m_xpSubPipeline->vEarlyExit(m_pDataStack->iPopIntBol(__func__));

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* SubPipeline[].errored@ -- fetch this pipeline errored indicator            */
/*----------------------------------------------------------------------------*/
void Step::vSubPipelineErroredFetch(void)
{
  ENTRY();
    
  // return if pipeline has been errored
  m_pDataStack->vPushInt(m_pLogicUnit->m_xpSubPipeline->bErrored());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* SubPipeline[].errored! -- store this pipeline errored indicator            */
/*----------------------------------------------------------------------------*/
void Step::vSubPipelineErroredStore(void)
{
  ENTRY();
  
  m_pLogicUnit->m_xpSubPipeline->vErrored(m_pDataStack->iPopBol(__func__));
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* SubPipeline[].getAccum -- get an accumulator from this pipeline            */
/*----------------------------------------------------------------------------*/
void Step::vSubPipelineGetAccum(void)
{
  ENTRY();
   
  i8 i_Accum = m_pDataStack->iPopInt(__func__);
  
  i8 iValue = m_pLogicUnit->m_xpSubPipeline->iGetAccum(i_Accum);
  
  m_pDataStack->vPushInt(iValue);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* SubPipeline[].grabDebugLock -- grab this pipeline's debug lock             */
/*----------------------------------------------------------------------------*/
void Step::vSubPipelineGrabDebugLock(void)
{
  ENTRY();
    
  m_pLogicUnit->m_xpSubPipeline->vGrabDebugLock();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* SubPipeline[].pulsed@ -- fetch this pipeline's pulsed indicator            */
/*----------------------------------------------------------------------------*/
void Step::vSubPipelinePulsedFetch(void)
{
  ENTRY();
    
  // return if pipeline has been pulsed
  m_pDataStack->vPushInt(m_pLogicUnit->m_xpSubPipeline->bPulsed());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* SubPipeline[].pulsed! -- store this pipeline's pulsed indicator            */
/*----------------------------------------------------------------------------*/
void Step::vSubPipelinePulsedStore(void)
{
  ENTRY();
    
  m_pLogicUnit->m_xpSubPipeline->vPulsed(m_pDataStack->iPopBol(__func__));
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* SubPipeline[].op0@ -- fetch op0 of this pipeline                           */
/*----------------------------------------------------------------------------*/
void Step::vSubPipelineOp0Fetch(void)
{
  ENTRY();
    
  Op* pOp0 = m_pLogicUnit->m_xpSubPipeline->pOp0();
    
  m_pDataStack->vPushPtr(pOp0);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* SubPipeline[].opsOnNode@ -- get no of ops executing on this node           */
/*----------------------------------------------------------------------------*/
void Step::vSubPipelineOpsOnNodeFetch(void)
{
  ENTRY();
    
  i8 iThisNode = AM::pGrid()->iThisNodeNo();

  i8 nOps = m_pLogicUnit->m_xpSubPipeline->nOpsOnNode(iThisNode);

  m_pDataStack->vPushInt(nOps);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* SubPipeline[].spawnOpTasks -- spawn tasks for all ops of pipeline for this node*/
/*----------------------------------------------------------------------------*/
void Step::vSubPipelineSpawnOpTasks(void)
{
  ENTRY();

  if (AM::TRACE::bVerbose(C_VB_PIPELINE)) {
    i8 iThisNodeNo = MY::iNodeNo();
    vTraceM("Spawning operator jobs for this node %lld\n"_Stz,iThisNodeNo);
  }
    
  m_pLogicUnit->m_xpSubPipeline->vSpawnOpTasks();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* SubPipeline[].startPipeline -- test for issueing a start marker            */
/*----------------------------------------------------------------------------*/
void Step::vSubPipelineStartPipeline(void)
{
  ENTRY();
  
  Pipework* pFormWork = m_pLogicUnit->m_xpSubPipeline->pPipework();

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
