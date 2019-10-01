/*============================================================================*/
/* Pipeline.cpp -- Operator Set Pipeline                                          */
/*============================================================================*/

#include "Operators.h"

extern OpCatalog gxOpCatalog;

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
/* process dynamic command                                                    */
/*----------------------------------------------------------------------------*/
void Pipeline::vPopulateOpVec(PipelineSpec* pPipelineSpec)
{
  
  StageSpecVec* pStageSpecVec = pPipelineSpec->pStageSpecVec();
  
  for (i8 i_StageSpec = 0; i_StageSpec < pStageSpecVec->nItems(); i_StageSpec++ ) {
    
    StageSpec* pStageSpec = pPipelineSpec->pStageSpec(i_StageSpec);
    m_oStageVec.vAdd(new Stage(pStageSpec));
    
    OpSpecVec* pOpSpecVec = pStageSpec->pOpSpecVec();
    
    Stage* pStage = m_oStageVec.pGet(i_StageSpec);
    
    for (i8 i_OpSpec = 0; i_OpSpec < pOpSpecVec->nItems(); i_OpSpec++ ) {
      
      OpSpec* pOpSpec = pOpSpecVec->pGet(i_OpSpec);
      OpDefn* aOpDefn = gxOpCatalog.aFindOp(pOpSpec->szName());
      
      // unknown
      if (! aOpDefn) {
        vTrace(" ? %s\n"_Stz,pOpSpec->szName());
        ASSERT(0);
        continue;
      }
      
      Op* pOp = (aOpDefn->m_fpNewOp)(pOpSpec,this);
      
      pStage->m_oOpVec.vAdd(pOp);
      
      // add the operator to the stages of the stagePtnVec as well
      for (i8 i_Ptn = 0; i_Ptn < pStage->m_oStagePtnVec.nItems(); i_Ptn++) {
        pStage->m_oStagePtnVec.pGet(i_Ptn)->m_oOpVec.vAdd(pOp);
      }
      vDebugPoint();
    }
    
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // link the operators
  
  // SINGLE STAGE
  Stage* pStage = m_oStageVec.pGet(0);
  
  // for each partition in the stage
  for (i8 i_Ptn = 0; i_Ptn < pStage->m_oStagePtnVec.nItems(); i_Ptn++) {
    
    StagePtn* pStagePtn = pStage->m_oStagePtnVec.pGet(0);
    
    // for each operator in the partition
    for (i8 i_Op = 0; i_Op < pStagePtn->m_oOpVec.nItems() - 1; i_Op++) {
    
      // get operator to be linked
      Op* pOp = pStagePtn->m_oOpVec.pGet(i_Op);
      
      // get the next operator
      Op* pOpNext = pStagePtn->m_oOpVec.pGet(i_Op+1);
      
      // link the two operators
      pOp->vNextLabel(pOpNext->szLabel());
      pOp->vOpOut(pOpNext);
      
      pOpNext->m_nLeftEndMarkers++;
    }
  }
  /*
  i8 i_Op = 0;
  i8 i_SlotsPerNode = AM::pGrid()->nSlotsPerNode();
  
  Stage* pStage = m_oStageVec.pGet(0);

  while (true) {
    
    Op* pOp;
    Op* pOpNext;
    i8 i_OpNext;
    
    // get operator to be linked
    pOp = pStage->m_oOpVec.pGet(i_Op);
    
    // get next operator
    if (pOp->m_aOpDefn->m_bIsPartitioned) {
      
      // figure out next operator
      i_OpNext = i_Op + i_SlotsPerNode;
      
      // get the next operator
      pOpNext = pStage->m_oOpVec.pGet(i_OpNext);
      
      // is the next operator an appendRight ?
      if (pOpNext->m_bAppendRight) {
        
        // skip this appendRight operator
        if (pOpNext->m_aOpDefn->m_bIsPartitioned) {
          i_OpNext = i_OpNext + i_SlotsPerNode ;
        }
        else {
          i_OpNext = i_OpNext + 1;
        }
      }
      
      // now link this operator with the next we figured out
      for (i8 i_Slot = 0; i_Slot < i_SlotsPerNode; i_Slot++) {
        
        // get the next operator
        pOpNext = pStage->m_oOpVec.pGet(i_OpNext);
        
        // link the two operators
        pOp->vNextLabel(pOpNext->szLabel());
        pOp->vOpOut(pOpNext);
        
        if (pOp->m_aOpDefn->m_bIsPartitioned && ! pOpNext->m_aOpDefn->m_bIsPartitioned)
          pOpNext->m_nLeftEndMarkers = i_SlotsPerNode;
        
        if (pOp->m_bAppendRight)
          pOpNext->m_nRightEndMarkers = pOpNext->m_nLeftEndMarkers;
        
        // move to next partition of this operator
        i_Op++;
        pOp = pStage->m_oOpVec.pGet(i_Op);
        
        // move to next partition of next operator
        if (pOpNext->m_aOpDefn->m_bIsPartitioned)
          i_OpNext++;
      }
    }
    else {
      
      // not partitioned, figure out next operator
      i_OpNext = i_Op + 1;
      
      // get next operator
      pOpNext = pStage->m_oOpVec.pGet(i_OpNext);
      
      // is the next operator an appendRight ?
      if (pOpNext->m_bAppendRight) {
        
        // skip this appendRight operator
        if (pOpNext->m_aOpDefn->m_bIsPartitioned) {
          
          i_OpNext = i_OpNext + i_SlotsPerNode - pOp->m_nSlot;
        }
        else
          i_OpNext = i_OpNext + 1;
      }
      
      // link the two operators
      pOp->vNextLabel(pOpNext->szLabel());
      pOp->vOpOut(pOpNext);
      if (pOp->m_aOpDefn->m_bIsPartitioned && ! pOpNext->m_aOpDefn->m_bIsPartitioned)
        pOpNext->m_nLeftEndMarkers = i_SlotsPerNode;
      
      if (pOp->m_bAppendRight)
        pOpNext->m_nRightEndMarkers = pOpNext->m_nLeftEndMarkers;
      
      //move to next operator to be linked
      i_Op++;
    }
    
    // are we done ?
    if (i_Op == pStage->m_oOpVec.nItems() - 1)
      break;
  }
*/
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // dump
  
  if (AM::bVerbose()) {
    vTrace("Dynamic Job:\n"_Stz);
    
    for (i8 i_Op = 0; i_Op < pStage->m_oOpVec.nItems(); i_Op++) {
      
      Op* pOp = pStage->m_oOpVec.pGet(i_Op);
      
      s32 sArgs;
      EnmSet* pEnmSet = pOp->m_pOpSpec->pEnmSet();
      for (i8 i_Arg = 0; i_Arg < pEnmSet->nEnm(); i_Arg++) {
        sArgs.vSprint("(%s)"_Stz,pEnmSet->szValue((i8)i_Arg));
      }
      
      pOp->m_pOpSpec->pEnmSet()->vPrint();
      
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
