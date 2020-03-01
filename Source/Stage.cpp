/*============================================================================*/
/* Stage.cpp -- Pipe Stage                                                    */
/*============================================================================*/

#include "Pipelines.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(Pipelines,Stage,CN_STAGE)
__aNewFunction(Stage,CN_STAGE)
__vDeleteFunction(Stage,CN_STAGE)
__aFooterFunction(Stage)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Stage::Stage(StageSpec* pStageSpec, Pipeline* pPipeline)
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
  
  for (i8 i_Nde = 0; i_Nde < nItems(); i_Nde++) {
    Strand* pDivision = pGet(i_Nde);
    
    if (m_pStageSpec->bNodeTopo()) {
      
      if (pDivision->i_Nde() == iThisNode) {
        Op* pOp = pDivision->pGet(0);
        RASSERT(AM::szJobName(pOp->pOpJob())->bSame("startOp"_Stz));
        pOp->vAppendLeft(new Marker(OM_CANCELMARKER));
      }
    }
    else {
      i8 iPtnNode = TablePtn::iPartitionToNode(pDivision->i_Ptn());
      
      if (iPtnNode == iThisNode) {
        Op* pOp = pDivision->pGet(0);
        RASSERT(AM::szJobName(pOp->pOpJob())->bSame("startOp"_Stz));
        pOp->vAppendLeft(new Marker(OM_CANCELMARKER));
      }
    }
  }
}

/*----------------------------------------------------------------------------*/
/* get division for given node                                                */
/*----------------------------------------------------------------------------*/
Strand* Stage::pGetNodeDivision(i8 i_Nde)
{
  _precondition_(_MAGIC_,CN_STAGE,_FOOTER_);
  
  Strand* pDivision   = nullptr;
  StageSpec* pStageSpec = nullptr;
  
  for (i8 iNdeVec = 0; iNdeVec < nItems(); iNdeVec++) {
    pDivision  = pGet(iNdeVec);
    pStageSpec = pDivision->pStageSpec();
    
    if (pStageSpec->bNodeTopo() &&
        pDivision->i_Nde() == i_Nde)
      return pDivision;
  }
  
  return pDivision;
}

/*----------------------------------------------------------------------------*/
/* get stage spec                                                             */
/*----------------------------------------------------------------------------*/
void Stage::vDump(Pipeline* pPipeline)
{
  _precondition_(_MAGIC_,CN_STAGE,_FOOTER_);
  
  i8 i_Stage = m_pStageSpec->i_StageSpec();
  vTrace("\n[%lld] Stage %s Topology %s: \n"_Stz,
         i_Stage,
         m_sStageNm.szStz(),
         aPO[m_pStageSpec->eTopology()]);

  for (i8 i_Div = 0; i_Div < nItems(); i_Div++) {
    Strand* pDiv = pGet(i_Div);
  
    pDiv->vDump();
  }
}

/*----------------------------------------------------------------------------*/
/* get ops to be executed on given node                                       */
/*----------------------------------------------------------------------------*/
i8 Stage::nOpsOnNode(i8 iNode)
{
  _precondition_(_MAGIC_,CN_STAGE,_FOOTER_);

  i8 nOps = 0;

  for (i8 i_Nde = 0; i_Nde < nItems(); i_Nde++) {
    Strand* pDivision = pGet(i_Nde);

    if (m_pStageSpec->bNodeTopo()) {
      // skip if Op not for this node
      if (pDivision->i_Nde() == iNode) {
        nOps += pDivision->nItems();
      }
    }
    else {
      i8 iPartitionNode = TablePtn::iPartitionToNode(pDivision->i_Ptn());
      if (iPartitionNode == iNode) {
        nOps += pDivision->nItems();
      }
    }
  }

  return nOps;
}

/*----------------------------------------------------------------------------*/
/* get pipeOut operator from the stage depending on cross topology            */
/*----------------------------------------------------------------------------*/
Op* Stage::pPipeOutOp(ePX eTopologyX, i8 iNde, i8 iPtn, i8 iOp)
{
  Op* pOp = nullptr;
  Strand* pNdeDivision = nullptr;
  Strand* pPtnDivision = nullptr;
  
  switch (eTopologyX) {
      
    case PX_ALL_NODES_TO_ORIGIN:
    case PX_ALL_PARTITIONS_TO_ORIGIN:
    case PX_ORIGIN_TO_ORIGIN:
    case PX_SPECIFIC_PARTITION_TO_ORIGIN:
      pNdeDivision = this->pGetNodeDivision(iNde);
      pOp = pNdeDivision->pGet(iOp);
      break;
      
    case PX_ORIGIN_TO_ALL_PARTITIONS:
    case PX_ORIGIN_TO_SPECIFIC_PARTITION:
        pPtnDivision = pGet(iPtn);
        pOp = pPtnDivision->pGet(iOp);
      break;
      
    case PX_ALL_PARTITIONS_TO_ALL_NODES:
      pNdeDivision = this->pGetNodeDivision(iNde);
      pOp = pNdeDivision->pGet(iOp);
      break;
      
    case PX_ALL_PARTITIONS_TO_ALL_PARTITIONS:
      pPtnDivision = pGet(iPtn);
      pOp = pPtnDivision->pGet(iOp);
      break;
      
    case PX_SPECIFIC_PARTITION_TO_SPECIFIC_PARTITION:
      pPtnDivision = pGet(0);
      ASSERT(pPtnDivision->i_Ptn() == iPtn);
      pOp = pPtnDivision->pGet(iOp);
      break;
      
    default:
      RASSERT(0);
  }
  
  return pOp;  
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
/* send startMarker to startop of the stage                                   */
/*----------------------------------------------------------------------------*/
void Stage::vStartPipeline(void)
{
  _precondition_(_MAGIC_,CN_STAGE,_FOOTER_);
  
  // would the start marker always be on ORIGIN???
  
  // get first stage from stageNdeVec
  Strand*  pDivision = pGet(0);
  StageSpec* pStageSpec = pDivision->pStageSpec();
  
  if (pStageSpec->bNodeTopo() &&
      pDivision->i_Nde() == AM::pGrid()->iThisNode()) {
    
    // get first operator of this first stage
    Op* pOp = pDivision->pGet(0);
    RASSERT(AM::szJobName(pOp->pOpJob())->bSame("startOp"_Stz));
    
    // append a STARTMARKER onto first operator
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
      RASSERT(m_pStageSpec->bNodeTopo());
      for (i_Nde = 0; i_Nde < AM::pGrid()->nConnectedNodes(); i_Nde++)
        vAdd(new Strand(m_pStageSpec, i_Nde));
      break;
      
    case PO_ALL_PARTITIONS:
      RASSERT(! m_pStageSpec->bNodeTopo());
      for (i_Ptn = 0; i_Ptn < AM::pGrid()->nPartitions(); i_Ptn++)
        vAdd(new Strand(m_pStageSpec, i_Ptn));
      break;
      
    case PO_ORIGIN:
      RASSERT(m_pStageSpec->bNodeTopo());
      if (AM::bVerbose()) {
        i8 iThisNode = AM::pGrid()->iThisNode();
        if (iThisNode == 0) {
          vTrace("Pipeline Originating node : %lld\n"_Stz,
                 m_pPipeline->iOriginatingNode());
        }
      }
      vAdd(new Strand(m_pStageSpec, m_pPipeline->iOriginatingNode()));
      break;
      
    case PO_SPECIFIC_NODE:
      RASSERT(m_pStageSpec->bNodeTopo());
      RASSERT(0);
      break;
      
    case PO_SPECIFIC_PARTITION:
      RASSERT(! m_pStageSpec->bNodeTopo());
      i_Ptn = m_pStageSpec->iPartition();
      vAdd(new Strand(m_pStageSpec, i_Ptn));
      break;
      
    default:
      RASSERT(0);
  }
    
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // populate the opVec and StageNde's and stagePtn's from the opSpec
  
  for (i8 i_OpSpec = 0; i_OpSpec < m_pStageSpec->nItems(); i_OpSpec++ ) {
    OpSpec* pOpSpec = m_pStageSpec->pGet(i_OpSpec);
    
    // add the operator to the stages of the stageNdeVec
    for (i8 i_Index = 0; i_Index < nItems(); i_Index++) {
      Strand* pDivision = pGet(i_Index);
      
      if (m_pStageSpec->bNodeTopo()) {
        Op* pOp = new Op(pOpSpec,this,m_pPipeline);
        pOp->vNode(pDivision->i_Nde());
        pDivision->vAdd(pOp);
      }
      else {
        Op* pOp = new Op(pOpSpec,this,m_pPipeline);
        pOp->vPartition(pDivision->i_Ptn());
        pDivision->vAdd(pOp);
      }
    }
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
