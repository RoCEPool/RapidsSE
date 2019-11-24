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
  vAddStep("Job_Pipeline.end"_Stz,      &Step::vJob_PipelineEnd);
  vAddStep("Job_Pipeline.execute"_Stz,  &Step::vJob_PipelineExecute);
  vAddStep("Job_Pipeline::compile"_Stz, &Step::vJob_PipelineCompile);
}

/*----------------------------------------------------------------------------*/
/* Job_Pipeline.begin -- begin the Pipeline job                               */
/*----------------------------------------------------------------------------*/
void Step::vJob_PipelineBegin(void)
{
  ENTRY(); // ENTRY("vJob_PipelineBegin"_Stz);
  
  Object* pJobPtr;
  if ( ! m_pDataStack->bPopObject(pJobPtr) )
    THROW_DSTACK();
  
  Job* pJob_Pipeline = _pJob(pJobPtr);
  
  //pJob_Pipeline->vBegin();
  
  pJob_Pipeline->m_pPipeline = new Pipeline(pJob_Pipeline);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  RASSERT( ! pJob_Pipeline->m_pBufferQueue );
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // set the pipeline
  
  pJob_Pipeline->m_pPipeline->vPopulateOpVec(m_pRegSet->m_pDynamo->pPipelineSpec());
  pJob_Pipeline->m_pPipeline->vSetupJob();
  
  delete m_pRegSet->m_pDynamo;
  m_pRegSet->m_pDynamo = nullptr;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Job_Pipeline.cancel -- cancel the Pipeline job                             */
/*----------------------------------------------------------------------------*/
void Step::vJob_PipelineCancel(void)
{
  ENTRY(); // ENTRY("vJob_PipelineCancel"_Stz);
  
  Object* pJobPtr;
  if ( ! m_pDataStack->bPopObject(pJobPtr) )
    THROW_DSTACK();
  
  Job* pJob_Pipeline = _pJob(pJobPtr);
  
  //pJob_Pipeline->vCancel();
  
  // get the first stage of the pipeline
  Stage* pStage = pJob_Pipeline->m_pPipeline->pStageVec()->pGet(0);
  
  // send cancelMarker to the startOps of the first stage of the pipeline
  i8 iThisNode = AM::pGrid()->iThisNode();
  pStage->vCancelOperators(iThisNode);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Job_Pipeline.end -- end the Pipeline job                                   */
/*----------------------------------------------------------------------------*/
void Step::vJob_PipelineEnd(void)
{
  ENTRY(); // ENTRY("vJob_PipelineEnd"_Stz);
  
  Object* pJobPtr;
  if ( ! m_pDataStack->bPopObject(pJobPtr) )
    THROW_DSTACK();
  
  Job* pJob_Pipeline = _pJob(pJobPtr);
  
  //pJob_Pipeline->vEnd();
  
  // get the first stage of the pipeline
  Stage* pStage = pJob_Pipeline->m_pPipeline->pStageVec()->pGet(0);
  
  // send endMarker to the startOps of the first stage of the pipeline
  i8 iThisNode = AM::pGrid()->iThisNode();
  pStage->vEndOperators(iThisNode);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Job_Pipeline.execute -- execute                                            */
/*----------------------------------------------------------------------------*/
void Step::vJob_PipelineExecute(void)
{
  ENTRY(); // ENTRY("vJob_PipelineExecute"_Stz);
  
  Object* pJobPtr;
  if ( ! m_pDataStack->bPopObject(pJobPtr) )
    THROW_DSTACK();
  
  Job* pJob_Pipeline = _pJob(pJobPtr);
  
  //pJob_Pipeline->vExecute();
  
  Pipeline* pPipeline = pJob_Pipeline->m_pPipeline;
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get no of operators to be executed on this node
  
  i8 iThisNode = AM::pGrid()->iThisNode();
  i8 nOps = pPipeline->nOpsOnNode(iThisNode);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // set acks remaining equal to no of operators to be executed on this node
  
  AM::vSetAcksRmg(pJob_Pipeline->pTask(),nOps);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // spawn operaotors for this node
  vTrace("Spawning operator jobs for node %lld\n"_Stz, AM::pGrid()->iThisNode());
  
  pPipeline->vSpawnOperatorJobs(pJob_Pipeline->m_xReqTuple.m_i_Session, iThisNode);
  
  // get the first stage of the pipeline
  Stage* pStage = pPipeline->pStageVec()->pGet(0);
  
  // send startMarker to the startOps of the first stage of the pipeline
  pStage->vStartOperators(iThisNode);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Job_Pipeline::compile -- compile the command                               */
/*----------------------------------------------------------------------------*/
void Step::vJob_PipelineCompile(void)
{
  ENTRY(); // ENTRY("vJob_PipelineCompile"_Stz);
  
  s4K sCmd;
  if (! m_pDataStack->bPopS4K(sCmd))
    THROW_DSTACK();

  m_pRegSet->m_pDynamo = new Dynamo(sCmd);
  
  bool bResult = m_pRegSet->m_pDynamo->bCompile();
  RASSERT( bResult );
  
//  if ( ! bResult) {
//    vAddError(EC_INVALID_COMMAND);
//  }
  
  if (AM::bVerbose()) {
    m_pRegSet->m_pDynamo->vShow();
  }
  
  EXIT();
}
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
