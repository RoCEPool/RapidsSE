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
StageSpec::StageSpec(Stz* szStageNm, i8 i_StageSpec, PipelineSpec* pPipelineSpec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STAGESPEC,_FOOTER_);
  
  m_sPipelineNm    = pPipelineSpec->szPipelineNm();
  m_sStageNm       = szStageNm;
  
  m_i_StageSpec    = i_StageSpec;
  
  m_pJob           = pPipelineSpec->pJob();
  m_pPipelineSpec  = pPipelineSpec;

  m_eTopology      = PO_NONE;
  m_bNodeTopo      = true;
  m_i_SpecificNode = C_UNKNOWN;
  m_tSpecificPtnNo = C_UNKNOWN;
  //m_nThreads     = 1;
  
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
/* get i_Stage                                                                */
/*----------------------------------------------------------------------------*/
i8 StageSpec::i_StageSpec(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STAGESPEC,_FOOTER_);
  

  return m_i_StageSpec;
}

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
/* get bNodeTopo ?                                                            */
/*----------------------------------------------------------------------------*/
bool StageSpec::bNodeTopo(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STAGESPEC,_FOOTER_);
  
  return m_bNodeTopo;
}

/*----------------------------------------------------------------------------*/
/* get i_Stage                                                                */
/*----------------------------------------------------------------------------*/
PipelineSpec* StageSpec::pPipelineSpec(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STAGESPEC,_FOOTER_);
  
  return m_pPipelineSpec;
}

/*----------------------------------------------------------------------------*/
/* prepend a consumeOp spec                                                   */
/*----------------------------------------------------------------------------*/
OpSpec* StageSpec::pPrependConsumeOp(ePX eTopologyX, i8 iStage, i8 i_Op)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STAGESPEC,_FOOTER_);
  
  OpSpec* pOpSpec;
  
  pOpSpec = new OpSpec("consumeOp"_Stz,0,this);
  
  vInsert(pOpSpec);
  
  // renumber the m_i_OpSpec
  for (i8 i_OpSpec = 0; i_OpSpec < nItems(); i_OpSpec++) {
    OpSpec* pOpSpec = pGet(i_OpSpec);
    pOpSpec->v_OpSpec(i_OpSpec);
  }
  
  return pOpSpec;
}

/*----------------------------------------------------------------------------*/
/* get specific node                                                          */
/*----------------------------------------------------------------------------*/
i8 StageSpec::iSpecificNode(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STAGESPEC,_FOOTER_);
  
  return m_i_SpecificNode;
}

/*----------------------------------------------------------------------------*/
/* set specific node                                                          */
/*----------------------------------------------------------------------------*/
void StageSpec::vSpecificNode(i8 i_SpecificNode)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STAGESPEC,_FOOTER_);
  
  m_i_SpecificNode = i_SpecificNode;
}

/*----------------------------------------------------------------------------*/
/* get specific partition                                                     */
/*----------------------------------------------------------------------------*/
tPN StageSpec::tSpecificPtnNo(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STAGESPEC,_FOOTER_);
  
  return m_tSpecificPtnNo;
}

/*----------------------------------------------------------------------------*/
/* set specific partition                                                     */
/*----------------------------------------------------------------------------*/
void StageSpec::vSpecificPtnNo(tPN tSpecificPtnNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STAGESPEC,_FOOTER_);
  
  m_tSpecificPtnNo = tSpecificPtnNo;
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
  
  switch (m_eTopology) {
    case PO_ALL_NODES:                m_bNodeTopo = true;   break;
    case PO_ALL_PARTITIONS:           m_bNodeTopo = false;  break;
    case PO_ALL_PARTITIONS_ON_ORIGIN: m_bNodeTopo = false;  break;
    case PO_ORIGIN:                   m_bNodeTopo = true;   break;
    case PO_SPECIFIC_NODE:            m_bNodeTopo = true;   break;
    case PO_SPECIFIC_PARTITION:       m_bNodeTopo = false;  break;
    default:                          m_bNodeTopo = true;   break;
  }

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
    case PO_ALL_NODES:                vTrace("on all nodes"_Stz);
                                      break;
    case PO_ALL_PARTITIONS:           vTrace("on all partitions"_Stz);
                                      break;
    case PO_ALL_PARTITIONS_ON_ORIGIN: vTrace("on all partitions_on_origin"_Stz);
                                      break;
    case PO_ORIGIN:                   vTrace("on origin"_Stz);
                                      break;
    case PO_SPECIFIC_NODE:            vTrace("on node %s"_Stz,m_i_SpecificNode);
                                      break;
    case PO_SPECIFIC_PARTITION:       vTrace("on partition %s"_Stz,m_tSpecificPtnNo);
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
