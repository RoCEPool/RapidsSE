/*============================================================================*/
/* StageSpec.cpp -- Stage Specification                                       */
/*============================================================================*/

#include "BASE.h"

#include "OpSpec.hpp"
#include "StageSpec.hpp"
#include "PipeSpec.hpp"
#include "PipelineSpec.hpp"


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
                     PipeSpec* pPipeWorkSpec)
: ListNL_t<OpSpec>(CN_STAGESPEC)
, LifeGuard_t<StageSpec>(this,CN_STAGESPEC)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STAGESPEC,_FOOTER_);
  
  m_sStageNm        = szStageNm;
  m_pPipeWorkSpec   = pPipeWorkSpec;

  m_eStageTopology  = YG_NONE;
  m_eStreamTopology = YS_NONE;
  m_eParcelForward  = YF_NONE;

  m_bNodeTopo       = true;
  m_i_SpecificNode  = C_UNKNOWN;
  m_tSpecificPtnNo  = C_UNKNOWN;

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
                              eYF&       reParcelForward,
                              eYS&       reIntraTopology)
{
  _static_
    
  eYG eFromTopo = pFromStageSpec->eStageTopology();
  eYG eToTopo   = pToStageSpec->eStageTopology();
  
  reIntraTopology = YS_NONE;
  
  
  switch(eToTopo) {
    case YG_ALL_NODES:      reParcelForward = YF_ALL_NODES;
                            break;
      
    case YG_ALL_PARTITIONS: reParcelForward = YF_ALL_PARTITIONS;
                            break;
      
    case YG_ALL_PARTITIONS_ON_ORIGIN:
                            reParcelForward = YF_ALL_PARTITIONS_ON_ORIGIN;
                            break;
      
    case YG_NEXT_NODE:      reParcelForward = YF_NEXT_NODE;
                            break;
      
    case YG_ORIGIN:         reParcelForward = YF_ORIGIN;
                            break;
    default:
      vTrace("Invalid %s %s\n"_Stz,aYG[eToTopo]);
      RASSERT(0);
  }
  
  
  switch (eFromTopo) {
          
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // from all nodes to ...
    case YG_ALL_NODES:
      switch(eToTopo) {
          
        case YG_ALL_NODES:
          reIntraTopology = YS_NODE_STREAM;
          return YX_ALL_NODES_TO_ALL_NODES;
          
        case YG_ALL_PARTITIONS:
          reIntraTopology = YS_PTN_STREAM;
          return YX_ALL_NODES_TO_ALL_PARTITIONS;
          
        case YG_ALL_PARTITIONS_ON_ORIGIN:
          vTrace("Invalid %s %s\n"_Stz,aYG[eFromTopo],aYG[eToTopo]);
          RASSERT(0);
          
        case YG_ORIGIN:
          reIntraTopology = YS_NODE_STREAM;
          return YX_ALL_NODES_TO_ORIGIN;
                              
        default:
          vTrace("Invalid %s %s\n"_Stz,aYG[eFromTopo],aYG[eToTopo]);
          RASSERT(0);
      }
      
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // from all partitions to ...
    case YG_ALL_PARTITIONS:
      switch(eToTopo) {
          
        case YG_ALL_NODES:
          reIntraTopology = YS_NODE_STREAM;
          reParcelForward = YF_SAME_NODE;
          return YX_ALL_PARTITIONS_TO_ALL_NODES;
          
        case YG_ALL_PARTITIONS:
          reIntraTopology = YS_PTN_STREAM;
          return YX_ALL_PARTITIONS_TO_ALL_PARTITIONS;
          
        case YG_ALL_PARTITIONS_ON_ORIGIN:
          vTrace("Invalid %s %s\n"_Stz,aYG[eFromTopo],aYG[eToTopo]);
          RASSERT(0);
          
        case YG_ORIGIN:
          reIntraTopology = YS_NODE_STREAM;
          return YX_ALL_PARTITIONS_TO_ORIGIN;
                              
        default:
          vTrace("Invalid %s %s\n"_Stz,aYG[eFromTopo],aYG[eToTopo]);
          RASSERT(0);
      }
      
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // from all partitions on origin to ...
    case YG_ALL_PARTITIONS_ON_ORIGIN:
      switch(eToTopo) {
          
        case YG_ALL_NODES:
          //vTrace("Invalid %s %s\n"_Stz,aYG[eFromTopo],aYG[eToTopo]);
          //RASSERT(0);
          
          reIntraTopology = YS_NODE_STREAM;
          return YX_ALL_PARTITIONS_ON_ORIGIN_TO_ALL_NODES;

        case YG_ALL_PARTITIONS:
          reIntraTopology = YS_PTN_STREAM;
          return YX_ALL_PARTITIONS_ON_ORIGIN_TO_ALL_PARTITIONS;
          
        case YG_ALL_PARTITIONS_ON_ORIGIN:
          reIntraTopology = YS_PTN_STREAM;
          return YX_ALL_PARTITIONS_TO_ALL_PARTITIONS;
          
        case YG_ORIGIN:
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
          reIntraTopology = YS_NODE_STREAM;
          return YX_ORIGIN_TO_ALL_NODES;
          
        case YG_ALL_PARTITIONS:
          reIntraTopology = YS_PTN_STREAM;
          return YX_ORIGIN_TO_ALL_PARTITIONS;
          
        case YG_ALL_PARTITIONS_ON_ORIGIN:
          reIntraTopology = YS_PTN_STREAM;
          return YX_ORIGIN_TO_ALL_PARTITIONS_ON_ORIGIN;
          
        case YG_NEXT_NODE:
          reIntraTopology = YS_NODE_STREAM;
          return YX_ORIGIN_TO_NEXT_NODE;
          
        case YG_ORIGIN:
          reIntraTopology = YS_NODE_STREAM;
          return YX_ORIGIN_TO_ORIGIN;
                              
        default:
          vTrace("Invalid %s %s\n"_Stz,aYG[eFromTopo],aYG[eToTopo]);
          RASSERT(0);
      }
            
     
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // from a node which is a 'next node' to ...
    case YG_NEXT_NODE:
      
      switch(eToTopo) {
          
        case YG_ORIGIN:
          reIntraTopology = YS_NODE_STREAM;
          return YX_NEXT_NODE_TO_ORIGIN;

          case YG_ALL_NODES:
          reIntraTopology = YS_NODE_STREAM;
          return YX_ORIGIN_TO_ALL_NODES;

        default:
          vTrace("Invalid %s %s\n"_Stz,aYG[eFromTopo],aYG[eToTopo]);
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
/* calculate stream topology                                                  */
/*----------------------------------------------------------------------------*/
eYS StageSpec::eStreamTopoCalc(eYG eStageTopology)
{
  _static_
  
  switch (eStageTopology) {
      
    case YG_ALL_NODES:                return YS_NODE_STREAM;
    case YG_ALL_PARTITIONS:           return YS_PTN_STREAM;
    case YG_ALL_PARTITIONS_ON_ORIGIN: return YS_PTN_STREAM;
    case YG_ORIGIN:                   return YS_NODE_STREAM;
    case YG_NEXT_NODE:                return YS_NODE_STREAM;

    default:
      RASSERT(0);
  }
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
/* decode stage topology into aggreg level                                    */
/*----------------------------------------------------------------------------*/
eAL StageSpec::eAggegLevel(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STAGESPEC,_FOOTER_);
  
  eYG eStageTopology = this->eStageTopology();
  if (YG_ORIGIN == eStageTopology)
    return AL_ORIGIN;
  else
  if (YG_ALL_NODES == eStageTopology)
    return AL_NODE;
  else
  if (YG_ALL_PARTITIONS == eStageTopology)
    return AL_PARTITION;
  else
    NEVER_REACH;
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
/* get parcel forward                                                         */
/*----------------------------------------------------------------------------*/
eYF StageSpec::eParcelForward(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STAGESPEC,_FOOTER_);
  
  return m_eParcelForward;
}

/*----------------------------------------------------------------------------*/
/* set parcel forward                                                         */
/*----------------------------------------------------------------------------*/
void StageSpec::vParcelForward(eYF eParcelForward)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STAGESPEC,_FOOTER_);
  
  m_eParcelForward = eParcelForward;
}

/*----------------------------------------------------------------------------*/
/* get i_Stage                                                                */
/*----------------------------------------------------------------------------*/
//PipelineSpec* StageSpec::pPipelineSpec(void)
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_STAGESPEC,_FOOTER_);
//
//  return m_pPipeWorkSpec->pPipelineSpec();
//}

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
  
  i8 i_StageSpec = m_pPipeWorkSpec->iFind(this);
  
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
    case YG_NEXT_NODE:                m_bNodeTopo = true;   break;
    case YG_ORIGIN:                   m_bNodeTopo = true;   break;
    default:                          m_bNodeTopo = true;   break;
  }

  m_eStreamTopology = StageSpec::eStreamTopoCalc(m_eStageTopology);
}

/*----------------------------------------------------------------------------*/
/* get stream topology                                                        */
/*----------------------------------------------------------------------------*/
eYS StageSpec::eStreamTopology(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STAGESPEC,_FOOTER_);
  
  return m_eStreamTopology;
}

/*----------------------------------------------------------------------------*/
/* set stream topology                                                        */
/*----------------------------------------------------------------------------*/
void StageSpec::vStreamTopology(eYS eStreamTopology)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STAGESPEC,_FOOTER_);
  
  m_eStreamTopology = eStreamTopology;
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
