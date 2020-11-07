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

  vCreateStrands();
  
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
/* send cancel marker to start ops of the stage                               */
/*----------------------------------------------------------------------------*/
void Stage::vCancelPipeline(Task* pTriggeringTask)
{
  _precondition_(_MAGIC_,CN_STAGE,_FOOTER_);
  
  // would the start marker always be on ORIGIN???
  
  // get first stage from stageNdeVec
  Strand*  pStrand = pGet(0);
  StageSpec* pStageSpec = pStrand->pStageSpec();
  
  if (pStageSpec->bNodeTopo() &&
      pStrand->i_Node() == AM::pGrid()->iThisNode()) {
    
    // get first operator of this first stage
    Op* pOp = pStrand->pGet(0);
    RASSERT(AM::szJobName(pOp->pOpJob())->bSame("start"_Stz));
    // append a CANCELMARKER onto first operator
    pOp->vAppendLeft(new Marker(OM_CANCELMARKER),pTriggeringTask);
  }
}

/*----------------------------------------------------------------------------*/
/* get strand for given partition                                             */
/*----------------------------------------------------------------------------*/
Strand* Stage::pGetPartitionStrand(i8 i_Ptn)
{
  _precondition_(_MAGIC_,CN_STAGE,_FOOTER_);
  
  Strand* pStrand   = nullptr;
  StageSpec* pStageSpec = nullptr;
  
  for (i8 i_Strand = 0; i_Strand < nItems(); i_Strand++) {
    pStrand  = pGet(i_Strand);
    pStageSpec = pStrand->pStageSpec();
    
    if ( ! pStageSpec->bNodeTopo() &&
        pStrand->tPtnNo() == i_Ptn)
      return pStrand;
  }
  
  return pStrand;
}

/*----------------------------------------------------------------------------*/
/* get strand for given node                                                  */
/*----------------------------------------------------------------------------*/
Strand* Stage::pGetNodeStrand(i8 i_Node)
{
  _precondition_(_MAGIC_,CN_STAGE,_FOOTER_);
  
  Strand* pStrand   = nullptr;
  StageSpec* pStageSpec = nullptr;
  
  for (i8 iNdeVec = 0; iNdeVec < nItems(); iNdeVec++) {
    pStrand  = pGet(iNdeVec);
    pStageSpec = pStrand->pStageSpec();
    
    if (pStageSpec->bNodeTopo() &&
        pStrand->i_Node() == i_Node)
      return pStrand;
  }
  
  return pStrand;
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

  for (i8 i_Strand = 0; i_Strand < nItems(); i_Strand++) {
    Strand* pStrand = pGet(i_Strand);
  
    pStrand->vDump();
  }
}

/*----------------------------------------------------------------------------*/
/* get ops to be executed on given node                                       */
/*----------------------------------------------------------------------------*/
i8 Stage::nOpsOnNode(i8 iNode)
{
  _precondition_(_MAGIC_,CN_STAGE,_FOOTER_);

  i8 nOps = 0;

  for (i8 i_Node = 0; i_Node < nItems(); i_Node++) {
    Strand* pStrand = pGet(i_Node);

    if (m_pStageSpec->bNodeTopo()) {
      // skip if Op not for this node
      if (pStrand->i_Node() == iNode) {
        nOps += pStrand->nItems();
      }
    }
    else {
      i8 iNode_exe = TablePtn::iPtnNoToNode(pStrand->tPtnNo());
      if (iNode_exe == iNode) {
        nOps += pStrand->nItems();
      }
    }
  }

  return nOps;
}

/*----------------------------------------------------------------------------*/
/* get pipeOut operator from the stage depending on cross topology            */
/*----------------------------------------------------------------------------*/
Op* Stage::pPipeOutOp(ePX eTopologyX, i8 i_Node, tPN tPtnNo, i8 iOp)
{
  Op* pOp = nullptr;
  Strand* pNdeStrand = nullptr;
  Strand* pPtnStrand = nullptr;
  
  switch (eTopologyX) {
      
    case PX_ALL_NODES_TO_ORIGIN:
    case PX_ALL_NODES_TO_ALL_NODES:
    case PX_ALL_PARTITIONS_TO_ORIGIN:
    case PX_ORIGIN_TO_ORIGIN:
    case PX_SPECIFIC_PARTITION_TO_ORIGIN:
    case PX_ORIGIN_TO_ALL_NODES:
      pNdeStrand = this->pGetNodeStrand(i_Node);
      pOp = pNdeStrand->pGet(iOp);
      break;
      
    case PX_ORIGIN_TO_ALL_PARTITIONS:
      pPtnStrand = pGet(tPtnNo);
      pOp = pPtnStrand->pGet(iOp);
      break;
      
    case PX_ALL_NODES_TO_ALL_PARTITIONS:
    case PX_ORIGIN_TO_SPECIFIC_PARTITION:
    case PX_ORIGIN_TO_ALL_PARTITIONS_ON_ORIGIN:
      pPtnStrand = pGetPartitionStrand(tPtnNo);
      pOp = pPtnStrand->pGet(iOp);
      break;
      
    case PX_ALL_PARTITIONS_TO_ALL_NODES:
      pNdeStrand = this->pGetNodeStrand(i_Node);
      pOp = pNdeStrand->pGet(iOp);
      break;
      
    case PX_ALL_PARTITIONS_TO_ALL_PARTITIONS:
      pPtnStrand = pGetPartitionStrand(tPtnNo);
      pOp = pPtnStrand->pGet(iOp);
      break;
      
    case PX_ALL_PARTITIONS_ON_ORIGIN_TO_ALL_PARTITIONS:
      pPtnStrand = pGetPartitionStrand(tPtnNo);
      pOp = pPtnStrand->pGet(iOp);
      break;
      
    case PX_SPECIFIC_PARTITION_TO_SPECIFIC_PARTITION:
      pPtnStrand = pGet(0);
      ASSERT(pPtnStrand->tPtnNo() == tPtnNo);
      pOp = pPtnStrand->pGet(iOp);
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
/* get stage spec                                                             */
/*----------------------------------------------------------------------------*/
StageSpec* Stage::pStageSpec(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STAGE,_FOOTER_);
  
  return m_pStageSpec;
}

/*----------------------------------------------------------------------------*/
/* send startMarker to start op of the stage                                  */
/*----------------------------------------------------------------------------*/
void Stage::vStartPipeline(Task* pTriggeringTask)
{
  _precondition_(_MAGIC_,CN_STAGE,_FOOTER_);
  
  // would the start marker always be on ORIGIN???
  
  // get first stage from stageNdeVec
  Strand*  pStrand = pGet(0);
  StageSpec* pStageSpec = pStrand->pStageSpec();
  
  if (pStageSpec->bNodeTopo() &&
      pStrand->i_Node() == AM::pGrid()->iThisNode()) {
    
    // get first operator of this first stage
    Op* pOp = pStrand->pGet(0);
    if (! AM::szJobName(pOp->pOpJob())->bSame("eatAll"_Stz) &&
        ! AM::szJobName(pOp->pOpJob())->bSame("statWrite"_Stz)) {

      RASSERT(AM::szJobName(pOp->pOpJob())->bSame("start"_Stz));
      // append a STARTMARKER onto first operator
      pOp->vAppendLeft(new Marker(OM_STARTMARKER),pTriggeringTask);
    }
  }
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* create the strands for this stage based on stage topology                  */
/*----------------------------------------------------------------------------*/
void Stage::vCreateStrands(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STAGE,_FOOTER_);
  
  tPN t_PtnNo;
  i8 i_Node;
    
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // create StageNde's and StagePtn's depending on the toplogy of the stage
  
  switch (m_pStageSpec->eTopology()) {
      
    case PO_ALL_NODES:
      RASSERT(m_pStageSpec->bNodeTopo());
      for (i_Node = 0; i_Node < AM::pGrid()->nConnectedNodes(); i_Node++)
        vAdd(new Strand(m_pStageSpec, i_Node));
      break;
      
    case PO_ALL_PARTITIONS:
      RASSERT(! m_pStageSpec->bNodeTopo());
      for (t_PtnNo = 0; t_PtnNo < AM::pGrid()->nPartitionsInGrid(); t_PtnNo++)
        vAdd(new Strand(m_pStageSpec, t_PtnNo));
      break;
      
    case PO_ORIGIN:
      RASSERT(m_pStageSpec->bNodeTopo());
      if (AM::bVerbose()) {
        i8 iThisNode = AM::pGrid()->iThisNode();
        if (iThisNode == m_pPipeline->iOriginatingNode()) {
          vTrace("Pipeline Originating node : %lld\n"_Stz,
                 m_pPipeline->iOriginatingNode());
        }
      }
      vAdd(new Strand(m_pStageSpec, m_pPipeline->iOriginatingNode()));
      break;
      
    case PO_ALL_PARTITIONS_ON_ORIGIN:
      RASSERT( ! m_pStageSpec->bNodeTopo());
      for (t_PtnNo = 0; t_PtnNo < AM::pGrid()->nPartitionsInGrid(); t_PtnNo++) {
        i_Node = TablePtn::iPtnNoToNode(t_PtnNo);
        if (i_Node == m_pPipeline->iOriginatingNode())
          vAdd(new Strand(m_pStageSpec, t_PtnNo));
      }
      break;
      
    case PO_SPECIFIC_NODE:
      RASSERT(m_pStageSpec->bNodeTopo());
      RASSERT(0);
      break;
      
    case PO_SPECIFIC_PARTITION:
      RASSERT(! m_pStageSpec->bNodeTopo());
      t_PtnNo = m_pStageSpec->tPtnNo();
      vAdd(new Strand(m_pStageSpec, t_PtnNo));
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
      Strand* pStrand = pGet(i_Index);
      
      if (m_pStageSpec->bNodeTopo()) {
        Op* pOp = new Op(pOpSpec,this,m_pPipeline);
        pOp->vNode(pStrand->i_Node());
        pStrand->vAdd(pOp);
      }
      else {
        Op* pOp = new Op(pOpSpec,this,m_pPipeline);
        pOp->vPtnNo(pStrand->tPtnNo());
        pStrand->vAdd(pOp);
      }
    }
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
