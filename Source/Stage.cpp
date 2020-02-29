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
  
  m_sStageNm   = pStageSpec->szStageNm();
  m_pStageSpec = pStageSpec;
  m_pPipeline  = pPipeline;

  _checkInitialised_(CN_STAGE);

  vCreateDivisions();
  
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
    Division* pDivision = m_oStageNdeVec.pGet(i_Nde);
    
    if (pDivision->i_Nde() == iThisNode) {
      Op* pOp = pDivision->pGet(0);
      RASSERT(AM::szJobName(pOp->pOpJob())->bSame("startOp"_Stz));
      pOp->vAppendLeft(new Marker(OM_CANCELMARKER));
    }
  }
  
  for (i8 i_Ptn = 0; i_Ptn < m_oDivisionVec.nItems(); i_Ptn++) {
    Division* pDivision = m_oDivisionVec.pGet(i_Ptn);
    
    i8 iPtnNode = TablePtn::iPartitionToNode(pDivision->i_Ptn());
    
    if (iPtnNode == iThisNode) {
      Op* pOp = pDivision->pGet(0);
      RASSERT(AM::szJobName(pOp->pOpJob())->bSame("startOp"_Stz));
      pOp->vAppendLeft(new Marker(OM_CANCELMARKER));
    }
  }
  
}

/*----------------------------------------------------------------------------*/
/* get stage spec                                                             */
/*----------------------------------------------------------------------------*/
void Stage::vDump(Pipeline* pPipeline)
{
  _precondition_(_MAGIC_,CN_STAGE,_FOOTER_);
  
  for (i8 i_Op = 0; i_Op < m_oOpVec.nItems(); i_Op++) {
    
    Op* pOp = m_oOpVec.pGet(i_Op);
    
    s256 sArgs;
    
    EnameSet* pEnameSet = pOp->pEnameSet();
    pEnameSet->vSprint(sArgs);
    
    // get the next stage
    i8 iNextStage = pEnameSet->iValue("<nextStage>"_Stz);
    Stage* pNextStage = pPipeline->pGet(iNextStage);
    
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
  
  for (i8 i_Ptn = 0; i_Ptn < m_oDivisionVec.nItems(); i_Ptn++) {
    
    Division* pStagePtn = m_oDivisionVec.pGet(i_Ptn);
    
    // skip if partition is not for this node
    i8 iPartitionNode = TablePtn::iPartitionToNode(pStagePtn->i_Ptn());
    if (iPartitionNode == iNode) {
      nOps += pStagePtn->nItems();
    }
  }
  
  for (i8 i_Nde = 0; i_Nde < m_oStageNdeVec.nItems(); i_Nde++) {
    
    Division* pDivision = m_oStageNdeVec.pGet(i_Nde);
    
    // skip if Op not for this node
    if (pDivision->i_Nde() == iNode) {
      nOps += pDivision->nItems();
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
  Division* pStageNde = nullptr;
  Division* pStagePtn = nullptr;
  
  switch (eTopologyX) {
      
    case PX_ALL_NODES_TO_ORIGIN:
    case PX_ALL_PARTITIONS_TO_ORIGIN:
    case PX_ORIGIN_TO_ORIGIN:
    case PX_SPECIFIC_PARTITION_TO_ORIGIN:
      pStageNde = this->pStageNde(iNde);
      pOp = pStageNde->pGet(iOp);
      break;
      
    case PX_ORIGIN_TO_ALL_PARTITIONS:
    case PX_ORIGIN_TO_SPECIFIC_PARTITION:
        pStagePtn = m_oDivisionVec.pGet(iPtn);
        pOp = pStagePtn->pGet(iOp);
      break;
      
    case PX_ALL_PARTITIONS_TO_ALL_NODES:
      pStageNde = this->pStageNde(iNde);
      pOp = pStageNde->pGet(iOp);
      break;
      
    case PX_ALL_PARTITIONS_TO_ALL_PARTITIONS:
      pStagePtn = m_oDivisionVec.pGet(iPtn);
      pOp = pStagePtn->pGet(iOp);
      break;
      
    case PX_SPECIFIC_PARTITION_TO_SPECIFIC_PARTITION:
      pStagePtn = m_oDivisionVec.pGet(0);
      ASSERT(pStagePtn->i_Ptn() == iPtn);
      pOp = pStagePtn->pGet(iOp);
      break;
      
    default:
      RASSERT(0);
  }
  
  return pOp;  
}

/*----------------------------------------------------------------------------*/
/* get stage node for given node                                              */
/*----------------------------------------------------------------------------*/
Division* Stage::pStageNde(i8 i_Nde)
{
  _precondition_(_MAGIC_,CN_STAGE,_FOOTER_);
  
  Division* pStageNde = nullptr;
  
  for (i8 iNdeVec = 0; iNdeVec < m_oStageNdeVec.nItems(); iNdeVec++) {
    pStageNde = m_oStageNdeVec.pGet(iNdeVec);
    if (pStageNde->i_Nde() == i_Nde)
      return pStageNde;
  }
  
  return pStageNde;
}

/*----------------------------------------------------------------------------*/
/* get stage spec                                                             */
/*----------------------------------------------------------------------------*/
StagePtnVec* Stage::pStageNdeVec(void)
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
StagePtnVec* Stage::pDivisionVec(void)
{
  _precondition_(_MAGIC_,CN_STAGE,_FOOTER_);
  
  return &m_oDivisionVec;
}

/*----------------------------------------------------------------------------*/
/* get stage ptn for given ptn                                                */
/*----------------------------------------------------------------------------*/
Division* Stage::pDivision(i8 i_Ptn)
{
  _precondition_(_MAGIC_,CN_STAGE,_FOOTER_);
  
  Division* pStagePtn = nullptr;
  
  for (i8 iPtnVec = 0; iPtnVec < m_oDivisionVec.nItems(); iPtnVec++) {
    pStagePtn = m_oDivisionVec.pGet(iPtnVec);
    if (pStagePtn->i_Ptn() == i_Ptn)
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
  Division* pStageNde = m_oStageNdeVec.pGet(0);

  if (pStageNde->i_Nde() == AM::pGrid()->iThisNode()) {
    
    // get first operator of this first stage
    Op* pOp = pStageNde->pGet(0);
    RASSERT(AM::szJobName(pOp->pOpJob())->bSame("startOp"_Stz));
    
    // append a STARTMARKER
    pOp->vAppendLeft(new Marker(OM_STARTMARKER));
  }
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* create divisions of this stage based on topology                           */
/*----------------------------------------------------------------------------*/
void Stage::vCreateDivisions(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STAGE,_FOOTER_);
  
  i8 i_Ptn;
  i8 i_Nde;
    
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // create StageNde's and StagePtn's depending on the toplogy of the stage
  
  switch (m_pStageSpec->eTopology()) {
      
    case PO_ALL_NODES:
      for (i_Nde = 0; i_Nde < AM::pGrid()->nConnectedNodes(); i_Nde++)
        m_oStageNdeVec.vAdd(new Division(m_pStageSpec, i_Nde));
      break;
      
    case PO_ALL_PARTITIONS:
      for (i_Ptn = 0; i_Ptn < AM::pGrid()->nPartitions(); i_Ptn++)
        m_oDivisionVec.vAdd(new Division(m_pStageSpec, i_Ptn));
      break;
      
    case PO_ORIGIN:
      if (AM::bVerbose()) {
        i8 iThisNode = AM::pGrid()->iThisNode();
        if (iThisNode == 0) {
          vTrace("Pipeline Originating node : %lld\n"_Stz,
                 m_pPipeline->iOriginatingNode());
        }
      }
      m_oStageNdeVec.vAdd(new Division(m_pStageSpec, m_pPipeline->iOriginatingNode()));
      break;
      
    case PO_SPECIFIC_NODE:
      RASSERT(0);
      break;
      
    case PO_SPECIFIC_PARTITION:
      i_Ptn = m_pStageSpec->iPartition();
      m_oDivisionVec.vAdd(new Division(m_pStageSpec, i_Ptn));
      break;
      
    default:
      RASSERT(0);
  }
    
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // populate the opVec and StageNde's and stagePtn's from the opSpec
  
  for (i8 i_OpSpec = 0; i_OpSpec < m_pStageSpec->nItems(); i_OpSpec++ ) {
    
    OpSpec* pOpSpec = m_pStageSpec->pGet(i_OpSpec);
    
    // add the operator to the stages of the stageNdeVec
    for (i8 i_Index = 0; i_Index < m_oStageNdeVec.nItems(); i_Index++) {
      Division* pDivision = m_oStageNdeVec.pGet(i_Index);
      
      Op* pOp = new Op(pOpSpec,this,m_pPipeline);
      pOp->vNode(pDivision->i_Nde());
      pDivision->vAdd(pOp);
      m_oOpVec.vAdd(pOp);
    }
    
    // add the operator to the stages of the stagePtnVec
    for (i8 i_Ptn = 0; i_Ptn < m_oDivisionVec.nItems(); i_Ptn++) {
      Op* pOp = new Op(pOpSpec,this,m_pPipeline);
      Division* pStagePtn = m_oDivisionVec.pGet(i_Ptn);
      
      pOp->vPartition(pStagePtn->i_Ptn());
      pStagePtn->vAdd(pOp);
      m_oOpVec.vAdd(pOp);
    }
  }
  
  //
  // either the division vector is used, or the stage node vector is used
  //   but not both
  //
//  RASSERT(    (m_oDivisionVec.nItems()      && 0 == m_oStageNdeVec.nItems())
//           ||
//              (0 == m_oDivisionVec.nItems() && m_oStageNdeVec.nItems())
//          );
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(Stage)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
