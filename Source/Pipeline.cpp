/*============================================================================*/
/* Pipeline.cpp -- Operator Set Pipeline                                      */
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
Pipeline::Pipeline(Job* pJob, PipelineSpec* pPipelineSpec, i8 iOriginatingNode)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINE,_FOOTER_);
  
  m_sPipelineNm = pPipelineSpec->szPipelineNm();
  
  m_pJob = pJob;
  m_pPipelineSpec = pPipelineSpec;
  m_iOriginatingNode = iOriginatingNode;
  
  m_bPulsed    = false;
  m_pOpToPulse = nullptr;
  
  m_bCancelling = false;
    
  _checkInitialised_(CN_PIPELINE);

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
void Pipeline::vCancel(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINE,_FOOTER_);
  
  // get the first stage of the pipeline
  Stage* pStage = pGet(0);
  
  // send cancelMarker to the startOp of the first stage of the pipeline
  pStage->vCancelPipeline();
}

/*----------------------------------------------------------------------------*/
/* set cancelling                                                             */
/*----------------------------------------------------------------------------*/
void Pipeline::vCancelling(bool bCancelling)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINE,_FOOTER_);
  
  m_bCancelling = bCancelling;
}

/*----------------------------------------------------------------------------*/
/* get cancelling                                                             */
/*----------------------------------------------------------------------------*/
bool Pipeline::bCancelling(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINE,_FOOTER_);
  
  if (m_bCancelling)
    vDebugPoint();
  
  return m_bCancelling;
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
    
  vTrace("Pipeline-Job %s Grid %s with %lld Nodes %lld Slots/Node:\n"_Stz,
         m_sPipelineNm.szStz(),
         pGrid->szGridNm(),
         pGrid->nDefinedNodes(),
         pGrid->nSlotsPerNode());
    
  //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // dump the stages with the operators
  for (i8 i_Stage = 0; i_Stage < nItems(); i_Stage++ ) {
    Stage* pStage = pGet(i_Stage);
    pStage->vDump(this);
  }
  
  vTrace("- - - - - - - - - - - - - - - - - -\n\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* get the originating node                                                   */
/*----------------------------------------------------------------------------*/
i8 Pipeline::iOriginatingNode(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINE,_FOOTER_);
  
  return m_iOriginatingNode;
}

/*----------------------------------------------------------------------------*/
/* get the job                                                                */
/*----------------------------------------------------------------------------*/
Job* Pipeline::pJob(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINE,_FOOTER_);
  
  return m_pJob;
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
/* Populate the OpVec from the PipeLineSpec                                   */
/*----------------------------------------------------------------------------*/
void Pipeline::vPopulateOpVec(PipelineSpec* pPipelineSpec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINE,_FOOTER_);
  
  // Buld the stageVec from the stageSpecVec
  for (i8 i_StageSpec = 0; i_StageSpec < pPipelineSpec->nItems(); i_StageSpec++ ) {
    
    // get the stageSpec
    StageSpec* pStageSpec = pPipelineSpec->pStageSpec(i_StageSpec);
    
    // add a stage from the stageSpec to the stageVec
    Stage* pStage = new Stage(pStageSpec, this);
    vAdd(pStage);
  }
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
        m_bPulsed = true;
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

      if (AM::szJobName(pOp->pOpJob())->bSame("scanOp"_Stz)) {
        m_pOpToPulse = pOp;
        pOp->vPulsed(true);
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

      pStrand->vSpawnOperators(iSessionId, iNode);
    }
  }
  
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
