/*============================================================================*/
/* Pipeline.cpp -- Pipeline                                                   */
/*============================================================================*/

#include "Pipelines.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(Pipelines,Pipeline,CN_PIPELINE)
__aNewFunction(Pipeline,CN_PIPELINE)
__vDeleteFunction(Pipeline,CN_PIPELINE)
__aFooterFunction(Pipeline)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Pipeline::Pipeline(Job* pPipelineJob, PipelineSpec* pPipelineSpec, i8 i_Origin)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINE,_FOOTER_);
  
  m_sPipelineNm   = pPipelineSpec->szPipelineNm();
  
  m_pPipelineJob  = pPipelineJob;
  m_pPipelineSpec = pPipelineSpec;
  m_i_Origin      = i_Origin;
  
  m_bErrored      = false;
  m_bPulsed       = false;
  m_bPulsedLimit  = false;
  m_pOpToPulse    = nullptr;
  
  m_bCancelled    = false;
    
  _checkInitialised_(CN_PIPELINE);
  
  vCreateStages();

  _postcondition_(_ADDRESS_,_MAGIC_,CN_PIPELINE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Pipeline::~Pipeline(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINE,_FOOTER_);
  
  delete m_pPipelineSpec;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* cancel the pipeline                                                        */
/*----------------------------------------------------------------------------*/
bool Pipeline::bCancel(Task* pTriggeringTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINE,_FOOTER_);
  
  // check if pipeline has already been pusled
  if (m_bPulsed)
    return false;
  
  // get the first stage of the pipeline
  Stage* pStage = pGet(0);
  
  // send cancelMarker to the start Op of the first stage of the pipeline
  pStage->vCancelPipeline(pTriggeringTask);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* pipeline has been  cancelled                                               */
/*----------------------------------------------------------------------------*/
void Pipeline::vCancelled(bool bCancelled)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINE,_FOOTER_);
  
  m_bCancelled = bCancelled;
}

/*----------------------------------------------------------------------------*/
/* is pipeline being cancelled                                                */
/*----------------------------------------------------------------------------*/
bool Pipeline::bCancelled(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINE,_FOOTER_);
  
  if (m_bCancelled)
    vDebugPoint();
  
  return m_bCancelled;
}

/*----------------------------------------------------------------------------*/
/* create the stages of this pipeline                                         */
/*----------------------------------------------------------------------------*/
void Pipeline::vCreateStages(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINE,_FOOTER_);

  i8 nStages = m_pPipelineSpec->nItems();
  for (i8 i_StageSpec = 0; i_StageSpec < nStages; i_StageSpec++ ) {
    
    // get the stageSpec
    StageSpec* pStageSpec = m_pPipelineSpec->pStageSpec(i_StageSpec);
    
    // add a stage from the stageSpec to the stageVec
    Stage* pStage = new Stage(pStageSpec, this);
    vAdd(pStage);
  }
}

/*----------------------------------------------------------------------------*/
/* dump the pipeline                                                          */
/*----------------------------------------------------------------------------*/
void Pipeline::vDump(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINE,_FOOTER_);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // dump all operators of each stage
    
  Grid* pGrid = AM::pGrid();
  
  vTrace("- - - - - - - - - - - - - - - - - -\n\n"_Stz);
    
  vTrace("Pipeline-Job %s Grid %s with %lld Nodes %lld tablePtns:\n"_Stz,
         m_sPipelineNm.szStz(),
         pGrid->szGridNm(),
         pGrid->nBootNodes(),
         pGrid->nTablePtns());
    
  //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // dump the stages with the operators
  for (i8 i_Stage = 0; i_Stage < nItems(); i_Stage++ ) {
    Stage* pStage = pGet(i_Stage);
    pStage->vDump(this);
  }
  
  vTrace("- - - - - - - - - - - - - - - - - -\n\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* set errored                                                                */
/*----------------------------------------------------------------------------*/
void Pipeline::vErrored(bool bErrored)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINE,_FOOTER_);
  
  m_bErrored = bErrored;
}

/*----------------------------------------------------------------------------*/
/* get errored                                                                 */
/*----------------------------------------------------------------------------*/
bool Pipeline::bErrored(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINE,_FOOTER_);
  
  return m_bErrored;
}

/*----------------------------------------------------------------------------*/
/* get the originating node                                                   */
/*----------------------------------------------------------------------------*/
i8 Pipeline::i_Origin(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINE,_FOOTER_);
  
  return m_i_Origin;
}

/*----------------------------------------------------------------------------*/
/* get the job                                                                */
/*----------------------------------------------------------------------------*/
Job* Pipeline::pPipelineJob(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINE,_FOOTER_);
  
  return m_pPipelineJob;
}

/*----------------------------------------------------------------------------*/
/* get number of ops to be excuted on given node                              */
/*----------------------------------------------------------------------------*/
i8 Pipeline::nOpsOnNode(i8 iNode)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINE,_FOOTER_);
  
  i8 nOps = 0;
  
  for (i8 i_Stage = 0; i_Stage < nItems(); i_Stage++) {
    Stage* pStage = pGet(i_Stage);
    nOps += pStage->nOpsOnNode(iNode);
  }
  
  return nOps;
}

/*----------------------------------------------------------------------------*/
/* get pOpToPulse                                                             */
/*----------------------------------------------------------------------------*/
Op* Pipeline::pOpToPulse(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINE,_FOOTER_);
  
  return m_pOpToPulse;
}

/*----------------------------------------------------------------------------*/
/* get pipeline name                                                          */
/*----------------------------------------------------------------------------*/
Stz* Pipeline::szPipelineNm(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINE,_FOOTER_);
  
  return m_sPipelineNm;
}

/*----------------------------------------------------------------------------*/
/* get pipeline spec                                                          */
/*----------------------------------------------------------------------------*/
PipelineSpec* Pipeline::pPipelineSpec(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINE,_FOOTER_);
  
  return m_pPipelineSpec;
}

/*----------------------------------------------------------------------------*/
/* get pulsed                                                                 */
/*----------------------------------------------------------------------------*/
bool Pipeline::bPulsedLimit(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINE,_FOOTER_);
  
  return m_bPulsed;
}

/*----------------------------------------------------------------------------*/
/* set pulsed                                                                 */
/*----------------------------------------------------------------------------*/
void Pipeline::vPulsed(bool bPulsed)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINE,_FOOTER_);
  
  m_bPulsed = bPulsed;
}

/*----------------------------------------------------------------------------*/
/* get pulsed                                                                 */
/*----------------------------------------------------------------------------*/
bool Pipeline::bPulsed(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINE,_FOOTER_);
  
  return m_bPulsed;
}

/*----------------------------------------------------------------------------*/
/* set up the pipeline job                                                    */
/*----------------------------------------------------------------------------*/
void Pipeline::vSetupJob(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINE,_FOOTER_);
  
  Stage* pStage = pGet(0);
  
  i8 nDivs = pStage->nItems();
  for (i8 i_Strand = 0; i_Strand < nDivs; i_Strand++) {
    Strand* pStrand = pStage->pGet(i_Strand);

    i8 nOps = pStrand->nItems();
    for (i8 i_Op = 0; i_Op < nOps; i_Op++) {
      Op* pOp = pStrand->pGet(i_Op);
      
      if (AM::szJobName(pOp->pOpJob())->bSame("limitOp"_Stz)) {
        m_bPulsedLimit = true;
        break;
      }
    }
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  for (i8 i_Strand = 0; i_Strand < nDivs; i_Strand++) {
    Strand* pStrand = pStage->pGet(i_Strand);

    i8 nOps = pStrand->nItems();
    for (i8 i_Op = 0; i_Op < nOps; i_Op++) {
      Op* pOp = pStrand->pGet(i_Op);

      if (AM::szJobName(pOp->pOpJob())->bSame("scan"_Stz)) {
        m_pOpToPulse = pOp;
        pOp->vPulsedLimit(true);
        break;
      }
    }
  }
  
}

/*----------------------------------------------------------------------------*/
/* spawn operaator jobs for the pipeline                                      */
/*----------------------------------------------------------------------------*/
void Pipeline::vSpawnOperatorJobs(i8 iSessionId, i8 iNode)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINE,_FOOTER_);
  
  for (i8 i_Stage = 0; i_Stage < nItems(); i_Stage++) {
    
    Stage* pStage = pGet(i_Stage);
    
    for (i8 i_Strand = 0; i_Strand < pStage->nItems(); i_Strand++) {
      Strand* pStrand = pStage->pGet(i_Strand);

      pStrand->vSpawnOperatorJobs(iSessionId, iNode);
    }
  }
  
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
