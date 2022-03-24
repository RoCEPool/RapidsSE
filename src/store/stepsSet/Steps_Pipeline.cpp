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
void Step::vAddSteps_Pipeline(void)
{
  vAddStep("Pipeline[]"_Stz,                &Step::vPipelineGet);
  vAddStep("Pipeline[].set"_Stz,            &Step::vPipelineSet);

  vAddStep("Pipeline[].addAccum"_Stz,       &Step::vPipelineAddAccum);
  vAddStep("Pipeline[].cancelled@"_Stz,     &Step::vPipelineCancelledFetch);
  vAddStep("Pipeline[].cancelled!"_Stz,     &Step::vPipelineCancelledStore);
  vAddStep("Pipeline[].create"_Stz,         &Step::vPipelineCreate);
  vAddStep("Pipeline[].dropDebugLock"_Stz,  &Step::vPipelineDropDebugLock);
  vAddStep("Pipeline[].dump"_Stz,           &Step::vPipelineDump);
  vAddStep("Pipeline[].earlyExit@"_Stz,     &Step::vPipelineEarlyExitFetch);
  vAddStep("Pipeline[].earlyExit!"_Stz,     &Step::vPipelineEarlyExitStore);
  vAddStep("Pipeline[].enqueueStart?"_Stz,  &Step::vPipelineEnqueueStart);
  vAddStep("Pipeline[].errored@"_Stz,       &Step::vPipelineErroredFetch);
  vAddStep("Pipeline[].errored!"_Stz,       &Step::vPipelineErroredStore);
  vAddStep("Pipeline[].getAccum"_Stz,       &Step::vPipelineGetAccum);
  vAddStep("Pipeline[].grabDebugLock"_Stz,  &Step::vPipelineGrabDebugLock);
  vAddStep("Pipeline[].op0@"_Stz,           &Step::vPipelineOp0Fetch);
  vAddStep("Pipeline[].opsOnNode@"_Stz,     &Step::vPipelineOpsOnNodeFetch);
  vAddStep("Pipeline[].pulsed@"_Stz,        &Step::vPipelinePulsedFetch);
  vAddStep("Pipeline[].pulsed!"_Stz,        &Step::vPipelinePulsedStore);
  vAddStep("Pipeline[].spawnOpTasks"_Stz,   &Step::vPipelineSpawnOpTasks);
}

/*----------------------------------------------------------------------------*/
/* Pipeline[].addAccum -- add to an accumulator in this pipeline              */
/*----------------------------------------------------------------------------*/
void Step::vPipelineAddAccum(void)
{
  ENTRY();
    
  i8 i_Accum = m_pDataStack->iPopInt(__func__);
  
  i8 iValue  = m_pDataStack->iPopInt(__func__);
  
  m_pLogicUnit->m_xpPipeline->vAddAccum(i_Accum,iValue);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Pipeline[].cancelled@ -- has this pipeline been cancelled?                 */
/*----------------------------------------------------------------------------*/
void Step::vPipelineCancelledFetch(void)
{
  ENTRY();
  
  // return if pipeline has been cancelled
  m_pDataStack->vPushInt(m_pLogicUnit->m_xpPipeline->bCancelled());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Pipeline[].cancelled! -- fetch this pipeline's cancelled indicator         */
/*----------------------------------------------------------------------------*/
void Step::vPipelineCancelledStore(void)
{
  ENTRY();
  
  m_pLogicUnit->m_xpPipeline->vCancelled(m_pDataStack->iPopInt(__func__));
  
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
  
  // create subpipeline
  
  Pipeline* pSubPipeline =
    new Pipeline(m_pJob,pPipelineSpec,m_pJob->iOriginatorNode());
  
  m_pLogicUnit->m_xpPipeline = pSubPipeline;
    
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
/* Pipeline[].dropDebugLock -- drop this pipeline's debug lock                */
/*----------------------------------------------------------------------------*/
void Step::vPipelineDropDebugLock(void)
{
  ENTRY();
    
  m_pLogicUnit->m_xpPipeline->vDropDebugLock();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Pipeline[].dump -- dump this pipelines and its subpipelines                */
/*----------------------------------------------------------------------------*/
void Step::vPipelineDump(void)
{
  ENTRY();
      
  SP_Pipeline spPipeline;
  
  vTrace("%s\n"_Stz,spPipeline.sz(* m_pLogicUnit->m_xpPipeline,true));
         
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Pipeline[].earlyExit@ -- fetch this pipelines earlyExit indicator          */
/*----------------------------------------------------------------------------*/
void Step::vPipelineEarlyExitFetch(void)
{
  ENTRY();

  // return if pipeline has been errored
  m_pDataStack->vPushInt(m_pLogicUnit->m_xpPipeline->bEarlyExit());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Pipeline[].earlyExit! -- store this pipeline's earlyExit indicator         */
/*----------------------------------------------------------------------------*/
void Step::vPipelineEarlyExitStore(void)
{
  ENTRY();

  m_pLogicUnit->m_xpPipeline->vEarlyExit(m_pDataStack->iPopIntBol(__func__));

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Pipeline[].enqueueStart? -- test for issuing a start marker                */
/*----------------------------------------------------------------------------*/
void Step::vPipelineEnqueueStart(void)
{
  ENTRY();
  
  PipeWork* pFormWork = m_pLogicUnit->m_xpPipeline->pPipework();

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
/* Pipeline[].errored@ -- fetch this pipeline errored indicator               */
/*----------------------------------------------------------------------------*/
void Step::vPipelineErroredFetch(void)
{
  ENTRY();
    
  // return if pipeline has been errored
  m_pDataStack->vPushInt(m_pLogicUnit->m_xpPipeline->bErrored());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Pipeline[].errored! -- store this pipeline errored indicator               */
/*----------------------------------------------------------------------------*/
void Step::vPipelineErroredStore(void)
{
  ENTRY();
  
  m_pLogicUnit->m_xpPipeline->vErrored(m_pDataStack->iPopBol(__func__));
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Pipeline[] -- get pipeline                                                 */
/*----------------------------------------------------------------------------*/
void Step::vPipelineGet(void)
{
  ENTRY();
   
  if (m_pLogicUnit->m_xpPipeline.bIsNull())
    m_pDataStack->vPushPtr(nullptr);
  else
    m_pDataStack->vPushPtr(* m_pLogicUnit->m_xpPipeline);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Pipeline[].getAccum -- get an accumulator from this pipeline               */
/*----------------------------------------------------------------------------*/
void Step::vPipelineGetAccum(void)
{
  ENTRY();
   
  i8 i_Accum = m_pDataStack->iPopInt(__func__);
  
  i8 iValue = m_pLogicUnit->m_xpPipeline->iGetAccum(i_Accum);
  
  m_pDataStack->vPushInt(iValue);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Pipeline[].grabDebugLock -- grab this pipeline's debug lock                */
/*----------------------------------------------------------------------------*/
void Step::vPipelineGrabDebugLock(void)
{
  ENTRY();
    
  m_pLogicUnit->m_xpPipeline->vGrabDebugLock();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Pipeline[].pulsed@ -- fetch this pipeline's pulsed indicator               */
/*----------------------------------------------------------------------------*/
void Step::vPipelinePulsedFetch(void)
{
  ENTRY();
    
  // return if pipeline has been pulsed
  m_pDataStack->vPushInt(m_pLogicUnit->m_xpPipeline->bPulsed());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Pipeline[].pulsed! -- store this pipeline's pulsed indicator               */
/*----------------------------------------------------------------------------*/
void Step::vPipelinePulsedStore(void)
{
  ENTRY();
    
  m_pLogicUnit->m_xpPipeline->vPulsed(m_pDataStack->iPopBol(__func__));
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Pipeline[].op0@ -- fetch op0 of this pipeline                              */
/*----------------------------------------------------------------------------*/
void Step::vPipelineOp0Fetch(void)
{
  ENTRY();
    
  Op* pOp0 = m_pLogicUnit->m_xpPipeline->pOp0();
    
  m_pDataStack->vPushPtr(pOp0);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Pipeline[].opsOnNode@ -- get no of ops executing on this node              */
/*----------------------------------------------------------------------------*/
void Step::vPipelineOpsOnNodeFetch(void)
{
  ENTRY();
    
  i8 iThisNode = AM::pGrid()->iThisNodeNo();

  i8 nOps = m_pLogicUnit->m_xpPipeline->nOpsOnNode(iThisNode);

  m_pDataStack->vPushInt(nOps);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Pipeline[].set -- set pipeline                                             */
/*----------------------------------------------------------------------------*/
void Step::vPipelineSet(void)
{
  ENTRY();
  
  m_pLogicUnit->m_xpPipeline =
    _pPipeline(m_pDataStack->pPopPtr(__func__));

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
    
  m_pLogicUnit->m_xpPipeline->vSpawnOpTasks();

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
