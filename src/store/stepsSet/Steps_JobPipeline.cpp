/*============================================================================*/
/* Steps_JobPipeline.cpp - JobPipeline step group                             */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "ePX.h"

#include "Grid.hpp"
#include "Op.hpp"
#include "Stage.hpp"
#include "Pipeline.hpp"
#include "PipelineSpec.hpp"
#include "Job.hpp"
#include "Node_I.hpp"
#include "LogicUnit.hpp"

#include "Dynamo.hpp"
#include "TagConstants.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_JobPipeline(void)
{
  vAddStep("Job_Pipeline::begin"_Stz,       &Step::vJob_PipelineBegin);
  vAddStep("Job_Pipeline::compile"_Stz,     &Step::vJob_PipelineCompile);

  vAddStep("Op_Pipeline::cancelled"_Stz,    &Step::vJob_PipelineCancelled);
  vAddStep("Op_Pipeline::deleteSpec"_Stz,   &Step::vJob_PipelineDeleteSpec);
  vAddStep("Op_Pipeline::earlyExit@"_Stz,   &Step::vJob_PipelineEarlyExitFetch);
  vAddStep("Op_Pipeline::earlyExit!"_Stz,   &Step::vJob_PipelineEarlyExitStore);
  vAddStep("Op_Pipeline::errored"_Stz,      &Step::vJob_PipelineErrored);
  vAddStep("Op_Pipeline::execute"_Stz,      &Step::vJob_PipelineExecute);
  vAddStep("Op_Pipeline::isCancelled"_Stz,  &Step::vJob_PipelineIsCancelled);
  vAddStep("Op_Pipeline::isErrored"_Stz,    &Step::vJob_PipelineIsErrored);
  vAddStep("Op_Pipeline::isPulsed"_Stz,     &Step::vJob_PipelineIsPulsed);
  vAddStep("Op_Pipeline::pulsed"_Stz,       &Step::vJob_PipelinePulsed);
}

/*----------------------------------------------------------------------------*/
/* Job_Pipeline::begin -- begin the Pipeline job                              */
/*----------------------------------------------------------------------------*/
void Step::vJob_PipelineBegin(void)
{
  ENTRY();
    
  i8 iThisNode = AM::pGrid()->iThisNode();
  LogicUnit* pLogicUnit = m_pJob->pLogicUnit_job();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // create a pipeline
  pLogicUnit->m_xpPipeline =
    new Pipeline(m_pJob,
                 *pLogicUnit->m_xpPipelineSpec,
                 m_pJob->iOriginatorNode());
  
  Pipeline* pPipeline = *pLogicUnit->m_xpPipeline;
      
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // dump the pipeline
  if (AM::bVerbose()) {
    if (iThisNode == 0)
       pPipeline->vDump();
  }
    
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // spawn operators for this node
  if (AM::bVerbose())
    vTrace("Spawning operator jobs for node %lld\n"_Stz, AM::pGrid()->iThisNode());
  
  pPipeline->vSpawnOperatorJobs(m_pJob->m_xReqTuple.m_i_Session, iThisNode);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Job_Pipeline::compile -- compile the command                               */
/*----------------------------------------------------------------------------*/
void Step::vJob_PipelineCompile(void)
{
  ENTRY();
    
  sSTMT sCmd;
  m_pDataStack->vPopStr(sCmd,__func__);
  

  LogicUnit* pLogicUnit = m_pJob->pLogicUnit_job();
  
  Dynamo* pDynamo = new Dynamo(sCmd,m_pJob);
  
  bool bCleanCompile = pDynamo->bCompile();
  
  pLogicUnit->m_xpPipelineSpec = pDynamo->pPipelineSpec();

  if (bCleanCompile) {
    
    if (AM::bPassesLowMem(m_pJob)) {
      
      // clean compile, passes low memory test
      
      m_pDataStack->vPushInt(EC_NONE);
      if (AM::bVerbose()) {
        pDynamo->vShow();
      }
      
    }
    else {
      
      // clean compile, but does not pass low memory test
      

      delete *pLogicUnit->m_xpPipelineSpec;
      pLogicUnit->m_xpPipelineSpec = nullptr;

      m_pDataStack->vPushInt(EC_LOW_MEMORY);
      
    }

  }
  else {
    
    // failed to compile
    
    delete *pLogicUnit->m_xpPipelineSpec;
    pLogicUnit->m_xpPipelineSpec = nullptr;

    m_pDataStack->vPushInt(EC_INVALID_COMMAND);

  }
  
  delete pDynamo;
  
  EXIT();
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Op_Pipeline::cancelled -- set pipeline as being cancelled                  */
/*----------------------------------------------------------------------------*/
void Step::vJob_PipelineCancelled(void)
{
  ENTRY();
  
  Op* pOp = m_pJob->m_pOp;
  
  Pipeline* pPipeline = pOp->pPipeline();
  
  pPipeline->vCancelled(m_pDataStack->iPopInt(__func__));
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Op_Pipeline::deleteSpec -- delete the pipeline spec                        */
/*----------------------------------------------------------------------------*/
void Step::vJob_PipelineDeleteSpec(void)
{
  ENTRY();
  
  LogicUnit* pLogicUnit = m_pJob->pLogicUnit_job();
  
  if (*pLogicUnit->m_xpPipelineSpec) {
    delete *pLogicUnit->m_xpPipelineSpec;
    pLogicUnit->m_xpPipelineSpec = nullptr;
  }
    
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Op_Pipeline::earlyExit! -- set earlyExit                                   */
/*----------------------------------------------------------------------------*/
void Step::vJob_PipelineEarlyExitStore(void)
{
  ENTRY();

  Op* pOp = m_pJob->m_pOp;

  Pipeline* pPipeline = pOp->pPipeline();

  pPipeline->vEarlyExit(m_pDataStack->iPopInt(__func__));

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Op_Pipeline::earlyExit@ -- has the pipeline been early exited              */
/*----------------------------------------------------------------------------*/
void Step::vJob_PipelineEarlyExitFetch(void)
{
  ENTRY();

  Op* pOp = m_pJob->m_pOp;

  Pipeline* pPipeline = pOp->pPipeline();

  // return if pipeline has been errored
  m_pDataStack->vPushInt(pPipeline->bEarlyExit());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Op_Pipeline::errored -- set pipeline as being errored                      */
/*----------------------------------------------------------------------------*/
void Step::vJob_PipelineErrored(void)
{
  ENTRY();
  
  Op* pOp = m_pJob->m_pOp;
  
  Pipeline* pPipeline = pOp->pPipeline();
  
  pPipeline->vErrored(m_pDataStack->iPopInt(__func__));
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Op_Pipeline::execute -- execute                                            */
/*----------------------------------------------------------------------------*/
void Step::vJob_PipelineExecute(void)
{
  ENTRY();
  
  
  LogicUnit* pLogicUnit = m_pJob->pLogicUnit_job();
  Pipeline* pPipeline = *pLogicUnit->m_xpPipeline;
  i8 iThisNode = AM::pGrid()->iThisNode();
  
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
    pStage->vStartPipeline(this);
    
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
/* Op_Pipeline::IsCancelled -- has the pipeline been cancelled                */
/*----------------------------------------------------------------------------*/
void Step::vJob_PipelineIsCancelled(void)
{
  ENTRY();
  
  Op* pOp = m_pJob->m_pOp;
  
  Pipeline* pPipeline = pOp->pPipeline();
  
  // return if pipeline has been cancelled
  m_pDataStack->vPushInt(pPipeline->bCancelled());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Op_Pipeline::IsErrored -- has the pipeline been errored                    */
/*----------------------------------------------------------------------------*/
void Step::vJob_PipelineIsErrored(void)
{
  ENTRY();
  
  Op* pOp = m_pJob->m_pOp;
  
  Pipeline* pPipeline = pOp->pPipeline();
  
  // return if pipeline has been errored
  m_pDataStack->vPushInt(pPipeline->bErrored());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Op_Pipeline::IsPulsed -- has the pipeline been pulsed                      */
/*----------------------------------------------------------------------------*/
void Step::vJob_PipelineIsPulsed(void)
{
  ENTRY();
  
  Op* pOp = m_pJob->m_pOp;
  
  Pipeline* pPipeline = pOp->pPipeline();
  
  // return if pipeline has been pulsed
  m_pDataStack->vPushInt(pPipeline->bPulsed());

EXIT();
}

/*----------------------------------------------------------------------------*/
/* Op_Pipeline::pulsed -- set pipeline as being pulsed                        */
/*----------------------------------------------------------------------------*/
void Step::vJob_PipelinePulsed(void)
{
  ENTRY();
  
  Op* pOp = m_pJob->m_pOp;
  
  Pipeline* pPipeline = pOp->pPipeline();
  
  pPipeline->vPulsed(m_pDataStack->iPopInt(__func__));
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
