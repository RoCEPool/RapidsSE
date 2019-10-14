/*============================================================================*/
/* Pipeline.cpp -- Operator Set Pipeline                                      */
/*============================================================================*/

#include "Operators.h"

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Pipeline* Operators::_pPipeline(void* aAddr)
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
Pipeline::Pipeline(Job* pJob)
: Object(CN_PIPELINE)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PIPELINE,_FOOTER_);
  
  m_pSession   = nullptr;
  m_pJob       = pJob;
  m_bPulsed    = false;
  m_pOpToPulse = nullptr;
  
  m_bCancelling = false;
  
  m_pPipelineSpec = nullptr;
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
  m_bCancelling = bCancelling;
}

/*----------------------------------------------------------------------------*/
/* get cancelling                                                             */
/*----------------------------------------------------------------------------*/
bool Pipeline::bCancelling(void)
{
  return m_bCancelling;
}

/*----------------------------------------------------------------------------*/
/* get the job                                                                */
/*----------------------------------------------------------------------------*/
Job* Pipeline::pJob(void)
{
  return m_pJob;
}

/*----------------------------------------------------------------------------*/
/* get number of ops to be excuted on given node                              */
/*----------------------------------------------------------------------------*/
i8 Pipeline::nOpsOnNode(i8 iNode)
{
  i8 nOps = 0;
  
  for (i8 i_Stage = 0; i_Stage < m_oStageVec.nItems(); i_Stage++) {
    
    Stage* pStage = m_oStageVec.pGet(i_Stage);
    
    for (i8 i_Ptn = 0; i_Ptn < pStage->pStagePtnVec()->nItems(); i_Ptn++) {
      
      StagePtn* pStagePtn = pStage->pStagePtnVec()->pGet(i_Ptn);
      
      // skip if partition is not for this node
      i8 iPartitionNode = TablePtn::iPartitionToNode(pStagePtn->m_i_Ptn);
      if (iPartitionNode == iNode) {
        nOps += pStagePtn->m_oOpVec.nItems();
      }
    }
    
    for (i8 i_Nde = 0; i_Nde < pStage->pStageNdeVec()->nItems(); i_Nde++) {
      
      StageNde* pStageNde = pStage->pStageNdeVec()->pGet(i_Nde);
      
      // skip if Op not for this node
      if (pStageNde->iNode() == iNode) {
        nOps += pStageNde->pOpVec()->nItems();
      }
    }
  }
  
  return nOps;
}

/*----------------------------------------------------------------------------*/
/* get pOpToPulse                                                             */
/*----------------------------------------------------------------------------*/
Op* Pipeline::pOpToPulse(void)
{
  return m_pOpToPulse;
}

/*----------------------------------------------------------------------------*/
/* Populate the OpVec from the PipeLineSpec                                   */
/*----------------------------------------------------------------------------*/
void Pipeline::vPopulateOpVec(PipelineSpec* pPipelineSpec)
{
  
  //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get the stageSpecVec
  StageSpecVec* pStageSpecVec = pPipelineSpec->pStageSpecVec();
  
  //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Buld the stageVec from the stageSpecVec
  for (i8 i_StageSpec = 0; i_StageSpec < pStageSpecVec->nItems(); i_StageSpec++ ) {
    
    // get the stageSpec
    StageSpec* pStageSpec = pPipelineSpec->pStageSpec(i_StageSpec);
    
    // add a stage from the stageSpec to the stageVec
    m_oStageVec.vAdd(new Stage(pStageSpec, this));
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // link the operators in a stage and across stages
  
  if (AM::bVerbose()) {
    vTrace("- - - - - - - - - - - - - - - - - -\n\n"_Stz);
    vTrace("Dynamic Job:\n\n"_Stz);
  }
    
  // for each stage in the stageVec
  for (i8 i_Stage = 0; i_Stage < m_oStageVec.nItems(); i_Stage++) {
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // get the stage
    Stage* pStage = m_oStageVec.pGet(i_Stage);
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // link all operators of a stagePtn

    for (i8 i_Ptn = 0; i_Ptn < pStage->pStagePtnVec()->nItems(); i_Ptn++) {
      
      // get the stagePtn
      StagePtn* pStagePtn = pStage->pStagePtnVec()->pGet(i_Ptn);
      
      // link all operators of the stage partition
      pStagePtn->vLinkOperators(&m_oStageVec);
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // link all operators of a stageNde
    
    for (i8 i_Nde = 0; i_Nde < pStage->pStageNdeVec()->nItems(); i_Nde++) {
      
      StageNde* pStageNde = pStage->pStageNdeVec()->pGet(i_Nde);
      
      // link all operators of the stage partition
      pStageNde->vLinkOperators();
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // dump all operators of a stage
    
    if (AM::bVerbose()) {
      vTrace("Stage %lld: \n"_Stz, i_Stage);
      pStage->vDump();
    }
  }
  
  if (AM::bVerbose())
    vTrace("- - - - - - - - - - - - - - - - - -\n\n"_Stz);
  
  vDebugPoint();
}

/*----------------------------------------------------------------------------*/
/* get pulsed                                                                 */
/*----------------------------------------------------------------------------*/
bool Pipeline::bPulsed(void)
{
  return m_bPulsed;
}

/*----------------------------------------------------------------------------*/
/* get the session                                                            */
/*----------------------------------------------------------------------------*/
Session* Pipeline::pSession(void)
{
  return m_pSession;
}

/*----------------------------------------------------------------------------*/
/* set the session                                                            */
/*----------------------------------------------------------------------------*/
void Pipeline::vSession(Session* pSession)
{
  m_pSession = pSession;
}

/*----------------------------------------------------------------------------*/
/* set up the job                                                             */
/*----------------------------------------------------------------------------*/
void Pipeline::vSetupJob(void)
{
  vDebugPoint();
  
  Stage* pStage = m_oStageVec.pGet(0);
  i8 nOps = pStage->pOpVec()->nItems();
  
  for (i8 i_Op = 0; i_Op < nOps; i_Op++) {
    Op* pOp = pStage->pOpVec()->pGet(i_Op);
    
    if (CN_LIMITOP == pOp->eClass()) {
      m_bPulsed = true;
      break;
    }
  }
  
  if (! m_bPulsed)
    return;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  for (i8 i_Op = 0; i_Op < nOps; i_Op++) {
    Op* pOp = pStage->pOpVec()->pGet(i_Op);
    
    if (CN_SCANOP == pOp->eClass()) {
      m_pOpToPulse = pOp;
      pOp->m_bPulsed = true;
      break;
    }
  }
  
}

/*----------------------------------------------------------------------------*/
/* get the stage vector                                                       */
/*----------------------------------------------------------------------------*/
StageVec* Pipeline::pStageVec(void)
{
  return &m_oStageVec;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(Pipeline);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
