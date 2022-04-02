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
#include "Vertex.hpp"
#include "Pipeline.hpp"
#include "VertexSpec.hpp"
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
void Step::vAddSteps_PipelineOfOp(void)
{
  vAddStep("[pipeline]"_Stz,                &Step::vPipelineOfOpGet);
  
  vAddStep("[pipeline].addAccum"_Stz,       &Step::vPipelineOfOpAddAccum);
  vAddStep("[pipeline].cancelled@"_Stz,     &Step::vPipelineOfOpCancelledFetch);
  vAddStep("[pipeline].cancelled!"_Stz,     &Step::vPipelineOfOpCancelledStore);
  vAddStep("[pipeline].create"_Stz,         &Step::vPipelineOfOpCreate);
  vAddStep("[pipeline].dropDebugLock"_Stz,  &Step::vPipelineOfOpDropDebugLock);
  vAddStep("[pipeline].earlyExit@"_Stz,     &Step::vPipelineOfOpEarlyExitFetch);
  vAddStep("[pipeline].earlyExit!"_Stz,     &Step::vPipelineOfOpEarlyExitStore);
  vAddStep("[pipeline].errored@"_Stz,       &Step::vPipelineOfOpErroredFetch);
  vAddStep("[pipeline].errored!"_Stz,       &Step::vPipelineOfOpErroredStore);
  vAddStep("[pipeline].getAccum"_Stz,       &Step::vPipelineOfOpGetAccum);
  vAddStep("[pipeline].grabDebugLock"_Stz,  &Step::vPipelineOfOpGrabDebugLock);
  vAddStep("[pipeline].op0@"_Stz,           &Step::vPipelineOfOpOp0Fetch);
  vAddStep("[pipeline].opsOnNode@"_Stz,     &Step::vPipelineOfOpOpsOnNodeFetch);
  vAddStep("[pipeline].pulsed@"_Stz,        &Step::vPipelineOfOpPulsedFetch);
  vAddStep("[pipeline].pulsed!"_Stz,        &Step::vPipelineOfOpPulsedStore);
  vAddStep("[pipeline].spawnOpTasks"_Stz,   &Step::vPipelineOfOpSpawnOpTasks);
  vAddStep("[pipeline].enqueueStart?"_Stz, &Step::vPipelineOfOpStartPipeline);
}

/*----------------------------------------------------------------------------*/
/* [pipeline].addAccum -- add to an accumulator in this pipeline          */
/*----------------------------------------------------------------------------*/
void Step::vPipelineOfOpAddAccum(void)
{
  ENTRY();
  
  Pipeline* pPipelineOfOp = m_pOp->pPipelineOfOp();
  
  i8 i_Accum = m_pDataStack->iPopInt(__func__);
  
  i8 iValue  = m_pDataStack->iPopInt(__func__);
  
  pPipelineOfOp->vAddAccum(i_Accum,iValue);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [pipeline].cancelled@ -- has this pipeline been cancelled?             */
/*----------------------------------------------------------------------------*/
void Step::vPipelineOfOpCancelledFetch(void)
{
  ENTRY();
  
  Pipeline* pPipelineOfOp = m_pOp->pPipelineOfOp();
  
  m_pDataStack->vPushInt(pPipelineOfOp->bCancelled());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [pipeline].cancelled! -- fetch this pipeline's cancelled indicator     */
/*----------------------------------------------------------------------------*/
void Step::vPipelineOfOpCancelledStore(void)
{
  ENTRY();

  Pipeline* pPipelineOfOp = m_pOp->pPipelineOfOp();
  
  pPipelineOfOp->vCancelled(m_pDataStack->iPopInt(__func__));
  
  EXIT();
}


/*----------------------------------------------------------------------------*/
/* [pipeline].create -- create the pipeline, stages, streams and ops      */
/*----------------------------------------------------------------------------*/
void Step::vPipelineOfOpCreate(void)
{
  ENTRY();
    
  i8 iThisNode = AM::pGrid()->iThisNodeNo();

  // get pipeline spec
  
  VertexSpec* pPipeSpec = _pVertexSpec(m_pDataStack->pPopPtr(__func__));
  
  // create pipeline
  
  Pipeline* pPipelineOfOp =
    new Pipeline(m_pJob,pPipeSpec,m_pJob->iOriginatorNodeNo());
  
  m_pLogicUnit->m_xpPipeline = pPipelineOfOp;
    
  // dump pipeline
  
  if (AM::TRACE::bVerbose(C_VB_PIPELINE)) {
    if (iThisNode == 0) {
      SP_Pipeline spPipeline(pPipelineOfOp,true);
      vTraceM("%s\n"_Stz,spPipeline.sz());
      vDebugPoint();
    }
  }
    
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [pipeline].dropDebugLock -- drop this pipeline's debug lock            */
/*----------------------------------------------------------------------------*/
void Step::vPipelineOfOpDropDebugLock(void)
{
  ENTRY();
  
  Pipeline* pPipelineOfOp = m_pOp->pPipelineOfOp();
  
  pPipelineOfOp->vDropDebugLock();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [pipeline].earlyExit@ -- fetch this pipelines earlyExit indicator      */
/*----------------------------------------------------------------------------*/
void Step::vPipelineOfOpEarlyExitFetch(void)
{
  ENTRY();

  Pipeline* pPipelineOfOp = m_pOp->pPipelineOfOp();

  // return if pipeline has been errored
  m_pDataStack->vPushInt(pPipelineOfOp->bEarlyExit());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [pipeline].earlyExit! -- store this pipeline's earlyExit indicator     */
/*----------------------------------------------------------------------------*/
void Step::vPipelineOfOpEarlyExitStore(void)
{
  ENTRY();

  Pipeline* pPipelineOfOp = m_pOp->pPipelineOfOp();

  pPipelineOfOp->vEarlyExit(m_pDataStack->iPopIntBol(__func__));

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [pipeline].errored@ -- fetch this pipeline errored indicator           */
/*----------------------------------------------------------------------------*/
void Step::vPipelineOfOpErroredFetch(void)
{
  ENTRY();
  
  Pipeline* pPipelineOfOp = m_pOp->pPipelineOfOp();
  
  // return if pipeline has been errored
  m_pDataStack->vPushInt(pPipelineOfOp->bErrored());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [pipeline].errored! -- store this pipeline errored indicator           */
/*----------------------------------------------------------------------------*/
void Step::vPipelineOfOpErroredStore(void)
{
  ENTRY();
  
  Pipeline* pPipelineOfOp = m_pOp->pPipelineOfOp();
  
  pPipelineOfOp->vErrored(m_pDataStack->iPopBol(__func__));
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [pipeline].get -- get this pipeline                                    */
/*----------------------------------------------------------------------------*/
void Step::vPipelineOfOpGet(void)
{
  ENTRY();
  
  if (m_pOp) {
    Pipeline* pPipelineOfOp = m_pOp->pPipelineOfOp();
    m_pDataStack->vPushPtr(pPipelineOfOp);
  }
  else {
    m_pDataStack->vPushPtr(nullptr);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [pipeline].getAccum -- get an accumulator from this pipeline           */
/*----------------------------------------------------------------------------*/
void Step::vPipelineOfOpGetAccum(void)
{
  ENTRY();
  
  Pipeline* pPipelineOfOp = m_pOp->pPipelineOfOp();
  
  i8 i_Accum = m_pDataStack->iPopInt(__func__);
  
  i8 iValue = pPipelineOfOp->iGetAccum(i_Accum);
  
  m_pDataStack->vPushInt(iValue);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [pipeline].grabDebugLock -- grab this pipeline's debug lock            */
/*----------------------------------------------------------------------------*/
void Step::vPipelineOfOpGrabDebugLock(void)
{
  ENTRY();
  
  Pipeline* pPipelineOfOp = m_pOp->pPipelineOfOp();
  
  pPipelineOfOp->vGrabDebugLock();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [pipeline].pulsed@ -- fetch this pipeline's pulsed indicator           */
/*----------------------------------------------------------------------------*/
void Step::vPipelineOfOpPulsedFetch(void)
{
  ENTRY();
  
  Pipeline* pPipelineOfOp = m_pOp->pPipelineOfOp();
  
  // return if pipeline has been pulsed
  m_pDataStack->vPushInt(pPipelineOfOp->bPulsed());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [pipeline].pulsed! -- store this pipeline's pulsed indicator           */
/*----------------------------------------------------------------------------*/
void Step::vPipelineOfOpPulsedStore(void)
{
  ENTRY();
  
  Pipeline* pPipelineOfOp = m_pOp->pPipelineOfOp();
  
  pPipelineOfOp->vPulsed(m_pDataStack->iPopBol(__func__));
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [pipeline].op0@ -- fetch op0 of this pipeline                          */
/*----------------------------------------------------------------------------*/
void Step::vPipelineOfOpOp0Fetch(void)
{
  ENTRY();
  
  Pipeline* pPipelineOfOp = m_pOp->pPipelineOfOp();

  Op* pOp0 = pPipelineOfOp->pOp0();
    
  m_pDataStack->vPushPtr(pOp0);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [pipeline].opsOnNode@ -- get no of ops executing on this node          */
/*----------------------------------------------------------------------------*/
void Step::vPipelineOfOpOpsOnNodeFetch(void)
{
  ENTRY();
    
  Pipeline* pPipelineOfOp = m_pOp->pPipelineOfOp();

  i8 iThisNode = AM::pGrid()->iThisNodeNo();

  i8 nOps = pPipelineOfOp->nOpsOnNode(iThisNode);

  m_pDataStack->vPushInt(nOps);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [pipeline].spawnOpTasks -- spawn tasks for all ops of pipeline for this node*/
/*----------------------------------------------------------------------------*/
void Step::vPipelineOfOpSpawnOpTasks(void)
{
  ENTRY();

  if (AM::TRACE::bVerbose(C_VB_PIPELINE)) {
    i8 iThisNodeNo = MY::iNodeNo();
    vTraceM("Spawning operator jobs for this node %lld\n"_Stz,iThisNodeNo);
  }
  
  Pipeline* pPipelineOfOp = m_pOp->pPipelineOfOp();
  
  pPipelineOfOp->vSpawnOpTasks();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [pipeline].enqueueStart? -- test for issuing a start marker            */
/*----------------------------------------------------------------------------*/
void Step::vPipelineOfOpStartPipeline(void)
{
  ENTRY();
  
  Pipeline* pPipelineOfOp = m_pOp->pPipelineOfOp();
  Vertex* pVortext = pPipelineOfOp->pVertexRoot();

  Stage*    pStage0   = pVortext->pStage0();

  if (pStage0->bEnqueueInvoke()) {
    
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
