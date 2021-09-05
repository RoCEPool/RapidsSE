/*============================================================================*/
/* StageSpec.cpp -- Stage Specification                                       */
/*============================================================================*/

#include "BASE.h"

#include "OpSpec.hpp"
#include "PipelineSpec.hpp"
#include "StageSpec.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__pCastFunction(STORE,StageSpec,CN_STAGESPEC)
__aNewFunction(StageSpec,CN_STAGESPEC)
__vDeleteFunction(StageSpec,CN_STAGESPEC)
__aFooterFunction(StageSpec)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
StageSpec::StageSpec(i8 i_StageSpec,
                     Stz* szStageNm,
                     PipelineSpec* pPipelineSpec)
: DeleteHelper_t<StageSpec>(this,CN_STAGESPEC)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STAGESPEC,_FOOTER_);
  
  m_sStageNm       = szStageNm;
  
  m_i_StageSpec    = i_StageSpec;
  
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
  
  i8 nOpSpecs = this->nItems();
  for (i8 i_Op = 0; i_Op < nOpSpecs; i_Op++) {
    OpSpec* pOpSpec = pGet(i_Op);
    
    pOpSpec->vGuardedDelete(&pOpSpec,this,(Stz*)__func__);
  }

  vClear();
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get cross topology between two stages                                      */
/*----------------------------------------------------------------------------*/
ePX StageSpec::eCrossTopo(StageSpec* pFromStageSpec, StageSpec* pToStageSpec)
{
  _static_
    
  ePO eFromTopo = pFromStageSpec->eTopology();
  ePO eToTopo   = pToStageSpec->eTopology();
  
  switch (eFromTopo) {
          
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // from all nodes to ...
    case PO_ALL_NODES:
      switch(eToTopo) {
        case PO_ALL_NODES:          return PX_ALL_NODES_TO_ALL_NODES;
        case PO_ALL_PARTITIONS:     return PX_ALL_NODES_TO_ALL_PARTITIONS;
        case PO_ALL_PARTITIONS_ON_ORIGIN:  RASSERT(0);
        case PO_ORIGIN:             return PX_ALL_NODES_TO_ORIGIN;
        case PO_SPECIFIC_NODE:      return PX_ALL_NODES_TO_SPECIFIC_NODE;
        case PO_SPECIFIC_PARTITION: return PX_ALL_NODES_TO_SPECIFIC_PARTITION;
        default:
          RASSERT(0);
      }
      
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // from all partitions to ...
    case PO_ALL_PARTITIONS:
      switch(eToTopo) {
        case PO_ALL_NODES:          return PX_ALL_PARTITIONS_TO_ALL_NODES;
        case PO_ALL_PARTITIONS:     return PX_ALL_PARTITIONS_TO_ALL_PARTITIONS;
        case PO_ALL_PARTITIONS_ON_ORIGIN:  RASSERT(0);
        case PO_ORIGIN:             return PX_ALL_PARTITIONS_TO_ORIGIN;
        case PO_SPECIFIC_NODE:      return PX_ALL_PARTITIONS_TO_SPECIFIC_NODE;
        case PO_SPECIFIC_PARTITION: return PX_ALL_PARTITIONS_TO_SPECIFIC_PARTITION;
        default:
          RASSERT(0);
      }
      
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // from all partitions on origin to ...
    case PO_ALL_PARTITIONS_ON_ORIGIN:
      switch(eToTopo) {
        case PO_ALL_NODES:          RASSERT(0);
        case PO_ALL_PARTITIONS:     return PX_ALL_PARTITIONS_ON_ORIGIN_TO_ALL_PARTITIONS;
        case PO_ALL_PARTITIONS_ON_ORIGIN:  return PX_ALL_PARTITIONS_TO_ALL_PARTITIONS;
        case PO_ORIGIN:             RASSERT(0);
        case PO_SPECIFIC_NODE:      RASSERT(0);
        case PO_SPECIFIC_PARTITION: RASSERT(0);
        default:
          RASSERT(0);
      }
      
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // from origin to ...
    case PO_ORIGIN:
      switch(eToTopo) {
        case PO_ALL_NODES:          return PX_ORIGIN_TO_ALL_NODES;
        case PO_ALL_PARTITIONS:     return PX_ORIGIN_TO_ALL_PARTITIONS;
        case PO_ALL_PARTITIONS_ON_ORIGIN: return PX_ORIGIN_TO_ALL_PARTITIONS_ON_ORIGIN;
        case PO_ORIGIN:             return PX_ORIGIN_TO_ORIGIN;
        case PO_SPECIFIC_NODE:      return PX_ORIGIN_TO_SPECIFIC_NODE;
        case PO_SPECIFIC_PARTITION: return PX_ORIGIN_TO_SPECIFIC_PARTITION;
        default:
          RASSERT(0);
      }
      
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // from specific node to ...
    case PO_SPECIFIC_NODE:
      switch(eToTopo) {
        case PO_ALL_NODES:          return PX_SPECIFIC_NODE_TO_ALL_NODES;
        case PO_ALL_PARTITIONS:     return PX_SPECIFIC_NODE_TO_ALL_PARTITIONS;
        case PO_ALL_PARTITIONS_ON_ORIGIN:  RASSERT(0);
        case PO_ORIGIN:             return PX_SPECIFIC_NODE_TO_ORIGIN;
        case PO_SPECIFIC_NODE:      return PX_SPECIFIC_NODE_TO_SPECIFIC_NODE;
        case PO_SPECIFIC_PARTITION: return PX_SPECIFIC_NODE_TO_SPECIFIC_PARTITION;
        default:
          RASSERT(0);
      }
      
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // from specific partition to ...
    case PO_SPECIFIC_PARTITION:
      switch(eToTopo) {
        case PO_ALL_NODES:          return PX_SPECIFIC_PARTITION_TO_ALL_NODES;
        case PO_ALL_PARTITIONS:     return PX_SPECIFIC_PARTITION_TO_ALL_PARTITIONS;
        case PO_ALL_PARTITIONS_ON_ORIGIN:  RASSERT(0);
        case PO_ORIGIN:             return PX_SPECIFIC_PARTITION_TO_ORIGIN;
        case PO_SPECIFIC_NODE:      return PX_SPECIFIC_PARTITION_SPECIFIC_NODE;
        case PO_SPECIFIC_PARTITION: return PX_SPECIFIC_PARTITION_TO_SPECIFIC_PARTITION;
        default:
          RASSERT(0);
      }
      
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // ugh
    default:
      RASSERT(0);
  }
  
  return PX_NONE;
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
  
  pOpSpec->vOpSpec(nItems());
  
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
/* find op matching given name                                                */
/*----------------------------------------------------------------------------*/
OpSpec* StageSpec::pFindOpSpec(Stz* szFindNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STAGESPEC,_FOOTER_);

  for (i8 i_OpSpec = 0; i_OpSpec < nItems(); i_OpSpec++) {
    OpSpec* pOpSpec = pGet(i_OpSpec);

    if (pOpSpec->szOpNm()->bSame(szFindNm))
      return pOpSpec;
  }

  return nullptr;
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
/* get op spec                                                                */
/*----------------------------------------------------------------------------*/
OpSpec* StageSpec::pOpSpec(i8 i_Op)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STAGESPEC,_FOOTER_);
  
  OpSpec* pOpSpec = pGet(i_Op);
  
  return pOpSpec;
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
/* renumber all the op specs                                                  */
/*----------------------------------------------------------------------------*/
void StageSpec::vRenumberOpSpecs(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STAGESPEC,_FOOTER_);
  
  for (i8 i_OpSpec = 0; i_OpSpec < nItems(); i_OpSpec++) {
    OpSpec* pOpSpec = pGet(i_OpSpec);
    pOpSpec->vOpSpec(i_OpSpec);
  }
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
/* get name                                                                   */
/*----------------------------------------------------------------------------*/
Stz* StageSpec::szStageNm(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STAGESPEC,_FOOTER_);
  
  return m_sStageNm;
}

/*----------------------------------------------------------------------------*/
/* get i_Stage                                                                */
/*----------------------------------------------------------------------------*/
i8 StageSpec::i_StageSpec(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STAGESPEC,_FOOTER_);
  

  return m_i_StageSpec;
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

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
