/*============================================================================*/
/* Steps_Pipeline.cpp -- Pipeline step group                                  */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "ePX.h"

#include "Grid.hpp"
#include "Op.hpp"
#include "Stream.hpp"
#include "Stage.hpp"
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
  vAddStep("Pipeline::cancelled@"_Stz,   &Step::vPipelineCancelledFetch);
  vAddStep("Pipeline::cancelled!"_Stz,   &Step::vPipelineCancelledStore);
  vAddStep("Pipeline::compileStmt"_Stz,  &Step::vPipelineCompileStmt);
  vAddStep("Pipeline::create"_Stz,       &Step::vPipelineCreate);
  vAddStep("Pipeline::deleteSpec"_Stz,   &Step::vPipelineDeleteSpec);
  vAddStep("Pipeline::earlyExit@"_Stz,   &Step::vPipelineEarlyExitFetch);
  vAddStep("Pipeline::earlyExit!"_Stz,   &Step::vPipelineEarlyExitStore);
  vAddStep("Pipeline::enqueueStart"_Stz, &Step::vPipelineEnqueueStart);
  vAddStep("Pipeline::errored@"_Stz,     &Step::vPipelineErroredFetch);
  vAddStep("Pipeline::errored!"_Stz,     &Step::vPipelineErroredStore);
  vAddStep("Pipeline::pulsed@"_Stz,      &Step::vPipelinePulsedFetch);
  vAddStep("Pipeline::pulsed!"_Stz,      &Step::vPipelinePulsedStore);
  
  vAddStep("pipeline.pipeout"_Stz,       &Step::vPipelinePipeout);
  
  vAddStep("Pipeline::spawnOpTasks"_Stz, &Step::vPipelineSpawnOpTasks);
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
/* Pipeline::compileStmt -- compile pipeline statement into a pipelines spec  */
/*----------------------------------------------------------------------------*/
void Step::vPipelineCompileStmt(void)
{
  ENTRY();
    
  sSTMT sPipelineStmt;
  m_pDataStack->vPopStr(sPipelineStmt,__func__);
  
  LogicUnit* pLogicUnit = m_p_TaskLogicUnit;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // use dynamo to compile the pipeline statement
  
  Dynamo* pDynamo = new Dynamo(sPipelineStmt,m_pJob);
    
  PipelineSpec* pPipelineSpec = pDynamo->pCompile();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // test compilation result
  
  if (pPipelineSpec) {
    
    eEC eError = AM::JOB::ePreExecuteTest(m_pJob);
    
    if (EC_NONE == eError) {
      
      // a) clean compile, passes pre-execution requirments
      
      if (AM::TRACE::bVerbose(C_VB_PIPELINESPEC)) {
        
        SP_PipelineSpec spPipelineSpec(pPipelineSpec);
        vTraceM("%s\n"_Stz,spPipelineSpec.sz());
      }
      
      pLogicUnit->m_xpPipelineSpec = pPipelineSpec;
      m_pDataStack->vPushInt(EC_NONE);
      
    }
    else {
      
      // b) clean compile, but does not pass pre-execution requirments

      delete pPipelineSpec;
      m_pDataStack->vPushInt(eError);
      
    }

  }
  else {
    
    // c) failed to compile
    
    m_pDataStack->vPushInt(EC_INVALID_COMMAND);

  }
  
  delete pDynamo;
  
  EXIT();
}
/*----------------------------------------------------------------------------*/
//void Step::vPipelineCompileStmt(void)
//{
//  ENTRY();
//
//  sSTMT sPipelineStmt;
//  m_pDataStack->vPopStr(sPipelineStmt,__func__);
//
//
//  LogicUnit* pLogicUnit = m_p_TaskLogicUnit;
//
//  Dynamo* pDynamo = new Dynamo(sPipelineStmt,m_pJob);
//
//  PipelineSpec* pPipelineSpec = pDynamo->pCompile();
//
//
//  if (pPipelineSpec) {
//
//    if (AM::SYSTEM::bPassesLowMem(m_pJob)) {
//
//      // clean compile, passes low memory test
//
//      if (AM::TRACE::bVerbose(C_VB_PIPELINESPEC)) {
//
//        SP_PipelineSpec spPipelineSpec(pPipelineSpec);
//        vTraceM("%s\n"_Stz,spPipelineSpec.sz());
//      }
//
//      pLogicUnit->m_xpPipelineSpec = pPipelineSpec;
//      m_pDataStack->vPushInt(EC_NONE);
//
//    }
//    else {
//
//      // clean compile, but does not pass low memory test
//
//      delete pPipelineSpec;
//      m_pDataStack->vPushInt(EC_LOW_MEMORY);
//
//    }
//
//  }
//  else {
//
//    // failed to compile
//
//    m_pDataStack->vPushInt(EC_INVALID_COMMAND);
//
//  }
//
//  delete pDynamo;
//
//  EXIT();
//}

/*----------------------------------------------------------------------------*/
/* Pipeline::create -- create the pipeline, stages, streams and ops           */
/*----------------------------------------------------------------------------*/
void Step::vPipelineCreate(void)
{
  ENTRY();
    
  i8 iThisNode = AM::pGrid()->iThisNodeNo();
  LogicUnit* pLogicUnit = m_p_TaskLogicUnit;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // create a pipeline
  
  pLogicUnit->m_xpPipeline =
    new Pipeline(m_pJob,
                 *pLogicUnit->m_xpPipelineSpec,
                 m_pJob->iOriginatorNode());
  
  Pipeline* pPipeline = *pLogicUnit->m_xpPipeline;
  
  m_pJob->vPipeline_job(pPipeline);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // dump the pipeline
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
/* Pipeline::deleteSpec -- delete the pipeline spec                           */
/*----------------------------------------------------------------------------*/
void Step::vPipelineDeleteSpec(void)
{
  ENTRY();
  
  LogicUnit* pLogicUnit = m_p_TaskLogicUnit;
  
  if (pLogicUnit->m_xpPipelineSpec.bIsNotNull()) {
    pLogicUnit->m_xpPipelineSpec.vGuardedDeleteComponent(this,(Stz*)__func__);
  }
    
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Pipeline::enqueueStart -- put StartMarker into left queue of start op      */
/*----------------------------------------------------------------------------*/
void Step::vPipelineEnqueueStart(void)
{
  ENTRY();
  
  
  LogicUnit* pLogicUnit = m_p_TaskLogicUnit;
    
  Pipeline* pPipeline = * pLogicUnit->m_xpPipeline;
    
  i8 iThisNode = AM::pGrid()->iThisNodeNo();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get no of operators to be executed on this node
  i8 nOps = pPipeline->nOpsOnNode(iThisNode);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // set acks remaining equal to no of operators to be executed on this node
  if (nOps) {
    if (AM::bDbgTag(C_TAG_DBG_ACKS))
      vTrace("Setting acks remaining to %lld\n"_Stz, nOps);
    AM::vSetAcksRmg(m_pJob->pTask(),nOps);
  
    // get the first stage of the pipeline
    Stage* pStage = pPipeline->pGet(0);
    
    // send startMarker to the start Op of the first stage of the pipeline
    if (pStage->bStartPipeline()) {
      //pStage->vStartPipeline(this);
      
      Stream*  pStream = pStage->pGet(0);

      Op* pOp = pStream->pGet(0);

      RASSERT(pOp->szOpNm()->bSame("start"_Stz));

      Operand* pOperand = new Marker(OM_STARTMARKER);
      
      pOp->vOpTrace(nullptr,(Stz*)__func__,false,"// queue StartMarker into pipeline"_Stz);

      pOp->vAppendLeft(pOperand,this);

    }
    
    // pipeline job to wait for acks
    m_pDataStack->vPushInt(true);
  }
  else {
    // pipeline job has no operators to wait for
    m_pDataStack->vPushInt(false);
  }
  
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

  pPipeline->vEarlyExit(m_pDataStack->iPopInt(__func__));

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
  
  pPipeline->vErrored(m_pDataStack->iPopInt(__func__));
  
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
  
  pPipeline->vPulsed(m_pDataStack->iPopInt(__func__));
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Pipeline::spawnOpTasks -- spawn tasks for all ops of pipeline for this node*/
/*----------------------------------------------------------------------------*/
void Step::vPipelineSpawnOpTasks(void)
{
  ENTRY();

  i8 iThisNode = AM::pGrid()->iThisNodeNo();
  
  if (AM::TRACE::bVerbose(C_VB_PIPELINE))
    vTraceM("Spawning operator jobs for node %lld\n"_Stz,iThisNode);
  
  Pipeline* pPipeline = * m_p_TaskLogicUnit->m_xpPipeline;
  
  pPipeline->vSpawnOpTasks(m_pJob->m_i_Session, iThisNode);
  
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

  Pipeline* pPipeline = pPipelineJob->pPipeline_job();
  
  Stage*    pStage0   = pPipeline->pGet(0);
  Stream*   pStand0   = pStage0->pGet(0);
  Op*       pOp0      = pStand0->pGet(0);

  pOp0->vAppendLeft(pOperand,this);

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
