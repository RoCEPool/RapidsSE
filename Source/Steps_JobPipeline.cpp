/*============================================================================*/
/* Steps_JobPipeline.cpp - JobPipeline step group                             */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_JobPipeline(void)
{
  vAddStep("Job_Pipeline.begin"_Stz,         &Step::vJob_PipelineBegin);
  vAddStep("Job_Pipeline::cancelled"_Stz,    &Step::vJob_PipelineCancelled);
  vAddStep("Job_Pipeline::compile"_Stz,      &Step::vJob_PipelineCompile);
  vAddStep("Job_Pipeline::deleteDynamo"_Stz, &Step::vJob_PipelineDeleteDynamo);
  vAddStep("Job_Pipeline::errored"_Stz,      &Step::vJob_PipelineErrored);
  vAddStep("Job_Pipeline.execute"_Stz,       &Step::vJob_PipelineExecute);
  vAddStep("Job_Pipeline::isCancelled"_Stz,  &Step::vJob_PipelineIsCancelled);
  vAddStep("Job_Pipeline::isErrored"_Stz,    &Step::vJob_PipelineIsErrored);
  vAddStep("Job_Pipeline::pulsed"_Stz,       &Step::vJob_PipelinePulsed);
  vAddStep("Job_Pipeline::isPulsed"_Stz,     &Step::vJob_PipelineIsPulsed);
}

/*----------------------------------------------------------------------------*/
/* Job_Pipeline.begin -- begin the Pipeline job                               */
/*----------------------------------------------------------------------------*/
void Step::vJob_PipelineBegin(void)
{
  ENTRY();
  
  Job* pJob_Pipeline = _pJob(m_pDataStack->pPopPtr(__func__));
  i8 iThisNode = AM::pGrid()->iThisNode();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // create a pipeline
  pJob_Pipeline->m_pPipeline =
    new Pipeline(pJob_Pipeline,
                 m_pRegSet->m_xpDynamo->pPipelineSpec(),
                 pJob_Pipeline->iOriginatorNode());
  
  Pipeline* pPipeline = pJob_Pipeline->m_pPipeline;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  RASSERT( ! pJob_Pipeline->pBufferQueue() );
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // popluate the op vector of the pipeline
  pPipeline->vPopulateOpVec(m_pRegSet->m_xpDynamo->pPipelineSpec());
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // dump the pipeline
  if (AM::bVerbose()) {
    if (iThisNode == 0)
       pPipeline->vDump();
  }
  
  delete *m_pRegSet->m_xpDynamo;
  m_pRegSet->m_xpDynamo = nullptr;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // spawn operators for this node
  if (AM::bVerbose())
    vTrace("Spawning operator jobs for node %lld\n"_Stz, AM::pGrid()->iThisNode());
  
  pPipeline->vSpawnOperatorJobs(pJob_Pipeline->m_xReqTuple.m_i_Session, iThisNode);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Job_Pipeline::cancelled -- set pipeline as being cancelled                 */
/*----------------------------------------------------------------------------*/
void Step::vJob_PipelineCancelled(void)
{
  ENTRY();
  
  Op* pOp = m_pTask->m_pJob->m_pOp;
  
  Pipeline* pPipeline = pOp->pPipeline();
  
  pPipeline->vCancelled(m_pDataStack->iPopInt(__func__));
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Job_Pipeline::compile -- compile the command                               */
/*----------------------------------------------------------------------------*/
void Step::vJob_PipelineCompile(void)
{
  ENTRY();
  
  s4K sCmd;
  m_pDataStack->vPopStr(sCmd,__func__);
  
  m_pRegSet->m_xpDynamo = new Dynamo(sCmd,m_pTask->m_pJob);
  
  bool bResult = m_pRegSet->m_xpDynamo->bCompile();
  
  if ( ! bResult) {
    PipelineSpec* pPipelineSpec = m_pRegSet->m_xpDynamo->pPipelineSpec();
    delete pPipelineSpec;
    
    delete *m_pRegSet->m_xpDynamo;
    m_pRegSet->m_xpDynamo = nullptr;
    m_pDataStack->vPushInt(EC_INVALID_COMMAND);
  }
  else {
    m_pDataStack->vPushInt(EC_NONE);
    if (AM::bVerbose()) {
      m_pRegSet->m_xpDynamo->vShow();
    }
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Job_Pipeline::deleteDynamo -- delete the dynamo                            */
/*----------------------------------------------------------------------------*/
void Step::vJob_PipelineDeleteDynamo(void)
{
  ENTRY();
  
  PipelineSpec* pPipelineSpec = m_pRegSet->m_xpDynamo->pPipelineSpec();
  delete pPipelineSpec;
  
  delete *m_pRegSet->m_xpDynamo;
  m_pRegSet->m_xpDynamo = nullptr;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Job_Pipeline::errored -- set pipeline as being errored                     */
/*----------------------------------------------------------------------------*/
void Step::vJob_PipelineErrored(void)
{
  ENTRY();
  
  Op* pOp = m_pTask->m_pJob->m_pOp;
  
  Pipeline* pPipeline = pOp->pPipeline();
  
  pPipeline->vErrored(m_pDataStack->iPopInt(__func__));
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Job_Pipeline.execute -- execute                                            */
/*----------------------------------------------------------------------------*/
void Step::vJob_PipelineExecute(void)
{
  ENTRY();
  
  Job* pJob_Pipeline = _pJob(m_pDataStack->pPopPtr(__func__));
  
  Pipeline* pPipeline = pJob_Pipeline->m_pPipeline;
  i8 iThisNode = AM::pGrid()->iThisNode();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get no of operators to be executed on this node
  i8 nOps = pPipeline->nOpsOnNode(iThisNode);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // set acks remaining equal to no of operators to be executed on this node
  if (nOps) {
    if (AM::bDbgTag(C_TAG_DBG_ACKS))
      vTrace("Setting acks remaining to %lld\n"_Stz, nOps);
    AM::vSetAcksRmg(pJob_Pipeline->pTask(),nOps);
  
    // get the first stage of the pipeline
    Stage* pStage = pPipeline->pGet(0);
    
    // send startMarker to the start Op of the first stage of the pipeline
    pStage->vStartPipeline(m_pTask);
    
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
/* Job_Pipeline::IsCancelled -- has the pipeline been cancelled               */
/*----------------------------------------------------------------------------*/
void Step::vJob_PipelineIsCancelled(void)
{
  ENTRY();
  
  Op* pOp = m_pTask->m_pJob->m_pOp;
  
  Pipeline* pPipeline = pOp->pPipeline();
  
  // return if pipeline has been cancelled
  m_pDataStack->vPushInt(pPipeline->bCancelled());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Job_Pipeline::IsErrored -- has the pipeline been errored                   */
/*----------------------------------------------------------------------------*/
void Step::vJob_PipelineIsErrored(void)
{
  ENTRY();
  
  Op* pOp = m_pTask->m_pJob->m_pOp;
  
  Pipeline* pPipeline = pOp->pPipeline();
  
  // return if pipeline has been errored
  m_pDataStack->vPushInt(pPipeline->bErrored());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Job_Pipeline::IsPulsed -- has the pipeline been pulsed                     */
/*----------------------------------------------------------------------------*/
void Step::vJob_PipelineIsPulsed(void)
{
  ENTRY();
  
  Op* pOp = m_pTask->m_pJob->m_pOp;
  
  Pipeline* pPipeline = pOp->pPipeline();
  
  // return if pipeline has been pulsed
  m_pDataStack->vPushInt(pPipeline->bPulsed());

EXIT();
}

/*----------------------------------------------------------------------------*/
/* Job_Pipeline::pulsed -- set pipeline as being pulsed                       */
/*----------------------------------------------------------------------------*/
void Step::vJob_PipelinePulsed(void)
{
  ENTRY();
  
  Op* pOp = m_pTask->m_pJob->m_pOp;
  
  Pipeline* pPipeline = pOp->pPipeline();
  
  pPipeline->vPulsed(m_pDataStack->iPopInt(__func__));
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
