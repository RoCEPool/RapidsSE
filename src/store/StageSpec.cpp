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
StageSpec::StageSpec(Stz* szStageNm,
                     PipelineSpec* pPipelineSpec)
: ListNL_t<OpSpec>(CN_STAGESPEC)
, LifeGuard_t<StageSpec>(this,CN_STAGESPEC)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STAGESPEC,_FOOTER_);
  
  m_sStageNm       = szStageNm;
    
  m_pPipelineSpec  = pPipelineSpec;

  m_eStageTopology = YG_NONE;
  m_bNodeTopo      = true;
  m_i_SpecificNode = C_UNKNOWN;
  m_tSpecificPtnNo = C_UNKNOWN;

  _checkInitialised_(CN_STAGESPEC);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_STAGESPEC,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
StageSpec::~StageSpec(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STAGESPEC,_FOOTER_);
    
  while (OpSpec* pOpSpec = pPop())
    pOpSpec->vGuardedDelete(CN_OPSPEC,&pOpSpec,
                            CN_STAGESPEC,this,(Stz*)__func__);

  vClear();
}


/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get cross topology between two stages                                      */
/*----------------------------------------------------------------------------*/
eYX StageSpec::eCrossTopoCalc(StageSpec* pFromStageSpec,
                              StageSpec* pToStageSpec,
                              eYO&       reIntraTopology)
{
  _static_
    
  eYG eFromTopo = pFromStageSpec->eStageTopology();
  eYG eToTopo   = pToStageSpec->eStageTopology();
  
  reIntraTopology = YO_NONE;
  
  switch (eFromTopo) {
          
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // from all nodes to ...
    case YG_ALL_NODES:
      switch(eToTopo) {
          
        case YG_ALL_NODES:
          reIntraTopology = YO_STREAM_BY_NODE;
          return YX_ALL_NODES_TO_ALL_NODES;
          
        case YG_ALL_PARTITIONS:
          reIntraTopology = YO_STREAM_BY_PTN;
          return YX_ALL_NODES_TO_ALL_PARTITIONS;
          
        case YG_ALL_PARTITIONS_ON_ORIGIN:
          vTrace("Invalid %s %s\n"_Stz,aYG[eFromTopo],aYG[eToTopo]);
          RASSERT(0);
          
        case YG_ORIGIN:
          reIntraTopology = YO_STREAM_BY_NODE;
          return YX_ALL_NODES_TO_ORIGIN;
          
        case YG_SPECIFIC_NODE:
          vTrace("Invalid %s %s\n"_Stz,aYG[eFromTopo],aYG[eToTopo]);
          RASSERT(0);
          return YX_ALL_NODES_TO_SPECIFIC_NODE;
          
        case YG_SPECIFIC_PARTITION:
          vTrace("Invalid %s %s\n"_Stz,aYG[eFromTopo],aYG[eToTopo]);
          RASSERT(0);
          return YX_ALL_NODES_TO_SPECIFIC_PARTITION;
          
        default:
          vTrace("Invalid %s %s\n"_Stz,aYG[eFromTopo],aYG[eToTopo]);
          RASSERT(0);
      }
      
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // from all partitions to ...
    case YG_ALL_PARTITIONS:
      switch(eToTopo) {
          
        case YG_ALL_NODES:
          reIntraTopology = YO_STREAM_BY_NODE;
          return YX_ALL_PARTITIONS_TO_ALL_NODES;
          
        case YG_ALL_PARTITIONS:
          reIntraTopology = YO_STREAM_BY_PTN;
          return YX_ALL_PARTITIONS_TO_ALL_PARTITIONS;
          
        case YG_ALL_PARTITIONS_ON_ORIGIN:
          vTrace("Invalid %s %s\n"_Stz,aYG[eFromTopo],aYG[eToTopo]);
          RASSERT(0);
          
        case YG_ORIGIN:
          reIntraTopology = YO_STREAM_BY_NODE;
          return YX_ALL_PARTITIONS_TO_ORIGIN;
          
        case YG_SPECIFIC_NODE:
          vTrace("Invalid %s %s\n"_Stz,aYG[eFromTopo],aYG[eToTopo]);
          RASSERT(0);
          return YX_ALL_PARTITIONS_TO_SPECIFIC_NODE;
          
        case YG_SPECIFIC_PARTITION:
          vTrace("Invalid %s %s\n"_Stz,aYG[eFromTopo],aYG[eToTopo]);
          RASSERT(0);
          return YX_ALL_PARTITIONS_TO_SPECIFIC_PARTITION;
          
        default:
          vTrace("Invalid %s %s\n"_Stz,aYG[eFromTopo],aYG[eToTopo]);
          RASSERT(0);
      }
      
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // from all partitions on origin to ...
    case YG_ALL_PARTITIONS_ON_ORIGIN:
      switch(eToTopo) {
          
        case YG_ALL_NODES:
          vTrace("Invalid %s %s\n"_Stz,aYG[eFromTopo],aYG[eToTopo]);
          RASSERT(0);
          
        case YG_ALL_PARTITIONS:
          reIntraTopology = YO_STREAM_BY_PTN;
          return YX_ALL_PARTITIONS_ON_ORIGIN_TO_ALL_PARTITIONS;
          
        case YG_ALL_PARTITIONS_ON_ORIGIN:
          reIntraTopology = YO_STREAM_BY_PTN;
          return YX_ALL_PARTITIONS_TO_ALL_PARTITIONS;
          
        case YG_ORIGIN:
          vTrace("Invalid %s %s\n"_Stz,aYG[eFromTopo],aYG[eToTopo]);
          RASSERT(0);
          
        case YG_SPECIFIC_NODE:
          vTrace("Invalid %s %s\n"_Stz,aYG[eFromTopo],aYG[eToTopo]);
          RASSERT(0);
          
        case YG_SPECIFIC_PARTITION:
          vTrace("Invalid %s %s\n"_Stz,aYG[eFromTopo],aYG[eToTopo]);
          RASSERT(0);
          
        default:
          vTrace("Invalid %s %s\n"_Stz,aYG[eFromTopo],aYG[eToTopo]);
          RASSERT(0);
      }
      
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // from origin to ...
    case YG_ORIGIN:
      switch(eToTopo) {
          
        case YG_ALL_NODES:
          reIntraTopology = YO_STREAM_BY_NODE;
          return YX_ORIGIN_TO_ALL_NODES;
          
        case YG_ALL_PARTITIONS:
          reIntraTopology = YO_STREAM_BY_STREAM;
          return YX_ORIGIN_TO_ALL_PARTITIONS;
          
        case YG_ALL_PARTITIONS_ON_ORIGIN:
          reIntraTopology = YO_STREAM_BY_PTN;
          return YX_ORIGIN_TO_ALL_PARTITIONS_ON_ORIGIN;
          
        case YG_ORIGIN:
          reIntraTopology = YO_STREAM_BY_NODE;
          return YX_ORIGIN_TO_ORIGIN;
          
        case YG_SPECIFIC_NODE:
          vTrace("Invalid %s %s\n"_Stz,aYG[eFromTopo],aYG[eToTopo]);
          RASSERT(0);
          return YX_ORIGIN_TO_SPECIFIC_NODE;
          
        case YG_SPECIFIC_PARTITION:
          reIntraTopology = YO_STREAM_BY_PTN;
          return YX_ORIGIN_TO_SPECIFIC_PARTITION;
          
        default:
          vTrace("Invalid %s %s\n"_Stz,aYG[eFromTopo],aYG[eToTopo]);
          RASSERT(0);
      }
      
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // from specific node to ...
    case YG_SPECIFIC_NODE:
      switch(eToTopo) {
          
        case YG_ALL_NODES:
          vTrace("Invalid %s %s\n"_Stz,aYG[eFromTopo],aYG[eToTopo]);
          RASSERT(0);
          return YX_SPECIFIC_NODE_TO_ALL_NODES;
          
        case YG_ALL_PARTITIONS:
          vTrace("Invalid %s %s\n"_Stz,aYG[eFromTopo],aYG[eToTopo]);
          RASSERT(0);
          return YX_SPECIFIC_NODE_TO_ALL_PARTITIONS;
          
        case YG_ALL_PARTITIONS_ON_ORIGIN:
          vTrace("Invalid %s %s\n"_Stz,aYG[eFromTopo],aYG[eToTopo]);
          RASSERT(0);
          
        case YG_ORIGIN:
          vTrace("Invalid %s %s\n"_Stz,aYG[eFromTopo],aYG[eToTopo]);
          RASSERT(0);
          return YX_SPECIFIC_NODE_TO_ORIGIN;
          
        case YG_SPECIFIC_NODE:
          vTrace("Invalid %s %s\n"_Stz,aYG[eFromTopo],aYG[eToTopo]);
          RASSERT(0);
          return YX_SPECIFIC_NODE_TO_SPECIFIC_NODE;
          
        case YG_SPECIFIC_PARTITION:
          vTrace("Invalid %s %s\n"_Stz,aYG[eFromTopo],aYG[eToTopo]);
          RASSERT(0);
          return YX_SPECIFIC_NODE_TO_SPECIFIC_PARTITION;
          
        default:
          vTrace("Invalid %s %s\n"_Stz,aYG[eFromTopo],aYG[eToTopo]);
          RASSERT(0);
      }
      
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // from specific partition to ...
    case YG_SPECIFIC_PARTITION:
      switch(eToTopo) {
          
        case YG_ALL_NODES:
          vTrace("Invalid %s %s\n"_Stz,aYG[eFromTopo],aYG[eToTopo]);
          RASSERT(0);
          return YX_SPECIFIC_PARTITION_TO_ALL_NODES;
          
        case YG_ALL_PARTITIONS:
          vTrace("Invalid %s %s\n"_Stz,aYG[eFromTopo],aYG[eToTopo]);
          RASSERT(0);
          return YX_SPECIFIC_PARTITION_TO_ALL_PARTITIONS;
          
        case YG_ALL_PARTITIONS_ON_ORIGIN:
          vTrace("Invalid %s %s\n"_Stz,aYG[eFromTopo],aYG[eToTopo]);
          RASSERT(0);
          
        case YG_ORIGIN:
          reIntraTopology = YO_STREAM_BY_NODE;
          return YX_SPECIFIC_PARTITION_TO_ORIGIN;
          
        case YG_SPECIFIC_NODE:
          vTrace("Invalid %s %s\n"_Stz,aYG[eFromTopo],aYG[eToTopo]);
          RASSERT(0);
          return YX_SPECIFIC_PARTITION_SPECIFIC_NODE;
          
        case YG_SPECIFIC_PARTITION:
          reIntraTopology = YO_STREAM_0;
          return YX_SPECIFIC_PARTITION_TO_SPECIFIC_PARTITION;
          
        default:
          RASSERT(0);
      }
      
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // ugh
    default:
      RASSERT(0);
  }
  
  return YX_NONE;
}


/*----------------------------------------------------------------------------*/
/* get op topology                                                            */
/*----------------------------------------------------------------------------*/
eYO StageSpec::eIntraTopoCalc(StageSpec* pStageSpec)
{
  _static_
  
  eYG eStageTopo = pStageSpec->eStageTopology();
  
  eYO eIntraTopology = YO_NONE;
  
  switch (eStageTopo) {
      
    case YG_ALL_NODES:
      eIntraTopology = YO_STREAM_BY_NODE;
      break;
      
    case YG_ALL_PARTITIONS:
      eIntraTopology = YO_STREAM_BY_PTN;
      break;

    case YG_ALL_PARTITIONS_ON_ORIGIN:
      eIntraTopology = YO_STREAM_BY_PTN;
      break;

    case YG_ORIGIN:
      eIntraTopology = YO_STREAM_BY_NODE;
      break;

    case YG_SPECIFIC_NODE:
      vTrace("Invalid %s \n"_Stz,aYG[eStageTopo]);
      RASSERT(0);
      break;

    case YG_SPECIFIC_PARTITION:
      eIntraTopology = YO_STREAM_0;
      break;

    default:
      RASSERT(0);
  }
  
  
  return eIntraTopology;
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
    
  vAppend(pOpSpec);
}

/*----------------------------------------------------------------------------*/
/* copy ops from given stage spec                                             */
/*----------------------------------------------------------------------------*/
void StageSpec::vCopyOps(StageSpec* pStageSpec_from)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STAGESPEC,_FOOTER_);
    
  for (i8 i_Op = 0; i_Op < pStageSpec_from->nItems(); i_Op++) {
    OpSpec* pOpSpec_from = pStageSpec_from->pGet(i_Op);
    
    vAppend(pOpSpec_from);
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
PtnId StageSpec::tSpecificPtnNo(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STAGESPEC,_FOOTER_);

  return m_tSpecificPtnNo;
}

/*----------------------------------------------------------------------------*/
/* set specific partition                                                     */
/*----------------------------------------------------------------------------*/
void StageSpec::vSpecificPtnNo(PtnId tSpecificPtnNo)
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
  
  i8 i_StageSpec = m_pPipelineSpec->iFind(this);
  
  return i_StageSpec;
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
/* get stage topology                                                        */
/*----------------------------------------------------------------------------*/
eYG StageSpec::eStageTopology(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STAGESPEC,_FOOTER_);
  
  return m_eStageTopology;
}

/*----------------------------------------------------------------------------*/
/* set stage topology                                                         */
/*----------------------------------------------------------------------------*/
void StageSpec::vStageTopology(eYG eStageTopology)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STAGESPEC,_FOOTER_);
  
  m_eStageTopology = eStageTopology;
  
  switch (m_eStageTopology) {
    case YG_ALL_NODES:                m_bNodeTopo = true;   break;
    case YG_ALL_PARTITIONS:           m_bNodeTopo = false;  break;
    case YG_ALL_PARTITIONS_ON_ORIGIN: m_bNodeTopo = false;  break;
    case YG_ORIGIN:                   m_bNodeTopo = true;   break;
    case YG_SPECIFIC_NODE:            m_bNodeTopo = true;   break;
    case YG_SPECIFIC_PARTITION:       m_bNodeTopo = false;  break;
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
