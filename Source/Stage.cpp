/*============================================================================*/
/* Stage.cpp -- Pipe Stage                                                    */
/*============================================================================*/

#include "Pipelines.h"

/*----------------------------------------------------------------------------*/
/*-- Lifecycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Stage* Pipelines::_pStage(void* aAddr)
{
  Stage* pStage = reinterpret_cast<Stage*>(aAddr);
  
  RASSERT_TYPE(pStage,CN_STAGE);
  
  return pStage;
}

/*----------------------------------------------------------------------------*/
/* new                                                                        */
/*----------------------------------------------------------------------------*/
void* Stage::operator new(size_t)
{
  void* pObject = HeapMem::pAllocate(CN_STAGE);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* delete                                                                     */
/*----------------------------------------------------------------------------*/
void Stage::operator delete(void* aAddr)
{
  Stage* pStage = reinterpret_cast<Stage*>(aAddr);
  
  HeapMem::vFree(pStage);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Stage::Stage(StageSpec* pStageSpec, Pipeline* pPipeline)
: Object(CN_STAGE)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STAGE,_FOOTER_);
  
  i8 i_Ptn;
  i8 i_Nde;
  
  m_sStageNm = pStageSpec->szStageNm();
  
  //m_oOpVec
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // create StageNde's and StagePtn's depending on the toplogy of the stage
  
  switch (pStageSpec->eTopology()) {
      
    case PO_ALL_NODES:
      for (i_Nde = 0; i_Nde < AM::pGrid()->nConnectedNodes(); i_Nde++) {
        m_oStageNdeVec.vAdd(new StageNde(pStageSpec, i_Nde));
      }
      break;
      
    case PO_ALL_PARTITIONS:
      for (i_Ptn = 0; i_Ptn < AM::pGrid()->nPartitions(); i_Ptn++) {
        m_oStagePtnVec.vAdd(new StagePtn(pStageSpec, i_Ptn));
      }
      break;
      
    case PO_ORIGIN:
      if (AM::bVerbose()) {
        i8 iThisNode = AM::pGrid()->iThisNode();
        if (iThisNode == 0) {
          vTrace("Pipeline Originating node : %lld\n"_Stz, pPipeline->iOriginatingNode());
        }
      }
      //for (i8 i_Thd = 0; i_Thd < pStageSpec->nThreads(); i_Thd++)
      //  m_oStageNdeVec.vAdd(new StageNde(pStageSpec, pPipeLine->iOriginatingNode(), i_Thd));
      m_oStageNdeVec.vAdd(new StageNde(pStageSpec, pPipeline->iOriginatingNode()));
      break;
      
    case PO_SPECIFIC_NODE:
      RASSERT(0);
      break;
      
    case PO_SPECIFIC_PARTITION:
      i_Ptn = pStageSpec->iPartition();
      m_oStagePtnVec.vAdd(new StagePtn(pStageSpec, i_Ptn));
      break;
      
    default:
      RASSERT(0);
  }
  
  // get the opSpecVec
  OpSpecVec* pOpSpecVec = pStageSpec->pOpSpecVec();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // populate the opVec and StageNde's and stagePtn's from the opSpec
  
  for (i8 i_OpSpec = 0; i_OpSpec < pOpSpecVec->nItems(); i_OpSpec++ ) {
    
    OpSpec* pOpSpec = pOpSpecVec->pGet(i_OpSpec);
    
    // add the operator to the stages of the stageNdeVec
    for (i8 i_Index = 0; i_Index < m_oStageNdeVec.nItems(); i_Index++) {
      StageNde* pStageNde = m_oStageNdeVec.pGet(i_Index);
      
      Op* pOp = new Op(pOpSpec,this,pPipeline);
      pOp->vNode(pStageNde->iNode());
      pStageNde->pOpVec()->vAdd(pOp);
      m_oOpVec.vAdd(pOp);
    }
    
    // add the operator to the stages of the stagePtnVec
    for (i8 i_Ptn = 0; i_Ptn < m_oStagePtnVec.nItems(); i_Ptn++) {
      Op* pOp = new Op(pOpSpec,this,pPipeline);
      StagePtn* pStagePtn = m_oStagePtnVec.pGet(i_Ptn);
      
      pOp->vPartition(pStagePtn->iPartition());
      pStagePtn->pOpVec()->vAdd(pOp);
      m_oOpVec.vAdd(pOp);
    }
  }
  
  _checkInitialised_(CN_STAGE);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_STAGE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Stage::~Stage(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STAGE,_FOOTER_);
  m_oOpVec.vClear();
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* send cancel marker to startops of the stage                                */
/*----------------------------------------------------------------------------*/
void Stage::vCancelOperators(i8 iThisNode)
{
  _precondition_(_MAGIC_,CN_STAGE,_FOOTER_);
  
  for (i8 i_Nde = 0; i_Nde < m_oStageNdeVec.nItems(); i_Nde++) {
    StageNde* pStageNde = m_oStageNdeVec.pGet(i_Nde);
    
    if (pStageNde->iNode() == iThisNode) {
      Op* pOp = pStageNde->pOpVec()->pGet(0);
      RASSERT(AM::szJobName(pOp->pOpJob())->bSame("startOp"_Stz));
      pOp->vAppendLeft(new Marker(OM_CANCELMARKER));
    }
  }
  
  for (i8 i_Ptn = 0; i_Ptn < m_oStagePtnVec.nItems(); i_Ptn++) {
    StagePtn* pStagePtn = m_oStagePtnVec.pGet(i_Ptn);
    
    i8 iPtnNode = TablePtn::iPartitionToNode(pStagePtn->iPartition());
    
    if (iPtnNode == iThisNode) {
      Op* pOp = pStagePtn->pOpVec()->pGet(0);
      RASSERT(AM::szJobName(pOp->pOpJob())->bSame("startOp"_Stz));
      pOp->vAppendLeft(new Marker(OM_CANCELMARKER));
    }
  }
  
}

/*----------------------------------------------------------------------------*/
/* get stage spec                                                             */
/*----------------------------------------------------------------------------*/
void Stage::vDump(StageVec* pStageVec)
{
  _precondition_(_MAGIC_,CN_STAGE,_FOOTER_);
  
  for (i8 i_Op = 0; i_Op < m_oOpVec.nItems(); i_Op++) {
    
    Op* pOp = m_oOpVec.pGet(i_Op);
    
    s256 sArgs;
    
    EnameSet* pEnameSet = pOp->pEnameSet();
    pEnameSet->vSprint(sArgs);
    
    // get the next stage
    i8 iNextStage = pEnameSet->iValue("<nextStage>"_Stz);
    Stage* pNextStage = pStageVec->pGet(iNextStage);
    
    // get the next op
    i8 iNextOp = pEnameSet->iValue("<nextOp>"_Stz);
    Op* pNextOp = pNextStage->pOpVec()->pGet(iNextOp);
    
    // get the cross topology
    ePX eTopology = (ePX)(pEnameSet->iValue("<topology>"_Stz));
    
    // get the left EOS markers for consume op
    //! ALKA: right EOS markers ???
    i8 iConsumeXmits =  pEnameSet->iValue("<consumeXmits>"_Stz);
    i8 iLeftEosMarkers = iConsumeXmits ? iConsumeXmits : pOp->nLeftEosMarkers();
    
    vTrace("  [%02lld] (N%lld) (P%02lld) {%12s%-20s EosMarkers(L%lld R%lld) :%-10s %s %s}\n"_Stz,
           i_Op,
           pOp->nNode(),
           pOp->nPartition(),
           pOp->szOpNm(),
           sArgs.szStz(),
           iLeftEosMarkers,
           pOp->nRightEosMarkers(),
           pNextOp->szOpNm(),
           pOp->bAppendRight() ? "appendRight"_Stz : "appendLeft "_Stz,
           aPX[eTopology]
          );
  }
}

/*----------------------------------------------------------------------------*/
/* get ops to be executed on given node                                       */
/*----------------------------------------------------------------------------*/
i8 Stage::nOpsOnNode(i8 iNode)
{
  _precondition_(_MAGIC_,CN_STAGE,_FOOTER_);
  
  i8 nOps = 0;
  
  for (i8 i_Ptn = 0; i_Ptn < m_oStagePtnVec.nItems(); i_Ptn++) {
    
    StagePtn* pStagePtn = m_oStagePtnVec.pGet(i_Ptn);
    
    // skip if partition is not for this node
    i8 iPartitionNode = TablePtn::iPartitionToNode(pStagePtn->iPartition());
    if (iPartitionNode == iNode) {
      nOps += pStagePtn->pOpVec()->nItems();
    }
  }
  
  for (i8 i_Nde = 0; i_Nde < m_oStageNdeVec.nItems(); i_Nde++) {
    
    StageNde* pStageNde = m_oStageNdeVec.pGet(i_Nde);
    
    // skip if Op not for this node
    if (pStageNde->iNode() == iNode) {
      nOps += pStageNde->pOpVec()->nItems();
    }
  }
  
  return nOps;
}

/*----------------------------------------------------------------------------*/
/* get opeVec                                                                 */
/*----------------------------------------------------------------------------*/
OpVec* Stage::pOpVec(void)
{
  _precondition_(_MAGIC_,CN_STAGE,_FOOTER_);
  
  return &m_oOpVec;
}

/*----------------------------------------------------------------------------*/
/* get pipeOut operator from the stage depending on cross topology            */
/*----------------------------------------------------------------------------*/
Op* Stage::pPipeOutOp(ePX eTopologyX, i8 iNde, i8 iPtn, i8 iOp)
{
  Op* pOp = nullptr;
  StageNde* pStageNde = nullptr;
  StagePtn* pStagePtn = nullptr;
  
  switch (eTopologyX) {
      
    case PX_ALL_NODES_TO_ORIGIN:
    case PX_ALL_PARTITIONS_TO_ORIGIN:
    case PX_ORIGIN_TO_ORIGIN:
    case PX_SPECIFIC_PARTITION_TO_ORIGIN:
      pStageNde = this->pStageNde(iNde);
      pOp = pStageNde->pOpVec()->pGet(iOp);
      break;
      
    case PX_ORIGIN_TO_ALL_PARTITIONS:
    case PX_ORIGIN_TO_SPECIFIC_PARTITION:
        pStagePtn = m_oStagePtnVec.pGet(iPtn);
        pOp = pStagePtn->pOpVec()->pGet(iOp);
      break;
      
    case PX_ALL_PARTITIONS_TO_ALL_NODES:
      pStageNde = this->pStageNde(iNde);
      pOp = pStageNde->pOpVec()->pGet(iOp);
      break;
      
    case PX_ALL_PARTITIONS_TO_ALL_PARTITIONS:
      pStagePtn = m_oStagePtnVec.pGet(iPtn);
      pOp = pStagePtn->pOpVec()->pGet(iOp);
      break;
      
    case PX_SPECIFIC_PARTITION_TO_SPECIFIC_PARTITION:
      pStagePtn = m_oStagePtnVec.pGet(0);
      ASSERT(pStagePtn->iPartition() == iPtn);
      pOp = pStagePtn->pOpVec()->pGet(iOp);
      break;
      
    default:
      RASSERT(0);
  }
  
  return pOp;  
}

/*----------------------------------------------------------------------------*/
/* get stage node for given node                                              */
/*----------------------------------------------------------------------------*/
StageNde* Stage::pStageNde(i8 i_Nde)
{
  _precondition_(_MAGIC_,CN_STAGE,_FOOTER_);
  
  StageNde* pStageNde = nullptr;
  
  for (i8 iNdeVec = 0; iNdeVec < m_oStageNdeVec.nItems(); iNdeVec++) {
    pStageNde = m_oStageNdeVec.pGet(iNdeVec);
    if (pStageNde->iNode() == i_Nde)
      return pStageNde;
  }
  
  return pStageNde;
}

/*----------------------------------------------------------------------------*/
/* get stage spec                                                             */
/*----------------------------------------------------------------------------*/
StageNdeVec* Stage::pStageNdeVec(void)
{
  _precondition_(_MAGIC_,CN_STAGE,_FOOTER_);
  
  return &m_oStageNdeVec;
}

/*----------------------------------------------------------------------------*/
/* get name                                                                   */
/*----------------------------------------------------------------------------*/
Stz* Stage::szStageNm(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STAGE,_FOOTER_);
  
  return m_sStageNm;
}

/*----------------------------------------------------------------------------*/
/* get stage spec                                                             */
/*----------------------------------------------------------------------------*/
StagePtnVec* Stage::pStagePtnVec(void)
{
  _precondition_(_MAGIC_,CN_STAGE,_FOOTER_);
  
  return &m_oStagePtnVec;
}

/*----------------------------------------------------------------------------*/
/* get stage ptn for given ptn                                                */
/*----------------------------------------------------------------------------*/
StagePtn* Stage::pStagePtn(i8 i_Ptn)
{
  _precondition_(_MAGIC_,CN_STAGE,_FOOTER_);
  
  StagePtn* pStagePtn = nullptr;
  
  for (i8 iPtnVec = 0; iPtnVec < m_oStagePtnVec.nItems(); iPtnVec++) {
    pStagePtn = m_oStagePtnVec.pGet(iPtnVec);
    if (pStagePtn->iPartition() == i_Ptn)
      return pStagePtn;
  }
  
  return pStagePtn;
}

/*----------------------------------------------------------------------------*/
/* send startMarker to startop of the stage                                   */
/*----------------------------------------------------------------------------*/
void Stage::vStartPipeline(void)
{
  _precondition_(_MAGIC_,CN_STAGE,_FOOTER_);
  
  // get first stage from stageNdeVec
  StageNde* pStageNde = m_oStageNdeVec.pGet(0);

  if (pStageNde->iNode() == AM::pGrid()->iThisNode()) {
    
    // get first operator of this first stage
    Op* pOp = pStageNde->pOpVec()->pGet(0);
    RASSERT(AM::szJobName(pOp->pOpJob())->bSame("startOp"_Stz));
    
    // append a STARTMARKER
    pOp->vAppendLeft(new Marker(OM_STARTMARKER));
  }
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(Stage)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
