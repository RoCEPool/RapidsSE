/*============================================================================*/
/* StageSpec.cpp -- Stage Specification                                       */
/*============================================================================*/

#include "PipelineSpecs.h"


/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* StageSpec::operator new(size_t)
{
  StageSpec* pStageSpec = reinterpret_cast<StageSpec*>(HeapMem::pAllocate(CN_STAGESPEC));
  
  return pStageSpec;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void StageSpec::operator delete(void* aAddr)
{
  StageSpec* pStageSpec = reinterpret_cast<StageSpec*>(aAddr);
  
  HeapMem::vFree(pStageSpec);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
StageSpec::StageSpec(Stz* szStageNm, PipelineSpec* pPipelineSpec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STAGESPEC,_FOOTER_);
  
  m_sStageNm      = szStageNm;
  m_pPipelineSpec = pPipelineSpec;
  
  m_eTopology     = PO_NONE;
  m_iNode         = C_UNKNOWN;
  m_iPartition    = C_UNKNOWN;
  //m_nThreads    = 1;
  
   _checkInitialised_(CN_STAGESPEC);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_STAGESPEC,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
StageSpec::~StageSpec(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STAGESPEC,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add op spec                                                                */
/*----------------------------------------------------------------------------*/
void StageSpec::vAddOpSpec(OpSpec* pOpSpec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STAGESPEC,_FOOTER_);
  
  pOpSpec->v_OpSpec(nItems());
  
  vAdd(pOpSpec);
}

/*----------------------------------------------------------------------------*/
/* copy ops from given stage spec                                             */
/*----------------------------------------------------------------------------*/
void StageSpec::vCopyOps(StageSpec* pStageSpec_from)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STAGESPEC,_FOOTER_);
    
  for (i8 i_Op = 0; i_Op < pStageSpec_from->nItems(); i_Op++) {
    OpSpec* pOpSpec_from = pStageSpec_from->pGet(i_Op);
    
    vAdd(pOpSpec_from);
  }
}

/*----------------------------------------------------------------------------*/
/* get name                                                                   */
/*----------------------------------------------------------------------------*/
Stz* StageSpec::szStageNm(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STAGESPEC,_FOOTER_);
  
  return m_sStageNm;
}

/*----------------------------------------------------------------------------*/
/* get node                                                                   */
/*----------------------------------------------------------------------------*/
i8 StageSpec::iNode(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STAGESPEC,_FOOTER_);
  
  return m_iNode;
}

/*----------------------------------------------------------------------------*/
/* set node                                                                   */
/*----------------------------------------------------------------------------*/
void StageSpec::vNode(i8 i_Nde)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STAGESPEC,_FOOTER_);
  
  m_iNode = i_Nde;
}

/*----------------------------------------------------------------------------*/
/* get partition                                                              */
/*----------------------------------------------------------------------------*/
i8 StageSpec::iPartition(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STAGESPEC,_FOOTER_);
  
  return m_iPartition;
}

/*----------------------------------------------------------------------------*/
/* set partition                                                              */
/*----------------------------------------------------------------------------*/
void StageSpec::vPartition(i8 i_Ptn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STAGESPEC,_FOOTER_);
  
  m_iPartition = i_Ptn;
}

/*----------------------------------------------------------------------------*/
/* prepend a consumeOp spec                                                   */
/*----------------------------------------------------------------------------*/
void StageSpec::vPrependConsumeOp(ePX eTopologyX, i8 iStage, i8 iOp)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STAGESPEC,_FOOTER_);
  
  OpSpec* pOpSpec;
  i8 iConsumeArg = C_UNKNOWN;
  
  pOpSpec = new OpSpec("consumeOp"_Stz,this);
  
  // add argument to the consumeOp depending on the topology
  switch (eTopologyX) {
    case PX_ALL_NODES_TO_ORIGIN:
      iConsumeArg = AM::pGrid()->nConnectedNodes();
      break;
      
    case PX_ALL_PARTITIONS_TO_ALL_NODES:
      iConsumeArg = AM::pGrid()->nSlotsPerNode();
      break;
      
    case PX_ALL_PARTITIONS_TO_ALL_PARTITIONS:
      iConsumeArg = 1;
      break;
      
    case PX_ALL_PARTITIONS_TO_ORIGIN:
      iConsumeArg = AM::pGrid()->nPartitions();
      break;
      
    case PX_ORIGIN_TO_ORIGIN:
      iConsumeArg = 1;
      break;
      
    case PX_ORIGIN_TO_ALL_PARTITIONS:
      iConsumeArg = 1;
      break;
      
    case PX_ORIGIN_TO_SPECIFIC_PARTITION:
      iConsumeArg = 1;
      break;
      
    case PX_SPECIFIC_PARTITION_TO_ORIGIN:
      iConsumeArg = 1;
      break;
      
    default:
      RASSERT(0);
  }
  
  pOpSpec->vAddEnamePair("<consumeXmits>"_Stz,iConsumeArg,TN_INTEGER);
  
  vInsert(pOpSpec);
  
  // rework the m_i_OpSpec
  for (i8 i_Op = 0; i_Op < nItems(); i_Op++) {
    OpSpec* pOpSpec = pGet(i_Op);
    pOpSpec->v_OpSpec(i_Op);
  }
  
}

/*----------------------------------------------------------------------------*/
/* get threads                                                                */
/*----------------------------------------------------------------------------*/
//i8 StageSpec::nThreads(void)
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_STAGESPEC,_FOOTER_);
//
//  return m_nThreads;
//}

/*----------------------------------------------------------------------------*/
/* set threads                                                                */
/*----------------------------------------------------------------------------*/
//void StageSpec::vThreads(i8 i_Threads)
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_STAGESPEC,_FOOTER_);
//  
//  m_nThreads = i_Threads;
//}

/*----------------------------------------------------------------------------*/
/* get topology                                                               */
/*----------------------------------------------------------------------------*/
ePO StageSpec::eTopology(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STAGESPEC,_FOOTER_);
  
  return m_eTopology;
}

/*----------------------------------------------------------------------------*/
/* set topology                                                               */
/*----------------------------------------------------------------------------*/
void StageSpec::vTopology(ePO eTopology)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STAGESPEC,_FOOTER_);
  
  m_eTopology = eTopology;
}

/*----------------------------------------------------------------------------*/
/* print stage spec                                                           */
/*----------------------------------------------------------------------------*/
void StageSpec::vToPipeStr(Stz* szPipeStr)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STAGESPEC,_FOOTER_);
  
  for (i8 i_Op = 0; i_Op < nItems(); i_Op++) {\
    OpSpec* pOpSpec = pGet(i_Op);

    if (i_Op)
      szPipeStr->vConcat(" | "_Stz);
    
    szPipeStr->vConcat(pOpSpec->szOpNm());
    
//    if (pOpSpec->szArgs()->iLength()) {
//      szPipeStr->vConcat(" "_Stz);
//      szPipeStr->vConcat(pOpSpec->szArgs());
//    }
  }

}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* print stage spec                                                           */
/*----------------------------------------------------------------------------*/
void StageSpec::vPrint(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STAGESPEC,_FOOTER_);
  
  vTrace("\n"_Stz);
  vTrace("stage %s "_Stz,m_sStageNm.szStz());
  
  switch (m_eTopology) {
    case PO_ALL_NODES:           vTrace("on all nodes"_Stz);
                                 break;
    case PO_ALL_PARTITIONS:      vTrace("on all partitions"_Stz);
                                 break;
    case PO_ORIGIN:              vTrace("on origin"_Stz);
                                 break;
    case PO_SPECIFIC_NODE:       vTrace("on node %s"_Stz,m_iNode);
                                 break;
    case PO_SPECIFIC_PARTITION:  vTrace("on partition %s"_Stz,m_iPartition);
                                 break;
    default:
      ;
  }
  vTrace(":\n"_Stz);


  vTrace("  "_Stz);
  for (i8 i_Op = 0; i_Op < nItems(); i_Op++) {
    if (i_Op)
      vTrace("| "_Stz);
    pGet(i_Op)->vPrint();
  }
  vTrace("\n"_Stz);
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(StageSpec)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
