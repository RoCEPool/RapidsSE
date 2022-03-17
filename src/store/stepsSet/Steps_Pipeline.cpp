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
#include "Formwork.hpp"
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
  
  vAddStep("pipeline.pipeout"_Stz,        &Step::vPipelinePipeout);
    
  vAddStep("Pipeline[].create"_Stz,       &Step::vPipelineCreate);
  vAddStep("Pipeline[].op0@"_Stz,         &Step::vPipelineOp0Fetch);
  vAddStep("Pipeline[].opsOnNode@"_Stz,   &Step::vPipelineOpsOnNodeFetch);
  vAddStep("Pipeline[].spawnOpTasks"_Stz, &Step::vPipelineSpawnOpTasks);
  vAddStep("Pipeline[].startPipeline"_Stz,&Step::vPipelineStartPipeline);
}

/*----------------------------------------------------------------------------*/
/* Pipeline::addAccum -- add to an accumulator                                */
/*----------------------------------------------------------------------------*/
void Step::vPipelineAddAccum(void)
{
  ENTRY();
  
  Op* pOp = *m_pOp;
  Pipeline* pPipeline = pOp->pPipeline_op();
  
  i8 i_Accum = m_pDataStack->iPopInt(__func__);
  
  i8 iValue  = m_pDataStack->iPopInt(__func__);
  
  pPipeline->vAddAccum(i_Accum,iValue);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Pipeline::cancelled@ -- has the pipeline been cancelled?                   */
/*----------------------------------------------------------------------------*/
void Step::vPipelineCancelledFetch(void)
{
  ENTRY();
  
  // revised job/op model
    Op* pOp = *m_pOp;

  Pipeline* pPipeline = pOp->pPipeline_op();
  
  // return if pipeline has been cancelled
  m_pDataStack->vPushInt(pPipeline->bCancelled());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Pipeline::cancelled! -- set pipeline as being cancelled                    */
/*----------------------------------------------------------------------------*/
void Step::vPipelineCancelledStore(void)
{
  ENTRY();
  
  // revised job/op model
    Op* pOp = *m_pOp;

  Pipeline* pPipeline = pOp->pPipeline_op();
  
  pPipeline->vCancelled(m_pDataStack->iPopInt(__func__));
  
  EXIT();
}


/*----------------------------------------------------------------------------*/
/* Pipeline[].create -- create the pipeline, stages, streams and ops           */
/*----------------------------------------------------------------------------*/
void Step::vPipelineCreate(void)
{
  ENTRY();
    
  i8 iThisNode = AM::pGrid()->iThisNodeNo();

  // get pipeline spec
  
  PipelineSpec* pPipelineSpec = _pPipelineSpec(m_pDataStack->pPopPtr(__func__));
  
  // create pipeline
  
  Pipeline* pPipeline =
    new Pipeline(m_pJob_task,pPipelineSpec,m_pJob_task->iOriginatorNode());
  
  m_p_TaskLogicUnit->m_xpPipeline = pPipeline;
    
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
/* Pipeline::dropDebugLock -- drop the pipeline debug lock                    */
/*----------------------------------------------------------------------------*/
void Step::vPipelineDropDebugLock(void)
{
  ENTRY();
  
  Op* pOp = *m_pOp;
  Pipeline* pPipeline = pOp->pPipeline_op();
  
  pPipeline->vDropDebugLock();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Pipeline::earlyExit@ -- has the pipeline been early exited                 */
/*----------------------------------------------------------------------------*/
void Step::vPipelineEarlyExitFetch(void)
{
  ENTRY();

  // revised job/op model
    Op* pOp = *m_pOp;

  Pipeline* pPipeline = pOp->pPipeline_op();

  // return if pipeline has been errored
  m_pDataStack->vPushInt(pPipeline->bEarlyExit());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Pipeline::earlyExit! -- set earlyExit                                      */
/*----------------------------------------------------------------------------*/
void Step::vPipelineEarlyExitStore(void)
{
  ENTRY();

  // revised job/op model
    Op* pOp = *m_pOp;

  Pipeline* pPipeline = pOp->pPipeline_op();

  pPipeline->vEarlyExit(m_pDataStack->iPopIntBol(__func__));

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Pipeline::errored@ -- has the pipeline been errored                        */
/*----------------------------------------------------------------------------*/
void Step::vPipelineErroredFetch(void)
{
  ENTRY();
  
  // revised job/op model
    Op* pOp = *m_pOp;

  Pipeline* pPipeline = pOp->pPipeline_op();
  
  // return if pipeline has been errored
  m_pDataStack->vPushInt(pPipeline->bErrored());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Pipeline::errored! -- set pipeline as being errored                        */
/*----------------------------------------------------------------------------*/
void Step::vPipelineErroredStore(void)
{
  ENTRY();
  
  // revised job/op model
    Op* pOp = *m_pOp;
  Pipeline* pPipeline = pOp->pPipeline_op();
  
  pPipeline->vErrored(m_pDataStack->iPopBol(__func__));
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Pipeline::getAccum -- get an accumulator                                   */
/*----------------------------------------------------------------------------*/
void Step::vPipelineGetAccum(void)
{
  ENTRY();
  
  Op* pOp = *m_pOp;
  Pipeline* pPipeline = pOp->pPipeline_op();
  
  i8 i_Accum = m_pDataStack->iPopInt(__func__);
  
  i8 iValue = pPipeline->iGetAccum(i_Accum);
  
  m_pDataStack->vPushInt(iValue);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Pipeline::grabDebugLock -- grab the pipeline debug lock                    */
/*----------------------------------------------------------------------------*/
void Step::vPipelineGrabDebugLock(void)
{
  ENTRY();
  
  Op* pOp = *m_pOp;
  Pipeline* pPipeline = pOp->pPipeline_op();
  
  pPipeline->vGrabDebugLock();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Pipeline::pulsed@ -- has the pipeline been pulsed                          */
/*----------------------------------------------------------------------------*/
void Step::vPipelinePulsedFetch(void)
{
  ENTRY();
  
  // revised job/op model
    Op* pOp = *m_pOp;

  Pipeline* pPipeline = pOp->pPipeline_op();
  
  // return if pipeline has been pulsed
  m_pDataStack->vPushInt(pPipeline->bPulsed());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Pipeline::pulsed! -- set pipeline as being pulsed                          */
/*----------------------------------------------------------------------------*/
void Step::vPipelinePulsedStore(void)
{
  ENTRY();
  
  // revised job/op model
    Op* pOp = *m_pOp;

  Pipeline* pPipeline = pOp->pPipeline_op();
  
  pPipeline->vPulsed(m_pDataStack->iPopBol(__func__));
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Pipeline[].op0@ -- fetch op0 of this pipeline                              */
/*----------------------------------------------------------------------------*/
void Step::vPipelineOp0Fetch(void)
{
  ENTRY();
  
  Pipeline* pPipeline = (* m_p_TaskLogicUnit->m_xpPipeline);
  
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
  
//  Pipeline* pPipeline = (* m_p_TaskLogicUnit->m_xpPipeline);
//  Formwork* pFormWork = pPipeline->pFormwork();
//
//  Stage*    pStage0   = pFormWork->pGet(0);
//
//  if (pStage0->bStartPipeline()) {
  
    Pipeline* pPipeline = (* m_p_TaskLogicUnit->m_xpPipeline);

    i8 iThisNode = AM::pGrid()->iThisNodeNo();

    i8 nOps = pPipeline->nOpsOnNode(iThisNode);
  
    m_pDataStack->vPushInt(nOps);
//  }
//  else {
//    m_pDataStack->vPushInt(0);
//  }

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Pipeline[].spawnOpTasks -- spawn tasks for all ops of pipeline for this node*/
/*----------------------------------------------------------------------------*/
void Step::vPipelineSpawnOpTasks(void)
{
  ENTRY();

  i8 iThisNode = AM::pGrid()->iThisNodeNo();
  
  if (AM::TRACE::bVerbose(C_VB_PIPELINE))
    vTraceM("Spawning operator jobs for node %lld\n"_Stz,iThisNode);
  
  Pipeline* pPipeline = * m_p_TaskLogicUnit->m_xpPipeline;
  
  pPipeline->vSpawnOpTasks(m_pJob_task->m_i_Session, iThisNode);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Pipeline[].startPipeline -- test for issueing a start marker               */
/*----------------------------------------------------------------------------*/
void Step::vPipelineStartPipeline(void)
{
  ENTRY();
  
  Pipeline* pPipeline = (* m_p_TaskLogicUnit->m_xpPipeline);
  Formwork* pFormWork = pPipeline->pFormwork();

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


/*----------------------------------------------------------------------------*/
/* pipeline.pipeout -- append operand onto next operator in pipeline          */
/*----------------------------------------------------------------------------*/
void Step::vPipelinePipeout(void)
{
  ENTRY();

  Job* pPipelineJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  Operand* pOperand = _pOperand(m_pDataStack->pPopPtr(__func__));
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // !!!!!!!!!!!!!!
  // move to AM

  Pipeline* pPipeline = pPipelineJob->pPipeline_job();
  Formwork* pFormWork = pPipeline->pFormwork();

  Stage*    pStage0   = pFormWork->pGet(0);
  Stream*   pStand0   = pStage0->pGet(0);
  Op*       pOp0      = pStand0->pGet(0);

  pOp0->vAppendLeft(pOperand,this);

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
