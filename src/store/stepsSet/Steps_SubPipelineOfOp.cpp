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
void Step::vAddSteps_SubPipelineOfOp(void)
{
  vAddStep("[subPLineOfOp].addAccum"_Stz,       &Step::vSubPipelineOfOpAddAccum);
  vAddStep("[subPLineOfOp].cancelled@"_Stz,     &Step::vSubPipelineOfOpCancelledFetch);
  vAddStep("[subPLineOfOp].cancelled!"_Stz,     &Step::vSubPipelineOfOpCancelledStore);
  vAddStep("[subPLineOfOp].create"_Stz,         &Step::vSubPipelineOfOpCreate);
  vAddStep("[subPLineOfOp].dropDebugLock"_Stz,  &Step::vSubPipelineOfOpDropDebugLock);
  vAddStep("[subPLineOfOp].earlyExit@"_Stz,     &Step::vSubPipelineOfOpEarlyExitFetch);
  vAddStep("[subPLineOfOp].earlyExit!"_Stz,     &Step::vSubPipelineOfOpEarlyExitStore);
  vAddStep("[subPLineOfOp].errored@"_Stz,       &Step::vSubPipelineOfOpErroredFetch);
  vAddStep("[subPLineOfOp].errored!"_Stz,       &Step::vSubPipelineOfOpErroredStore);
  vAddStep("[subPLineOfOp].getAccum"_Stz,       &Step::vSubPipelineOfOpGetAccum);
  vAddStep("[subPLineOfOp].grabDebugLock"_Stz,  &Step::vSubPipelineOfOpGrabDebugLock);
  vAddStep("[subPLineOfOp].op0@"_Stz,           &Step::vSubPipelineOfOpOp0Fetch);
  vAddStep("[subPLineOfOp].opsOnNode@"_Stz,     &Step::vSubPipelineOfOpOpsOnNodeFetch);
  vAddStep("[subPLineOfOp].pulsed@"_Stz,        &Step::vSubPipelineOfOpPulsedFetch);
  vAddStep("[subPLineOfOp].pulsed!"_Stz,        &Step::vSubPipelineOfOpPulsedStore);
  vAddStep("[subPLineOfOp].spawnOpTasks"_Stz,   &Step::vSubPipelineOfOpSpawnOpTasks);
  vAddStep("[subPLineOfOp].startPipeline?"_Stz, &Step::vSubPipelineOfOpStartPipeline);
}

/*----------------------------------------------------------------------------*/
/* [subPLineOfOp].addAccum -- add to an accumulator in this pipeline          */
/*----------------------------------------------------------------------------*/
void Step::vSubPipelineOfOpAddAccum(void)
{
  ENTRY();
  
  Pipeline* pSubPipelineOfOp = m_pOp->pSubPipelineOfOp();
  
  i8 i_Accum = m_pDataStack->iPopInt(__func__);
  
  i8 iValue  = m_pDataStack->iPopInt(__func__);
  
  pSubPipelineOfOp->vAddAccum(i_Accum,iValue);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [subPLineOfOp].cancelled@ -- has this pipeline been cancelled?             */
/*----------------------------------------------------------------------------*/
void Step::vSubPipelineOfOpCancelledFetch(void)
{
  ENTRY();
  
  Pipeline* pSubPipelineOfOp = m_pOp->pSubPipelineOfOp();
  
  m_pDataStack->vPushInt(pSubPipelineOfOp->bCancelled());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [subPLineOfOp].cancelled! -- fetch this pipeline's cancelled indicator     */
/*----------------------------------------------------------------------------*/
void Step::vSubPipelineOfOpCancelledStore(void)
{
  ENTRY();

  Pipeline* pSubPipelineOfOp = m_pOp->pSubPipelineOfOp();
  
  pSubPipelineOfOp->vCancelled(m_pDataStack->iPopInt(__func__));
  
  EXIT();
}


/*----------------------------------------------------------------------------*/
/* [subPLineOfOp].create -- create the pipeline, stages, streams and ops      */
/*----------------------------------------------------------------------------*/
void Step::vSubPipelineOfOpCreate(void)
{
  ENTRY();
    
  i8 iThisNode = AM::pGrid()->iThisNodeNo();

  // get pipeline spec
  
  PipelineSpec* pPipelineSpec = _pPipelineSpec(m_pDataStack->pPopPtr(__func__));
  
  // create pipeline
  
  Pipeline* pSubPipelineOfOp =
    new Pipeline(m_pJob,pPipelineSpec,m_pJob->iOriginatorNode());
  
  m_pLogicUnit->m_xpPipeline = pSubPipelineOfOp;
    
  // dump pipeline
  
  if (AM::TRACE::bVerbose(C_VB_PIPELINE)) {
    if (iThisNode == 0) {
      SP_Pipeline spPipeline(pSubPipelineOfOp,true);
      vTraceM("%s\n"_Stz,spPipeline.sz());
      vDebugPoint();
    }
  }
    
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [subPLineOfOp].dropDebugLock -- drop this pipeline's debug lock            */
/*----------------------------------------------------------------------------*/
void Step::vSubPipelineOfOpDropDebugLock(void)
{
  ENTRY();
  
  Pipeline* pSubPipelineOfOp = m_pOp->pSubPipelineOfOp();
  
  pSubPipelineOfOp->vDropDebugLock();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [subPLineOfOp].earlyExit@ -- fetch this pipelines earlyExit indicator      */
/*----------------------------------------------------------------------------*/
void Step::vSubPipelineOfOpEarlyExitFetch(void)
{
  ENTRY();

  Pipeline* pSubPipelineOfOp = m_pOp->pSubPipelineOfOp();

  // return if pipeline has been errored
  m_pDataStack->vPushInt(pSubPipelineOfOp->bEarlyExit());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [subPLineOfOp].earlyExit! -- store this pipeline's earlyExit indicator     */
/*----------------------------------------------------------------------------*/
void Step::vSubPipelineOfOpEarlyExitStore(void)
{
  ENTRY();

  Pipeline* pSubPipelineOfOp = m_pOp->pSubPipelineOfOp();

  pSubPipelineOfOp->vEarlyExit(m_pDataStack->iPopIntBol(__func__));

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [subPLineOfOp].errored@ -- fetch this pipeline errored indicator           */
/*----------------------------------------------------------------------------*/
void Step::vSubPipelineOfOpErroredFetch(void)
{
  ENTRY();
  
  Pipeline* pSubPipelineOfOp = m_pOp->pSubPipelineOfOp();
  
  // return if pipeline has been errored
  m_pDataStack->vPushInt(pSubPipelineOfOp->bErrored());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [subPLineOfOp].errored! -- store this pipeline errored indicator           */
/*----------------------------------------------------------------------------*/
void Step::vSubPipelineOfOpErroredStore(void)
{
  ENTRY();
  
  Pipeline* pSubPipelineOfOp = m_pOp->pSubPipelineOfOp();
  
  pSubPipelineOfOp->vErrored(m_pDataStack->iPopBol(__func__));
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [subPLineOfOp].getAccum -- get an accumulator from this pipeline           */
/*----------------------------------------------------------------------------*/
void Step::vSubPipelineOfOpGetAccum(void)
{
  ENTRY();
  
  Pipeline* pSubPipelineOfOp = m_pOp->pSubPipelineOfOp();
  
  i8 i_Accum = m_pDataStack->iPopInt(__func__);
  
  i8 iValue = pSubPipelineOfOp->iGetAccum(i_Accum);
  
  m_pDataStack->vPushInt(iValue);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [subPLineOfOp].grabDebugLock -- grab this pipeline's debug lock            */
/*----------------------------------------------------------------------------*/
void Step::vSubPipelineOfOpGrabDebugLock(void)
{
  ENTRY();
  
  Pipeline* pSubPipelineOfOp = m_pOp->pSubPipelineOfOp();
  
  pSubPipelineOfOp->vGrabDebugLock();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [subPLineOfOp].pulsed@ -- fetch this pipeline's pulsed indicator           */
/*----------------------------------------------------------------------------*/
void Step::vSubPipelineOfOpPulsedFetch(void)
{
  ENTRY();
  
  Pipeline* pSubPipelineOfOp = m_pOp->pSubPipelineOfOp();
  
  // return if pipeline has been pulsed
  m_pDataStack->vPushInt(pSubPipelineOfOp->bPulsed());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [subPLineOfOp].pulsed! -- store this pipeline's pulsed indicator           */
/*----------------------------------------------------------------------------*/
void Step::vSubPipelineOfOpPulsedStore(void)
{
  ENTRY();
  
  Pipeline* pSubPipelineOfOp = m_pOp->pSubPipelineOfOp();
  
  pSubPipelineOfOp->vPulsed(m_pDataStack->iPopBol(__func__));
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [subPLineOfOp].op0@ -- fetch op0 of this pipeline                          */
/*----------------------------------------------------------------------------*/
void Step::vSubPipelineOfOpOp0Fetch(void)
{
  ENTRY();
  
  Pipeline* pSubPipelineOfOp = m_pOp->pSubPipelineOfOp();

  Op* pOp0 = pSubPipelineOfOp->pOp0();
    
  m_pDataStack->vPushPtr(pOp0);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [subPLineOfOp].opsOnNode@ -- get no of ops executing on this node          */
/*----------------------------------------------------------------------------*/
void Step::vSubPipelineOfOpOpsOnNodeFetch(void)
{
  ENTRY();
    
  Pipeline* pSubPipelineOfOp = m_pOp->pSubPipelineOfOp();

  i8 iThisNode = AM::pGrid()->iThisNodeNo();

  i8 nOps = pSubPipelineOfOp->nOpsOnNode(iThisNode);

  m_pDataStack->vPushInt(nOps);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [subPLineOfOp].spawnOpTasks -- spawn tasks for all ops of pipeline for this node*/
/*----------------------------------------------------------------------------*/
void Step::vSubPipelineOfOpSpawnOpTasks(void)
{
  ENTRY();

  if (AM::TRACE::bVerbose(C_VB_PIPELINE)) {
    i8 iThisNodeNo = MY::iNodeNo();
    vTraceM("Spawning operator jobs for this node %lld\n"_Stz,iThisNodeNo);
  }
  
  Pipeline* pSubPipelineOfOp = m_pOp->pSubPipelineOfOp();
  
  pSubPipelineOfOp->vSpawnOpTasks();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [subPLineOfOp].startPipeline? -- test for issueing a start marker          */
/*----------------------------------------------------------------------------*/
void Step::vSubPipelineOfOpStartPipeline(void)
{
  ENTRY();
  
  Pipeline* pSubPipelineOfOp = m_pOp->pSubPipelineOfOp();
  Pipework* pFormWork = pSubPipelineOfOp->pPipework();

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
