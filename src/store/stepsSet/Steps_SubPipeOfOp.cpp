/*============================================================================*/
/* Steps_SubPipeOfOp.cpp -- SubPipeOfOp step group                            */
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
void Step::vAddSteps_SubPipeOfOp(void)
{
  vAddStep("[subPipeOfOp]"_Stz,                &Step::vSubPipeOfOpGet);
  
  vAddStep("[subPipeOfOp].addAccum"_Stz,       &Step::vSubPipeOfOpAddAccum);
  vAddStep("[subPipeOfOp].cancelled@"_Stz,     &Step::vSubPipeOfOpCancelledFetch);
  vAddStep("[subPipeOfOp].cancelled!"_Stz,     &Step::vSubPipeOfOpCancelledStore);
  vAddStep("[subPipeOfOp].create"_Stz,         &Step::vSubPipeOfOpCreate);
  vAddStep("[subPipeOfOp].dropDebugLock"_Stz,  &Step::vSubPipeOfOpDropDebugLock);
  vAddStep("[subPipeOfOp].earlyExit@"_Stz,     &Step::vSubPipeOfOpEarlyExitFetch);
  vAddStep("[subPipeOfOp].earlyExit!"_Stz,     &Step::vSubPipeOfOpEarlyExitStore);
  vAddStep("[subPipeOfOp].enqueueStart?"_Stz,  &Step::vSubPipeOfOpStartPipeline);
  vAddStep("[subPipeOfOp].errored@"_Stz,       &Step::vSubPipeOfOpErroredFetch);
  vAddStep("[subPipeOfOp].errored!"_Stz,       &Step::vSubPipeOfOpErroredStore);
  vAddStep("[subPipeOfOp].getAccum"_Stz,       &Step::vSubPipeOfOpGetAccum);
  vAddStep("[subPipeOfOp].grabDebugLock"_Stz,  &Step::vSubPipeOfOpGrabDebugLock);
  vAddStep("[subPipeOfOp].op0@"_Stz,           &Step::vSubPipeOfOpOp0Fetch);
  vAddStep("[subPipeOfOp].opsOnNode@"_Stz,     &Step::vSubPipeOfOpOpsOnNodeFetch);
  vAddStep("[subPipeOfOp].pulsed@"_Stz,        &Step::vSubPipeOfOpPulsedFetch);
  vAddStep("[subPipeOfOp].pulsed!"_Stz,        &Step::vSubPipeOfOpPulsedStore);
  vAddStep("[subPipeOfOp].spawnOpTasks"_Stz,   &Step::vSubPipeOfOpSpawnOpTasks);
}

/*----------------------------------------------------------------------------*/
/* [subPipeOfOp].addAccum -- add to an accumulator in this pipeline           */
/*----------------------------------------------------------------------------*/
void Step::vSubPipeOfOpAddAccum(void)
{
  ENTRY();
  
  Pipeline* pSubPipelineOfOp = m_pOp->pSubPipeOfOp();
  
  i8 i_Accum = m_pDataStack->iPopInt(__func__);
  
  i8 iValue  = m_pDataStack->iPopInt(__func__);
  
  pSubPipelineOfOp->vAddAccum(i_Accum,iValue);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [subPipeOfOp].cancelled@ -- has this pipeline been cancelled?              */
/*----------------------------------------------------------------------------*/
void Step::vSubPipeOfOpCancelledFetch(void)
{
  ENTRY();
  
  Pipeline* pSubPipelineOfOp = m_pOp->pSubPipeOfOp();
  
  m_pDataStack->vPushInt(pSubPipelineOfOp->bCancelled());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [subPipeOfOp].cancelled! -- fetch this pipeline's cancelled indicator      */
/*----------------------------------------------------------------------------*/
void Step::vSubPipeOfOpCancelledStore(void)
{
  ENTRY();

  Pipeline* pSubPipelineOfOp = m_pOp->pSubPipeOfOp();
  
  pSubPipelineOfOp->vCancelled(m_pDataStack->iPopInt(__func__));
  
  EXIT();
}


/*----------------------------------------------------------------------------*/
/* [subPipeOfOp].create -- create the pipeline, stages, streams and ops       */
/*----------------------------------------------------------------------------*/
void Step::vSubPipeOfOpCreate(void)
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
/* [subPipeOfOp].dropDebugLock -- drop this pipeline's debug lock             */
/*----------------------------------------------------------------------------*/
void Step::vSubPipeOfOpDropDebugLock(void)
{
  ENTRY();
  
  Pipeline* pSubPipelineOfOp = m_pOp->pSubPipeOfOp();
  
  pSubPipelineOfOp->vDropDebugLock();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [subPipeOfOp].earlyExit@ -- fetch this pipelines earlyExit indicator       */
/*----------------------------------------------------------------------------*/
void Step::vSubPipeOfOpEarlyExitFetch(void)
{
  ENTRY();

  Pipeline* pSubPipelineOfOp = m_pOp->pSubPipeOfOp();

  // return if pipeline has been errored
  m_pDataStack->vPushInt(pSubPipelineOfOp->bEarlyExit());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [subPipeOfOp].earlyExit! -- store this pipeline's earlyExit indicator      */
/*----------------------------------------------------------------------------*/
void Step::vSubPipeOfOpEarlyExitStore(void)
{
  ENTRY();

  Pipeline* pSubPipelineOfOp = m_pOp->pSubPipeOfOp();

  pSubPipelineOfOp->vEarlyExit(m_pDataStack->iPopIntBol(__func__));

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [subPipeOfOp].errored@ -- fetch this pipeline errored indicator            */
/*----------------------------------------------------------------------------*/
void Step::vSubPipeOfOpErroredFetch(void)
{
  ENTRY();
  
  Pipeline* pSubPipelineOfOp = m_pOp->pSubPipeOfOp();
  
  // return if pipeline has been errored
  m_pDataStack->vPushInt(pSubPipelineOfOp->bErrored());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [subPipeOfOp].errored! -- store this pipeline errored indicator            */
/*----------------------------------------------------------------------------*/
void Step::vSubPipeOfOpErroredStore(void)
{
  ENTRY();
  
  Pipeline* pSubPipelineOfOp = m_pOp->pSubPipeOfOp();
  
  pSubPipelineOfOp->vErrored(m_pDataStack->iPopBol(__func__));
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [subPipeOfOp].get -- get this pipeline                                     */
/*----------------------------------------------------------------------------*/
void Step::vSubPipeOfOpGet(void)
{
  ENTRY();
  
  if (m_pOp) {
    Pipeline* pSubPipelineOfOp = m_pOp->pSubPipeOfOp();
    m_pDataStack->vPushPtr(pSubPipelineOfOp);
  }
  else {
    m_pDataStack->vPushPtr(nullptr);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [subPipeOfOp].getAccum -- get an accumulator from this pipeline            */
/*----------------------------------------------------------------------------*/
void Step::vSubPipeOfOpGetAccum(void)
{
  ENTRY();
  
  Pipeline* pSubPipelineOfOp = m_pOp->pSubPipeOfOp();
  
  i8 i_Accum = m_pDataStack->iPopInt(__func__);
  
  i8 iValue = pSubPipelineOfOp->iGetAccum(i_Accum);
  
  m_pDataStack->vPushInt(iValue);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [subPipeOfOp].grabDebugLock -- grab this pipeline's debug lock             */
/*----------------------------------------------------------------------------*/
void Step::vSubPipeOfOpGrabDebugLock(void)
{
  ENTRY();
  
  Pipeline* pSubPipelineOfOp = m_pOp->pSubPipeOfOp();
  
  pSubPipelineOfOp->vGrabDebugLock();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [subPipeOfOp].pulsed@ -- fetch this pipeline's pulsed indicator            */
/*----------------------------------------------------------------------------*/
void Step::vSubPipeOfOpPulsedFetch(void)
{
  ENTRY();
  
  Pipeline* pSubPipelineOfOp = m_pOp->pSubPipeOfOp();
  
  // return if pipeline has been pulsed
  m_pDataStack->vPushInt(pSubPipelineOfOp->bPulsed());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [subPipeOfOp].pulsed! -- store this pipeline's pulsed indicator            */
/*----------------------------------------------------------------------------*/
void Step::vSubPipeOfOpPulsedStore(void)
{
  ENTRY();
  
  Pipeline* pSubPipelineOfOp = m_pOp->pSubPipeOfOp();
  
  pSubPipelineOfOp->vPulsed(m_pDataStack->iPopBol(__func__));
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [subPipeOfOp].op0@ -- fetch op0 of this pipeline                           */
/*----------------------------------------------------------------------------*/
void Step::vSubPipeOfOpOp0Fetch(void)
{
  ENTRY();
  
  Pipeline* pSubPipelineOfOp = m_pOp->pSubPipeOfOp();

  Op* pOp0 = pSubPipelineOfOp->pOp0();
    
  m_pDataStack->vPushPtr(pOp0);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [subPipeOfOp].opsOnNode@ -- get no of ops executing on this node           */
/*----------------------------------------------------------------------------*/
void Step::vSubPipeOfOpOpsOnNodeFetch(void)
{
  ENTRY();
    
  Pipeline* pSubPipelineOfOp = m_pOp->pSubPipeOfOp();

  i8 iThisNode = AM::pGrid()->iThisNodeNo();

  i8 nOps = pSubPipelineOfOp->nOpsOnNode(iThisNode);

  m_pDataStack->vPushInt(nOps);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [subPipeOfOp].spawnOpTasks -- spawn tasks for all ops of pipeline for this node*/
/*----------------------------------------------------------------------------*/
void Step::vSubPipeOfOpSpawnOpTasks(void)
{
  ENTRY();

  if (AM::TRACE::bVerbose(C_VB_PIPELINE)) {
    i8 iThisNodeNo = MY::iNodeNo();
    vTraceM("Spawning operator jobs for this node %lld\n"_Stz,iThisNodeNo);
  }
  
  Pipeline* pSubPipelineOfOp = m_pOp->pSubPipeOfOp();
  
  pSubPipelineOfOp->vSpawnOpTasks();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [subPipeOfOp].enqueueStart? -- test for issuing a start marker             */
/*----------------------------------------------------------------------------*/
void Step::vSubPipeOfOpStartPipeline(void)
{
  ENTRY();
  
  Pipeline* pSubPipelineOfOp = m_pOp->pSubPipeOfOp();
  PipeWork* pFormWork = pSubPipelineOfOp->pPipework();

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
