/*============================================================================*/
/* Stage.cpp -- Stage of Pipeline                                             */
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
  
  m_sPipelineNm = pPipeline->szPipelineNm();
  m_sStageNm    = pStageSpec->szStageNm();
  m_i_Stage     = pStageSpec->i_StageSpec();
  
  m_pStageSpec  = pStageSpec;
  m_pPipeline   = pPipeline;

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
      i8 iNode_exe = PtnMap::i_PtnNoToNode(pStrand->tPtnNo());
      if (iNode_exe == iNode) {
        nOps += pStrand->nItems();
      }
    }
  }

  return nOps;
}

/*----------------------------------------------------------------------------*/
/* get piepline                                                               */
/*----------------------------------------------------------------------------*/
Pipeline* Stage::pPipeline(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STAGE,_FOOTER_);
  
  return m_pPipeline;
}

/*----------------------------------------------------------------------------*/
/* get pipeOut operator from the stage depending on cross topology            */
/*----------------------------------------------------------------------------*/
Op* Stage::pPipeOutOp(ePX eCrossTopo, i8 i_Node, tPN tPtnNo, i8 i_Op)
{
  Op* pOp = nullptr;
  Strand* pNdeStrand = nullptr;
  Strand* pPtnStrand = nullptr;
  
  switch (eCrossTopo) {
      
    case PX_ALL_NODES_TO_ORIGIN:
    case PX_ALL_NODES_TO_ALL_NODES:
    case PX_ALL_PARTITIONS_TO_ORIGIN:
    case PX_ORIGIN_TO_ORIGIN:
    case PX_SPECIFIC_PARTITION_TO_ORIGIN:
    case PX_ORIGIN_TO_ALL_NODES:
      pNdeStrand = this->pGetNodeStrand(i_Node);
      pOp = pNdeStrand->pGet(i_Op);
      break;
      
    case PX_ORIGIN_TO_ALL_PARTITIONS:
      pPtnStrand = pGet(tPtnNo);
      pOp = pPtnStrand->pGet(i_Op);
      break;
      
    case PX_ALL_NODES_TO_ALL_PARTITIONS:
    case PX_ORIGIN_TO_SPECIFIC_PARTITION:
    case PX_ORIGIN_TO_ALL_PARTITIONS_ON_ORIGIN:
      pPtnStrand = pGetPartitionStrand(tPtnNo);
      pOp = pPtnStrand->pGet(i_Op);
      break;
      
    case PX_ALL_PARTITIONS_TO_ALL_NODES:
      pNdeStrand = this->pGetNodeStrand(i_Node);
      pOp = pNdeStrand->pGet(i_Op);
      break;
      
    case PX_ALL_PARTITIONS_TO_ALL_PARTITIONS:
      pPtnStrand = pGetPartitionStrand(tPtnNo);
      pOp = pPtnStrand->pGet(i_Op);
      break;
      
    case PX_ALL_PARTITIONS_ON_ORIGIN_TO_ALL_PARTITIONS:
      pPtnStrand = pGetPartitionStrand(tPtnNo);
      pOp = pPtnStrand->pGet(i_Op);
      break;
      
    case PX_SPECIFIC_PARTITION_TO_SPECIFIC_PARTITION:
      pPtnStrand = pGet(0);
      ASSERT(pPtnStrand->tPtnNo() == tPtnNo);
      pOp = pPtnStrand->pGet(i_Op);
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
  
  tPN tPtnNo;
  i8 i_Node;
  i8 i_Strand;

  Strand* pStrand;
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // create StageNde's and StagePtn's depending on the toplogy of the stage
  
  switch (m_pStageSpec->eTopology()) {
            
    // node topologies
      
    case PO_ALL_NODES:
      RASSERT(m_pStageSpec->bNodeTopo());
      i_Strand = 0;
      tPtnNo = C_UNKNOWN;
      for (i_Node = 0; i_Node < AM::pGrid()->nConnectedNodes(); i_Node++) {
        pStrand = new Strand(m_pStageSpec,this,i_Strand,tPtnNo,i_Node);
        vAdd(pStrand);
      }
      break;
      
    case PO_ORIGIN:
      RASSERT(m_pStageSpec->bNodeTopo());
      i_Strand = 0;
      tPtnNo = C_UNKNOWN;
      i_Node = m_pPipeline->i_Origin();
      pStrand = new Strand(m_pStageSpec,this,i_Strand,tPtnNo,i_Node);
      vAdd(pStrand);
      break;
        
    case PO_SPECIFIC_NODE:
      RASSERT(m_pStageSpec->bNodeTopo());
      RASSERT(0);
      break;
        
      
    // partition topologies

    case PO_ALL_PARTITIONS:
      RASSERT(! m_pStageSpec->bNodeTopo());
      for (tPtnNo = 0; tPtnNo < AM::pGrid()->nPartitionsInGrid(); tPtnNo++) {
        i_Strand = PtnMap::i_PtnNoToStrand(tPtnNo);
        i_Node = PtnMap::i_PtnNoToNode(tPtnNo);
        pStrand = new Strand(m_pStageSpec,this,i_Strand,tPtnNo,i_Node);
        vAdd(pStrand);
      }
      break;
      
    case PO_ALL_PARTITIONS_ON_ORIGIN:
      RASSERT(! m_pStageSpec->bNodeTopo());
      for (tPtnNo = 0; tPtnNo < AM::pGrid()->nPartitionsInGrid(); tPtnNo++) {
        i_Node = PtnMap::i_PtnNoToNode(tPtnNo);
        if (i_Node == m_pPipeline->i_Origin()) {
          i_Strand = PtnMap::i_PtnNoToStrand(tPtnNo);
          pStrand = new Strand(m_pStageSpec,this,i_Strand,tPtnNo,i_Node);
          vAdd(pStrand);
        }
      }
      break;
      
    case PO_SPECIFIC_PARTITION:
      RASSERT(! m_pStageSpec->bNodeTopo());
      tPtnNo = m_pStageSpec->tSpecificPtnNo();
      i_Node = PtnMap::i_PtnNoToNode(tPtnNo);
      i_Strand = PtnMap::i_PtnNoToStrand(tPtnNo);
      pStrand = new Strand(m_pStageSpec,this,i_Strand,tPtnNo,i_Node);
      vAdd(pStrand);
      break;
      
    default:
      RASSERT(0);
  }
    
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
