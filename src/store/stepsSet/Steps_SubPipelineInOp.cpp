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
void Step::vAddSteps_SubPipelineInOp(void)
{
  vAddStep("[SubPLineInOp].addAccum"_Stz,       &Step::vSubPipelineInOpAddAccum);
  vAddStep("[SubPLineInOp].cancelled@"_Stz,     &Step::vSubPipelineInOpCancelledFetch);
  vAddStep("[SubPLineInOp].cancelled!"_Stz,     &Step::vSubPipelineInOpCancelledStore);
  vAddStep("[SubPLineInOp].create"_Stz,         &Step::vSubPipelineInOpCreate);
  vAddStep("[SubPLineInOp].dropDebugLock"_Stz,  &Step::vSubPipelineInOpDropDebugLock);
  vAddStep("[SubPLineInOp].earlyExit@"_Stz,     &Step::vSubPipelineInOpEarlyExitFetch);
  vAddStep("[SubPLineInOp].earlyExit!"_Stz,     &Step::vSubPipelineInOpEarlyExitStore);
  vAddStep("[SubPLineInOp].errored@"_Stz,       &Step::vSubPipelineInOpErroredFetch);
  vAddStep("[SubPLineInOp].errored!"_Stz,       &Step::vSubPipelineInOpErroredStore);
  vAddStep("[SubPLineInOp].getAccum"_Stz,       &Step::vSubPipelineInOpGetAccum);
  vAddStep("[SubPLineInOp].grabDebugLock"_Stz,  &Step::vSubPipelineInOpGrabDebugLock);
  vAddStep("[SubPLineInOp].op0@"_Stz,           &Step::vSubPipelineInOpOp0Fetch);
  vAddStep("[SubPLineInOp].opsOnNode@"_Stz,     &Step::vSubPipelineInOpOpsOnNodeFetch);
  vAddStep("[SubPLineInOp].pulsed@"_Stz,        &Step::vSubPipelineInOpPulsedFetch);
  vAddStep("[SubPLineInOp].pulsed!"_Stz,        &Step::vSubPipelineInOpPulsedStore);
  vAddStep("[SubPLineInOp].spawnOpTasks"_Stz,   &Step::vSubPipelineInOpSpawnOpTasks);
  vAddStep("[SubPLineInOp].startPipeline?"_Stz, &Step::vSubPipelineInOpStartPipeline);
}

/*----------------------------------------------------------------------------*/
/* [SubPLineInOp].addAccum -- add to an accumulator in this pipeline       */
/*----------------------------------------------------------------------------*/
void Step::vSubPipelineInOpAddAccum(void)
{
  ENTRY();
    
  i8 i_Accum = m_pDataStack->iPopInt(__func__);
  
  i8 iValue  = m_pDataStack->iPopInt(__func__);
  
  m_pLogicUnit->m_xpSubPipeline->vAddAccum(i_Accum,iValue);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [SubPLineInOp].cancelled@ -- has this pipeline been cancelled?          */
/*----------------------------------------------------------------------------*/
void Step::vSubPipelineInOpCancelledFetch(void)
{
  ENTRY();
  
  // return if pipeline has been cancelled
  m_pDataStack->vPushInt(m_pLogicUnit->m_xpSubPipeline->bCancelled());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [SubPLineInOp].cancelled! -- fetch this pipeline's cancelled indicator  */
/*----------------------------------------------------------------------------*/
void Step::vSubPipelineInOpCancelledStore(void)
{
  ENTRY();
  
  m_pLogicUnit->m_xpSubPipeline->vCancelled(m_pDataStack->iPopInt(__func__));
  
  EXIT();
}


/*----------------------------------------------------------------------------*/
/* [SubPLineInOp].create -- create the pipeline, stages, streams and ops   */
/*----------------------------------------------------------------------------*/
void Step::vSubPipelineInOpCreate(void)
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
/* [SubPLineInOp].dropDebugLock -- drop this pipeline's debug lock         */
/*----------------------------------------------------------------------------*/
void Step::vSubPipelineInOpDropDebugLock(void)
{
  ENTRY();
    
  m_pLogicUnit->m_xpSubPipeline->vDropDebugLock();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [SubPLineInOp].earlyExit@ -- fetch this pipelines earlyExit indicator   */
/*----------------------------------------------------------------------------*/
void Step::vSubPipelineInOpEarlyExitFetch(void)
{
  ENTRY();

  // return if pipeline has been errored
  m_pDataStack->vPushInt(m_pLogicUnit->m_xpSubPipeline->bEarlyExit());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [SubPLineInOp].earlyExit! -- store this pipeline's earlyExit indicator  */
/*----------------------------------------------------------------------------*/
void Step::vSubPipelineInOpEarlyExitStore(void)
{
  ENTRY();

  m_pLogicUnit->m_xpSubPipeline->vEarlyExit(m_pDataStack->iPopIntBol(__func__));

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [SubPLineInOp].errored@ -- fetch this pipeline errored indicator        */
/*----------------------------------------------------------------------------*/
void Step::vSubPipelineInOpErroredFetch(void)
{
  ENTRY();
    
  // return if pipeline has been errored
  m_pDataStack->vPushInt(m_pLogicUnit->m_xpSubPipeline->bErrored());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [SubPLineInOp].errored! -- store this pipeline errored indicator        */
/*----------------------------------------------------------------------------*/
void Step::vSubPipelineInOpErroredStore(void)
{
  ENTRY();
  
  m_pLogicUnit->m_xpSubPipeline->vErrored(m_pDataStack->iPopBol(__func__));
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [SubPLineInOp].getAccum -- get an accumulator from this pipeline        */
/*----------------------------------------------------------------------------*/
void Step::vSubPipelineInOpGetAccum(void)
{
  ENTRY();
   
  i8 i_Accum = m_pDataStack->iPopInt(__func__);
  
  i8 iValue = m_pLogicUnit->m_xpSubPipeline->iGetAccum(i_Accum);
  
  m_pDataStack->vPushInt(iValue);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [SubPLineInOp].grabDebugLock -- grab this pipeline's debug lock         */
/*----------------------------------------------------------------------------*/
void Step::vSubPipelineInOpGrabDebugLock(void)
{
  ENTRY();
    
  m_pLogicUnit->m_xpSubPipeline->vGrabDebugLock();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [SubPLineInOp].pulsed@ -- fetch this pipeline's pulsed indicator        */
/*----------------------------------------------------------------------------*/
void Step::vSubPipelineInOpPulsedFetch(void)
{
  ENTRY();
    
  // return if pipeline has been pulsed
  m_pDataStack->vPushInt(m_pLogicUnit->m_xpSubPipeline->bPulsed());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [SubPLineInOp].pulsed! -- store this pipeline's pulsed indicator        */
/*----------------------------------------------------------------------------*/
void Step::vSubPipelineInOpPulsedStore(void)
{
  ENTRY();
    
  m_pLogicUnit->m_xpSubPipeline->vPulsed(m_pDataStack->iPopBol(__func__));
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [SubPLineInOp].op0@ -- fetch op0 of this pipeline                       */
/*----------------------------------------------------------------------------*/
void Step::vSubPipelineInOpOp0Fetch(void)
{
  ENTRY();
    
  Op* pOp0 = m_pLogicUnit->m_xpSubPipeline->pOp0();
    
  m_pDataStack->vPushPtr(pOp0);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [SubPLineInOp].opsOnNode@ -- get no of ops executing on this node       */
/*----------------------------------------------------------------------------*/
void Step::vSubPipelineInOpOpsOnNodeFetch(void)
{
  ENTRY();
    
  i8 iThisNode = AM::pGrid()->iThisNodeNo();

  i8 nOps = m_pLogicUnit->m_xpSubPipeline->nOpsOnNode(iThisNode);

  m_pDataStack->vPushInt(nOps);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [SubPLineInOp].spawnOpTasks -- spawn tasks for all ops of pipeline for this node*/
/*----------------------------------------------------------------------------*/
void Step::vSubPipelineInOpSpawnOpTasks(void)
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
/* [SubPLineInOp].startPipeline? -- test for issueing a start marker       */
/*----------------------------------------------------------------------------*/
void Step::vSubPipelineInOpStartPipeline(void)
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
