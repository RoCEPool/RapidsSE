/*============================================================================*/
/* Stage.cpp -- Pipe Stage                                                    */
/*============================================================================*/

#include "Operators.h"

/*----------------------------------------------------------------------------*/
/*-- Lifecycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Stage* Operators::_pStage(void* aAddr)
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
Stage::Stage(StageSpec* pStageSpec, Pipeline* pPipeLine)
: Object(CN_STAGE)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STAGE,_FOOTER_);
  
  m_pStageSpec = pStageSpec;
  //m_oOpVec
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // create StageNde's and StagePtn's depending on the toplogy of the stage
  
  switch (m_pStageSpec->eTopology()) {
      
    case PO_ALL_NODES:
      for (i8 i_Nde = 0; i_Nde < AM::pGrid()->nConnectedNodes(); i_Nde++) {
        m_oStageNdeVec.vAdd(new StageNde(pStageSpec, i_Nde));
      }
      break;
      
    case PO_ALL_PARTITIONS:
      for (i8 i_Ptn = 0; i_Ptn < AM::pGrid()->nPartitions(); i_Ptn++) {
        m_oStagePtnVec.vAdd(new StagePtn(pStageSpec, i_Ptn));
      }
      break;
      
    case PO_ORIGIN:
      vTrace("Pipeline Originating node : %lld\n"_Stz, pPipeLine->iOriginatingNode());
      m_oStageNdeVec.vAdd(new StageNde(pStageSpec, pPipeLine->iOriginatingNode()));
      break;
      
    case PO_SPECIFIC_NODE:
      RASSERT(0);
      break;
      
    case PO_SPECIFIC_PARTITION:
      RASSERT(0);
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
      Op* pOp = new Op(CN_OP,pOpSpec,pPipeLine);
      StageNde* pStageNde = m_oStageNdeVec.pGet(i_Index);
      
      pOp->vNode(pStageNde->iNode());
      pStageNde->pOpVec()->vAdd(pOp);
      m_oOpVec.vAdd(pOp);
    }
    
    // add the operator to the stages of the stagePtnVec
    for (i8 i_Ptn = 0; i_Ptn < m_oStagePtnVec.nItems(); i_Ptn++) {
      Op* pOp = new Op(CN_OP,pOpSpec,pPipeLine);
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
      RASSERT(AM::eJobType(pOp->pOpJob()) == JT_STARTOP);
      pOp->vAppendLeft(new Marker(OM_CANCELMARKER));
    }
  }
  
  for (i8 i_Ptn = 0; i_Ptn < m_oStagePtnVec.nItems(); i_Ptn++) {
    StagePtn* pStagePtn = m_oStagePtnVec.pGet(i_Ptn);
    
    i8 iPtnNode = TablePtn::iPartitionToNode(pStagePtn->iPartition());
    
    if (iPtnNode == iThisNode) {
      Op* pOp = pStagePtn->pOpVec()->pGet(0);
      RASSERT(AM::eJobType(pOp->pOpJob()) == JT_STARTOP);
      pOp->vAppendLeft(new Marker(OM_CANCELMARKER));
    }
  }
  
}

/*----------------------------------------------------------------------------*/
/* get stage spec                                                             */
/*----------------------------------------------------------------------------*/
void Stage::vDump(void)
{
  _precondition_(_MAGIC_,CN_STAGE,_FOOTER_);
  
  for (i8 i_Op = 0; i_Op < m_oOpVec.nItems(); i_Op++) {
    
    Op* pOp = m_oOpVec.pGet(i_Op);
    
    s32 sArgs;
    
    EnameSet* pEnameSet = pOp->pOpSpec()->pEnameSet();
    pEnameSet->vSprint(sArgs);
    
    vTrace("  [%02lld] (N%lld) (P%02lld) {%12s%-24s EosMarkers(L%lld R%lld) :%-10s %s }\n"_Stz,
           i_Op,
           pOp->nNode(),
           pOp->nPartition(),
           pOp->szName(),
           sArgs.szStz(),
           pOp->nLeftEosMarkers(),
           pOp->nRightEosMarkers(),
           ""_Stz,
           pOp->bAppendRight() ? "appendRight"_Stz : "appendLeft "_Stz
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
      pStageNde = this->pStageNde(iNde);
      pOp = pStageNde->pOpVec()->pGet(iOp);
      break;
      
    case PX_ORIGIN_TO_ALL_PARTITIONS:
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
      
    default:
      RASSERT(0);
  }
  
  return pOp;  
}

/*----------------------------------------------------------------------------*/
/* get stage spec                                                             */
/*----------------------------------------------------------------------------*/
StageSpec* Stage::pStageSpec(void)
{
  _precondition_(_MAGIC_,CN_STAGE,_FOOTER_);
  
  return m_pStageSpec;
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
    RASSERT(AM::eJobType(pOp->pOpJob()) == JT_STARTOP);
    
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
