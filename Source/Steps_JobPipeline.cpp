/*============================================================================*/
/* Steps_JobPipeline.cpp - JobPipeline step group                             */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_JobPipeline(void)
{
  vAddStep("Job_Pipeline.begin"_Stz,    &Step::vJob_PipelineBegin);
  vAddStep("Job_Pipeline.cancel"_Stz,   &Step::vJob_PipelineCancel);
  vAddStep("Job_Pipeline.execute"_Stz,  &Step::vJob_PipelineExecute);
  vAddStep("Job_Pipeline::compile"_Stz, &Step::vJob_PipelineCompile);
}

/*----------------------------------------------------------------------------*/
/* Job_Pipeline.begin -- begin the Pipeline job                               */
/*----------------------------------------------------------------------------*/
void Step::vJob_PipelineBegin(void)
{
  ENTRY(); // ENTRY("vJob_PipelineBegin"_Stz);
  
  Job* pJob_Pipeline = _pJob(m_pDataStack->pPopPtr(__func__));
  i8 iThisNode = AM::pGrid()->iThisNode();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // create a pipeline
  pJob_Pipeline->m_pPipeline = new Pipeline(pJob_Pipeline, pJob_Pipeline->iOriginatorNode());
  Pipeline* pPipeline = pJob_Pipeline->m_pPipeline;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  RASSERT( ! pJob_Pipeline->m_pBufferQueue );
  
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
/* Job_Pipeline.cancel -- cancel the Pipeline job                             */
/*----------------------------------------------------------------------------*/
void Step::vJob_PipelineCancel(void)
{
  ENTRY(); // ENTRY("vJob_PipelineCancel"_Stz);
  
  Job* pJob_Pipeline = _pJob(m_pDataStack->pPopPtr(__func__));
  
  // get the first stage of the pipeline
  Stage* pStage = pJob_Pipeline->m_pPipeline->pStageVec()->pGet(0);
  
  // send cancelMarker to the startOps of the first stage of the pipeline
  i8 iThisNode = AM::pGrid()->iThisNode();
  pStage->vCancelOperators(iThisNode);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Job_Pipeline.execute -- execute                                            */
/*----------------------------------------------------------------------------*/
void Step::vJob_PipelineExecute(void)
{
  ENTRY(); // ENTRY("vJob_PipelineExecute"_Stz);
  
  Job* pJob_Pipeline = _pJob(m_pDataStack->pPopPtr(__func__));
  
  Pipeline* pPipeline = pJob_Pipeline->m_pPipeline;
  i8 iThisNode = AM::pGrid()->iThisNode();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get no of operators to be executed on this node
  i8 nOps = pPipeline->nOpsOnNode(iThisNode);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // set acks remaining equal to no of operators to be executed on this node
  AM::vSetAcksRmg(pJob_Pipeline->pTask(),nOps);
  
  // get the first stage of the pipeline
  Stage* pStage = pPipeline->pStageVec()->pGet(0);
  
  // send startMarker to the startOp of the first stage of the pipeline
  pStage->vStartPipeline();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Job_Pipeline::compile -- compile the command                               */
/*----------------------------------------------------------------------------*/
void Step::vJob_PipelineCompile(void)
{
  ENTRY(); // ENTRY("vJob_PipelineCompile"_Stz);
  
  s4K sCmd;
  m_pDataStack->vPopStr(sCmd,__func__);

  m_pRegSet->m_xpDynamo = new Dynamo(sCmd);
  
  bool bResult = m_pRegSet->m_xpDynamo->bCompile(m_pTask->m_pJob);
  
  if ( ! bResult) {
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
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
