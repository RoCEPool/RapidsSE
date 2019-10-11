/*============================================================================*/
/* Pipeline.cpp -- Operator Set Pipeline                                          */
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
  
  // for each stage in the stageVec
  for (i8 i_Stage = 0; i_Stage < m_oStageVec.nItems(); i_Stage++) {
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // get the stage
    Stage* pStage = m_oStageVec.pGet(i_Stage);
    Op* pOpNext = nullptr;
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // link all operators of a stagePtn
  
    // for each stagePtn in the stagePtnVec of the stage
    for (i8 i_Ptn = 0; i_Ptn < pStage->m_oStagePtnVec.nItems(); i_Ptn++) {
      
      // get the stagePtn
      StagePtn* pStagePtn = pStage->m_oStagePtnVec.pGet(i_Ptn);
      
      // for each operator in the stagePtn
      for (i8 i_Op = 0; i_Op < pStagePtn->m_oOpVec.nItems() - 1; i_Op++) {
      
        // get operator
        Op* pOp = pStagePtn->m_oOpVec.pGet(i_Op);
        
        // get the next operator
        pOpNext = pStagePtn->m_oOpVec.pGet(i_Op+1);
        
        // link the two operators
        pOp->vNextLabel(pOpNext->szLabel());
        pOp->vOpOut(pOpNext);
      }
      
      // get the last operator of this stage
      Op* pOp = pStagePtn->m_oOpVec.pGet(pStagePtn->m_oOpVec.nItems()-1);
      
      // if last operator is xmit, link to the first operator of the next stage
      if (pOp->eClass() == CN_XMITOP) {
        
        // get the xmitOp's next stage
        RASSERT(pOp->m_iNextStage != C_UNKNOWN);
        Stage* pNextStage = m_oStageVec.pGet(pOp->m_iNextStage);
        RASSERT(pStage);
        
        // is it a stage run on node/nodes ?
        if (pNextStage->m_oStageNdeVec.nItems()) {
          RASSERT(pNextStage->m_oStagePtnVec.nItems() == 0);
          
          if (pNextStage->m_oStageNdeVec.nItems() == 1) {
            // going from all/one ptn to one node
            pOp->m_iNdeVec = 0;
          }
          else {
            // going from all/one ptn to all nodes
            RASSERT(0);
          }
          StageNde* pStageNde = pNextStage->m_oStageNdeVec.pGet(pOp->m_iNdeVec);
          AM::vXmitNode(pOp,pStageNde->m_i_Nde);
          pOpNext = pStageNde->m_oOpVec.pGet(0);
        }
        else {
          RASSERT(0);
        }
        pOp->vNextLabel(pOpNext->szLabel());
        pOp->vOpOut(pOpNext);
      }
    }
    
    // for each node in the stageNdeVec of the stage
    for (i8 i_Nde = 0; i_Nde < pStage->m_oStageNdeVec.nItems(); i_Nde++) {
      
      StageNde* pStageNde = pStage->m_oStageNdeVec.pGet(i_Nde);
      
      // for each operator in the partition
      for (i8 i_Op = 0; i_Op < pStageNde->m_oOpVec.nItems() - 1; i_Op++) {
        
        // get operator to be linked
        Op* pOp = pStageNde->m_oOpVec.pGet(i_Op);
        
        if (pOpNext)
          pOpNext->vNextLabel(pOp->szLabel());
        
        // get the next operator
        pOpNext = pStageNde->m_oOpVec.pGet(i_Op+1);
        
        // link the two operators
        pOp->vNextLabel(pOpNext->szLabel());
        pOp->vOpOut(pOpNext);
        
      }
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // dump
    
    if (AM::bVerbose()) {
      vTrace("Dynamic Job:\n"_Stz);
      
      for (i8 i_Op = 0; i_Op < pStage->m_oOpVec.nItems(); i_Op++) {
        
        Op* pOp = pStage->m_oOpVec.pGet(i_Op);
        
        s32 sArgs;
        EnameSet* pEnameSet = pOp->m_pOpSpec->pEnameSet();
        for (i8 i_Arg = 0; i_Arg < pEnameSet->nEnames(); i_Arg++) {
          sArgs.vSprint("(%s)"_Stz,pEnameSet->szValue((i8)i_Arg));
        }
        
        pOp->m_pOpSpec->pEnameSet()->vPrint();
        
        vTrace("  [%02lld] { %9s: %8s%-24s endMarkers(L%lld R%lld) :%-6s %s }\n"_Stz,
               i_Op,
               pOp->szLabel(),
               aCN[pOp->eClass()],
               sArgs.aStz(),
               pOp->m_nLeftEndMarkers,
               pOp->m_nRightEndMarkers,
               pOp->szNextLabel(),
               pOp->m_bAppendRight ? "appendRight"_Stz : "appendLeft "_Stz
               );
      }
    }

  }
  
  vDebugPoint();
}

/*----------------------------------------------------------------------------*/
/* set up the job                                                             */
/*----------------------------------------------------------------------------*/
void Pipeline::vSetupJob(void)
{
  vDebugPoint();
  
  Stage* pStage = m_oStageVec.pGet(0);
  i8 nOps = pStage->m_oOpVec.nItems();
  
  for (i8 i_Op = 0; i_Op < nOps; i_Op++) {
    Op* pOp = pStage->m_oOpVec.pGet(i_Op);
    
    if (CN_LIMITOP == pOp->eClass()) {
      m_bPulsed = true;
      break;
    }
  }
  
  if (! m_bPulsed)
    return;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  for (i8 i_Op = 0; i_Op < nOps; i_Op++) {
    Op* pOp = pStage->m_oOpVec.pGet(i_Op);
    
    if (CN_SCANOP == pOp->eClass()) {
      m_pOpToPulse = pOp;
      pOp->m_bPulsed = true;
      break;
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
