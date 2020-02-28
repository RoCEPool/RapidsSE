/*============================================================================*/
/* Pipeline.cpp -- Operator Set Pipeline                                      */
/*============================================================================*/

#include "Pipelines.h"

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Pipeline* Pipelines::_pPipeline(void* aAddr)
{
  Pipeline* pPipeline = reinterpret_cast<Pipeline*>(aAddr);
  
  RASSERT_TYPE(pPipeline,CN_PIPELINE);
  
  return pPipeline;
}

/*----------------------------------------------------------------------------*/
/* new                                                                        */
/*----------------------------------------------------------------------------*/
void* Pipeline::operator new(size_t)
{
  void* pObject = HeapMem::pAllocate(CN_PIPELINE);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* delete                                                                     */
/*----------------------------------------------------------------------------*/
void Pipeline::operator delete(void* aAddr)
{
  Pipeline* pPipeline = reinterpret_cast<Pipeline*>(aAddr);
  
  HeapMem::vFree(pPipeline);
}

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
  
  //m_oStageVec
  
  _checkInitialised_(CN_PIPELINE);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_PIPELINE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Pipeline::~Pipeline(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

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
    
  vTrace("- - - - - - - - - - - - - - - - - -\n\n"_Stz);
    
  vTrace("Pipeline Job:\n\n"_Stz);
    
  //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // dump the stages with the operators
  for (i8 i_Stage = 0; i_Stage < nItems(); i_Stage++ ) {
    Stage* pStage = pGet(i_Stage);
    vTrace("\n[%lld] Stage %s : \n"_Stz, i_Stage, pStage->szStageNm());
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
/* Populate the OpVec from the PipeLineSpec                                   */
/*----------------------------------------------------------------------------*/
void Pipeline::vPopulateOpVec(PipelineSpec* pPipelineSpec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINE,_FOOTER_);
  
  //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get the stageSpecVec
  StageSpecVec* pStageSpecVec = pPipelineSpec->pStageSpecVec();
  
  //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Buld the stageVec from the stageSpecVec
  for (i8 i_StageSpec = 0; i_StageSpec < pStageSpecVec->nItems(); i_StageSpec++ ) {
    
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
/* set up the job                                                             */
/*----------------------------------------------------------------------------*/
void Pipeline::vSetupJob(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINE,_FOOTER_);
  
  Stage* pStage = pGet(0);
  i8 nOps = pStage->pOpVec()->nItems();
  
  for (i8 i_Op = 0; i_Op < nOps; i_Op++) {
    Op* pOp = pStage->pOpVec()->pGet(i_Op);
    
    if (AM::szJobName(pOp->pOpJob())->bSame("limitOp"_Stz)) {
      m_bPulsed = true;
      break;
    }
  }
  
  if (! m_bPulsed)
    return;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  for (i8 i_Op = 0; i_Op < nOps; i_Op++) {
    Op* pOp = pStage->pOpVec()->pGet(i_Op);
    
    if (AM::szJobName(pOp->pOpJob())->bSame("scanOp"_Stz)) {
      m_pOpToPulse = pOp;
      pOp->vPulsed(true);
      break;
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
    
    for (i8 i_Nde = 0; i_Nde < pStage->pStageNdeVec()->nItems(); i_Nde++) {
      Division* pDivision = pStage->pStageNdeVec()->pGet(i_Nde);
      
      pDivision->vSpawnOperators(iSessionId, i_Nde);
    }
    
    for (i8 i_Ptn = 0; i_Ptn < pStage->pStagePtnVec()->nItems(); i_Ptn++) {
      Division* pDivision = pStage->pStagePtnVec()->pGet(i_Ptn);
      
      pDivision->vSpawnOperators(iSessionId, iNode);
    }
  }
  
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(Pipeline);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
