/*============================================================================*/
/* Stage.cpp -- Stage of Pipeline                                             */
/*============================================================================*/

#include "BASE.h"

#include "eYX.h"

#include "Marker.hpp"

#include "LglPtnMap.hpp"

#include "Grid.hpp"

#include "Op.hpp"
#include "Stream.hpp"
#include "Stage.hpp"
#include "Vertex.hpp"
#include "Pipeline.hpp"
#include "VertexSpec.hpp"
#include "StageSpec.hpp"
#include "SP_Stage.hpp"
#include "TagConstants.h" //?
#include "Deck.hpp"
#include "MpdPtn.hpp"

extern AttachMap gxAttachMap;

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, Stage, CN_STAGE)
__aNewFunction(Stage, CN_STAGE)
__vDeleteFunction(Stage, CN_STAGE)
__aFooterFunction(Stage)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Stage::Stage(Deck* pDeck, StageSpec *pStageSpec)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_STAGE, _FOOTER_);

  m_pDeck      = pDeck;
  m_pStageSpec = pStageSpec;

  m_i_Stage    = pStageSpec->i_StageSpec();

  _checkInitialised_(CN_STAGE);

  vCreateStreams();

  _postcondition_(_ADDRESS_, _MAGIC_, CN_STAGE, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Stage::~Stage(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_STAGE, _FOOTER_);

//  i8 nStreams = this->nItems();
//  for (i8 i_Stream = 0; i_Stream < nStreams; i_Stream++) {
//    Stream *pStream = pGet(i_Stream);
//
//    delete pStream;
//  }
//
//  vClear();
  
  while (Stream* pStream = pPop())
    delete pStream;
  vCheckEmpty();
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* send cancel marker to start ops of the stage                               */
/*----------------------------------------------------------------------------*/
void Stage::vCancelPipeline(Task *pTriggeringTask)
{
  _precondition_(_MAGIC_, CN_STAGE, _FOOTER_);

  // would the start marker always be on ORIGIN???

  // get first stage from stageNdeVec
  Stream *pStream = pGet(0);
  StageSpec *pStageSpec = pStream->pStageSpec();

  if (pStageSpec->bNodeTopo() &&
      pStream->iNodeId() == AM::pGrid()->iThisNodeId()) {

    // get first operator of this first stage
    Op *pOp = pStream->pGet(0);
    RASSERT(pOp->szOpNm()->bSame("start"_Stz));
    // append a CANCELMARKER onto first operator
    pOp->vAppendLeft(new Marker(OM_CANCELMARKER), pTriggeringTask);
  }
}

/*----------------------------------------------------------------------------*/
/* get deck                                                                   */
/*----------------------------------------------------------------------------*/
Deck *Stage::pDeck(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_STAGE, _FOOTER_);

  return m_pDeck;
}

/*----------------------------------------------------------------------------*/
/* get stream for given node                                                  */
/*----------------------------------------------------------------------------*/
Stream *Stage::pGetStream_byNodeId(i8 i_Node)
{
  _precondition_(_MAGIC_, CN_STAGE, _FOOTER_);

  Stream *pStream = nullptr;
  StageSpec *pStageSpec = nullptr;

  for (i8 i_Stream = 0; i_Stream < nItems(); i_Stream++) {
    pStream = pGet(i_Stream);
    pStageSpec = pStream->pStageSpec();

    if (pStageSpec->bNodeTopo() &&
        pStream->iNodeId() == i_Node)
      return pStream;
  }

  return pStream;
}

/*----------------------------------------------------------------------------*/
/* get stream for given partition                                             */
/*----------------------------------------------------------------------------*/
Stream *Stage::pGetStream_byPtnNo(i8 i_Ptn)
{
  _precondition_(_MAGIC_, CN_STAGE, _FOOTER_);

  Stream *pStream = nullptr;
  StageSpec *pStageSpec = nullptr;

  for (i8 i_Stream = 0; i_Stream < nItems(); i_Stream++) {
    pStream = pGet(i_Stream);
    pStageSpec = pStream->pStageSpec();

    if (!pStageSpec->bNodeTopo() &&
        pStream->tLglPtn().iAsI8() == i_Ptn) {
      
      //RASSERT(pStream->i_Stream() == i_Ptn);
      return pStream;
    }
  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* get stream by streamNo                                                     */
/*----------------------------------------------------------------------------*/
Stream *Stage::pGetStream_byStreamNo(i8 i_Stream)
{
  _precondition_(_MAGIC_, CN_STAGE, _FOOTER_);

  Stream *pStream = pGet(i_Stream);

  return pStream;
}

/*----------------------------------------------------------------------------*/
/* get nOps that will executed by this stage on this given node               */
/*----------------------------------------------------------------------------*/
i8 Stage::nOpsExeOnNode(i8 iThisNodeId, i8 iOriginNodeId)
{
  _precondition_(_MAGIC_, CN_STAGE, _FOOTER_);

  i8 nOpsExe = 0;

  if (m_pStageSpec->bNodeTopo()) {
    // stage has node topology
    for (i8 i_Stream = 0; i_Stream < nItems(); i_Stream++) {
      Stream *pStream = pGet(i_Stream);

      if (YG_NEXT_NODE == m_pStageSpec->eStageTopology()) {
        if (MY::pNode()->iNodeId() != iOriginNodeId &&
            pStream->iNodeId() == iThisNodeId) {
          nOpsExe += pStream->nItems();
        }
      }
      else
      if (pStream->iNodeId() == iThisNodeId) {
        nOpsExe += pStream->nItems();
      }
    }
    
  }
  else {
    // stage has partitions topology
    for (i8 i_Stream = 0; i_Stream < nItems(); i_Stream++) {
      Stream *pStream = pGet(i_Stream);
      
      i8 iWhichNodeId = AM::pLglPtnMap()->iNodeId(pStream->tLglPtn());
      if (iWhichNodeId == iThisNodeId) {
        nOpsExe += pStream->nItems();
      }
    }
  }

  if (AM::TRACE::bVerbose(C_VB_BOOT))
    vTrace("Stage: %s nOps=%lld\n"_Stz
           ,m_pStageSpec->szStageNm()
           ,nOpsExe);

  return nOpsExe;
}

/*----------------------------------------------------------------------------*/
/* get outer pipeline                                                         */
/*----------------------------------------------------------------------------*/
Pipeline* Stage::pPipeline(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STAGE,_FOOTER_);
  
  return m_pDeck->pPipeline();
}

/*----------------------------------------------------------------------------*/
/* get vertex                                                                 */
/*----------------------------------------------------------------------------*/
Vertex *Stage::pVertex(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_STAGE, _FOOTER_);

  return m_pDeck->pVertex();
}

/*----------------------------------------------------------------------------*/
/* get op 0                                                                   */
/*----------------------------------------------------------------------------*/
Op* Stage::pOp0(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STAGE,_FOOTER_);
  
  Stream* pStream0 = this->pStream0();
  Op* pOp0 = pStream0->pOp0();

  return pOp0;
}

/*----------------------------------------------------------------------------*/
/* get pipeOut operator from the stage depending on cross topology            */
/*----------------------------------------------------------------------------*/
Op *Stage::pPipeDstOp(LglPtn tDstLglPtn, i8 i_DstOp)
{
  i8 i_ThisNode = AM::iThisNodeId();

  Op *pOp = nullptr;
  Stream *pStream = nullptr;

  eYS eStreamTopology = m_pStageSpec->eStreamTopology();
  
  switch (eStreamTopology) {

    case YS_NODE_STREAM:
      pStream = this->pGetStream_byNodeId(i_ThisNode);
      //vTrace("After pGetStream_byNodeId %p\n"_Stz, pStream);
      break;

    case YS_PTN_STREAM:
      pStream = pGetStream_byPtnNo(tDstLglPtn.iAsI8());
      break;

    default:
      vThrow(FK_FATAL, __func__, __FILE__, __LINE__,
             "Invalid eStreamTopology : %lld\n"_Stz, (i8) eStreamTopology);
  }

  if (!pStream) vTrace("Stream is null\n"_Stz);

  pOp = pStream->pGet(i_DstOp);

  return pOp;
}

/*----------------------------------------------------------------------------*/
/* get stage number                                                           */
/*----------------------------------------------------------------------------*/
i8 Stage::i_Stage(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_STAGE, _FOOTER_);

  return m_i_Stage;
}

/*----------------------------------------------------------------------------*/
/* get name                                                                   */
/*----------------------------------------------------------------------------*/
Stz *Stage::szStageNm(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_STAGE, _FOOTER_);

  return m_pStageSpec->szStageNm();
}

/*----------------------------------------------------------------------------*/
/* get stage spec                                                             */
/*----------------------------------------------------------------------------*/
StageSpec *Stage::pStageSpec(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_STAGE, _FOOTER_);

  return m_pStageSpec;
}

/*----------------------------------------------------------------------------*/
/* get stream 0                                                               */
/*----------------------------------------------------------------------------*/
Stream* Stage::pStream0(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STAGE,_FOOTER_);
  
  Stream* pStream0 = this->pGet(0);

  return pStream0;
}

/*----------------------------------------------------------------------------*/
/* test if to send startMarker to start op of the stage                       */
/*----------------------------------------------------------------------------*/
bool Stage::bEnqueueInvoke(void)
{
  _precondition_(_MAGIC_, CN_STAGE, _FOOTER_);

  // would the start marker always be on ORIGIN???

  // get first stage from stageNdeVec
  Stream *pStream = pGet(0);
  StageSpec *pStageSpec = pStream->pStageSpec();

  if (pStageSpec->bNodeTopo() &&
      pStream->iNodeId() == AM::pGrid()->iThisNodeId()) {

    // get first operator of this first stage
    Op *pOp = pStream->pGet(0);
    if (!pOp->szOpNm()->bSame("eatAll"_Stz) &&
        !pOp->szOpNm()->bSame("statWrite"_Stz)) {

      RASSERT(pOp->szOpNm()->bSame("start"_Stz));

      return true;
    }
  }

  return false;
}


/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* create the streams for this stage                                          */
/*----------------------------------------------------------------------------*/
void Stage::vCreateStreams(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_STAGE, _FOOTER_);
  LglPtn tLglPtn;

  i8 iMappedPtnNo;
  Stream *pStream;
  i8 iGridNodeId;
  i8 iOriginNodeId;
  i8 iMappedNodeId;

  //Node* pThisNode = MY::pNode();
  
  switch (m_pStageSpec->eStageTopology()) {

    //                      -- node topologies --                             //

    // create a stream in this stage for the origin node only
    case YG_ORIGIN_NODE:
      RASSERT(m_pStageSpec->bNodeTopo());
      
      iMappedPtnNo = 0;
      tLglPtn = C_UNKNOWN;
      iOriginNodeId = pPipeline()->iOriginNodeId();

      pStream = new Stream(this, m_pStageSpec,
                           tLglPtn, iOriginNodeId, iMappedPtnNo);
      vAppend(pStream);
      break;

    // create a stream in this stage for each node in the grid
    case YG_ALL_NODES:
      RASSERT(m_pStageSpec->bNodeTopo());
      
      iMappedPtnNo = 0;
      tLglPtn = C_UNKNOWN;
      
      for (i8 i_Node = 0; i_Node < AM::pGrid()->nKnownNodes();  i_Node++) {
        iGridNodeId = AM::GRID::iGetNthNodeId(i_Node);
        
        pStream = new Stream(this, m_pStageSpec,
                             tLglPtn, iGridNodeId, iMappedPtnNo);
        vAppend(pStream);
      }
      break;

    // create a stream in this stage for a next node only
    case YG_NEXT_NODE:
      RASSERT(m_pStageSpec->bNodeTopo());

      iMappedPtnNo = 0;
      tLglPtn = C_UNKNOWN;
      
      for (i8 i_Node = 0; i_Node < AM::pGrid()->nKnownNodes(); i_Node++) {
        iGridNodeId = AM::GRID::iGetNthNodeId(i_Node);
        
        pStream = new Stream(this, m_pStageSpec,
                             tLglPtn, iGridNodeId, iMappedPtnNo);
        vAppend(pStream);
      }
      break;

    //                    -- partition topologies --                          //

    // create a stream in this stage for all logical partitions in the grid
    case YG_ALL_PARTITIONS:
      RASSERT(!m_pStageSpec->bNodeTopo());

      for (MpdPtn tMpdPtn; tMpdPtn.bValid(); tMpdPtn++) {
        tLglPtn = tMpdPtn.tLglPtn();
        iMappedNodeId = AM::pLglPtnMap()->iNodeId(tLglPtn);
        iMappedPtnNo = AM::pLglPtnMap()->iPtnNo(tLglPtn);
        
        pStream = new Stream(this, m_pStageSpec,
                             tLglPtn, iMappedNodeId, iMappedPtnNo);
        vAppend(pStream);
      }
      break;

    // create a stream in this stage for all ptns that are on origin only
    case YG_ALL_PARTITIONS_ON_ORIGIN:
      RASSERT(!m_pStageSpec->bNodeTopo());

//      for (MpdPtn tMpdPtn; tMpdPtn.bValid(); tMpdPtn++) {
//        tLglPtn = tMpdPtn.tLglPtn();
//        iMappedNodeId = AM::pLglPtnMap()->iNodeId(tLglPtn);
//        if (iMappedNodeId != pPipeline()->iOriginNodeId())
//          continue;
//        iMappedPtnNo = AM::pLglPtnMap()->iPtnNo(tLglPtn);
//
//        pStream = new Stream(this, m_pStageSpec,
//                             tLglPtn, iMappedNodeId, iMappedPtnNo);
//        vAppend(pStream);
//      }
      for (MpdPtn tMpdPtn; tMpdPtn.bValid(); tMpdPtn++) {
        tMpdPtn.vMap();
        if (! tMpdPtn.bIsNode(pPipeline()->iOriginNodeId()))
          continue;

        tLglPtn       = tMpdPtn.tLglPtn();
        iMappedNodeId = tMpdPtn.iNodeId();
        iMappedPtnNo  = tMpdPtn.iPtnNo();
        
        pStream = new Stream(this, m_pStageSpec,
                             tLglPtn, iMappedNodeId, iMappedPtnNo);
        vAppend(pStream);
      }
      break;

    default:
      RASSERT(0);
  }

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
