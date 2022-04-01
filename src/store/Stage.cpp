/*============================================================================*/
/* Stage.cpp -- Stage of Pipeline                                             */
/*============================================================================*/

#include "BASE.h"

#include "eYX.h"

#include "Marker.hpp"

#include "PtnMap.hpp"

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
Stage::Stage(Vertex* pPipeWork, Deck* pDeck, StageSpec *pStageSpec)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_STAGE, _FOOTER_);

  m_pVertex    = pPipeWork;
  m_pDeck      = pDeck;
  m_pStageSpec = pStageSpec;

  m_sStageNm = pStageSpec->szStageNm();
  m_i_Stage  = pStageSpec->i_StageSpec();


  _checkInitialised_(CN_STAGE);

  //vCreateStreams();

  _postcondition_(_ADDRESS_, _MAGIC_, CN_STAGE, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Stage::~Stage(void) noexcept(false) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_STAGE, _FOOTER_);

  i8 nStreams = this->nItems();
  for (i8 i_Stream = 0; i_Stream < nStreams; i_Stream++) {
    Stream *pStream = pGet(i_Stream);

    delete pStream;
  }

  vClear();
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* send cancel marker to start ops of the stage                               */
/*----------------------------------------------------------------------------*/
void Stage::vCancelPipeline(Task *pTriggeringTask) {
  _precondition_(_MAGIC_, CN_STAGE, _FOOTER_);

  // would the start marker always be on ORIGIN???

  // get first stage from stageNdeVec
  Stream *pStream = pGet(0);
  StageSpec *pStageSpec = pStream->pStageSpec();

  if (pStageSpec->bNodeTopo() &&
      pStream->iNodeNo() == AM::pGrid()->iThisNodeNo()) {

    // get first operator of this first stage
    Op *pOp = pStream->pGet(0);
    RASSERT(pOp->szOpNm()->bSame("start"_Stz));
    // append a CANCELMARKER onto first operator
    pOp->vAppendLeft(new Marker(OM_CANCELMARKER), pTriggeringTask);
  }
}


/*----------------------------------------------------------------------------*/
/* get stream for given node                                                  */
/*----------------------------------------------------------------------------*/
Stream *Stage::pGetStream_byNodeNo(i8 i_Node) {
  _precondition_(_MAGIC_, CN_STAGE, _FOOTER_);

  Stream *pStream = nullptr;
  StageSpec *pStageSpec = nullptr;

  for (i8 i_Stream = 0; i_Stream < nItems(); i_Stream++) {
    pStream = pGet(i_Stream);
    pStageSpec = pStream->pStageSpec();

    if (pStageSpec->bNodeTopo() &&
        pStream->iNodeNo() == i_Node)
      return pStream;
  }

  return pStream;
}

/*----------------------------------------------------------------------------*/
/* get stream for given partition                                             */
/*----------------------------------------------------------------------------*/
Stream *Stage::pGetStream_byPtnNo(i8 i_Ptn) {
  _precondition_(_MAGIC_, CN_STAGE, _FOOTER_);

  Stream *pStream = nullptr;
  StageSpec *pStageSpec = nullptr;

  for (i8 i_Stream = 0; i_Stream < nItems(); i_Stream++) {
    pStream = pGet(i_Stream);
    pStageSpec = pStream->pStageSpec();

    if (!pStageSpec->bNodeTopo() &&
        pStream->tPtnId().iAsI8() == i_Ptn)
      return pStream;
  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* get stream by streamNo                                                     */
/*----------------------------------------------------------------------------*/
Stream *Stage::pGetStream_byStreamNo(i8 i_Stream) {
  _precondition_(_MAGIC_, CN_STAGE, _FOOTER_);

  Stream *pStream = pGet(i_Stream);

  return pStream;
}

/*----------------------------------------------------------------------------*/
/* get ops to be executed on given node                                       */
/*----------------------------------------------------------------------------*/
i8 Stage::nOpsOnNode(i8 iNode, i8 iOrigin) {
  _precondition_(_MAGIC_, CN_STAGE, _FOOTER_);

  i8 nOps = 0;

  for (i8 i_Node = 0; i_Node < nItems(); i_Node++) {
    Stream *pStream = pGet(i_Node);

    if (m_pStageSpec->bNodeTopo()) {
      
      if (YG_NEXT_NODE == m_pStageSpec->eStageTopology()) {
        if (MY::pNode()->iNodeNo() != iOrigin && pStream->iNodeNo() == iNode) {
          nOps += pStream->nItems();
        }
      }
      else
      if (pStream->iNodeNo() == iNode) {
        // skip if Op not for this node
        nOps += pStream->nItems();
      }
    }
    else {
      i8 iNode_exe = AM::pPtnMap()->iWhichNodeNo(pStream->tPtnId());
      if (iNode_exe == iNode) {
        nOps += pStream->nItems();
      }
    }
  }

  if (AM::TRACE::bVerbose(C_VB_BOOT))
    vTrace("Stage: %s nOps=%lld\n"_Stz
           ,m_pStageSpec->szStageNm()
           ,nOps);

  return nOps;
}

/*----------------------------------------------------------------------------*/
/* get outer pipeline                                                         */
/*----------------------------------------------------------------------------*/
Pipeline* Stage::pPipeline(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STAGE,_FOOTER_);
  
  return m_pVertex->pPipeline();
}

/*----------------------------------------------------------------------------*/
/* get piepline                                                               */
/*----------------------------------------------------------------------------*/
Vertex *Stage::pVertex(void) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_STAGE, _FOOTER_);

  return m_pVertex;
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
Op *Stage::pPipeDstOp(PtnId tDstPtnId, i8 i_DstOp) {
  i8 i_ThisNode = AM::iThisNodeNo();

  Op *pOp = nullptr;
  Stream *pStream = nullptr;

  eYS eStreamTopology = m_pStageSpec->eStreamTopology();
  
  switch (eStreamTopology) {

    case YS_NODE_STREAM:
      pStream = this->pGetStream_byNodeNo(i_ThisNode);
      //vTrace("After pGetStream_byNodeNo %p\n"_Stz, pStream);
      break;

    case YS_PTN_STREAM:
      pStream = pGetStream_byPtnNo(tDstPtnId.iAsI8());
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
i8 Stage::i_Stage(void) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_STAGE, _FOOTER_);

  return m_i_Stage;
}

/*----------------------------------------------------------------------------*/
/* get name                                                                   */
/*----------------------------------------------------------------------------*/
Stz *Stage::szStageNm(void) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_STAGE, _FOOTER_);

  return m_sStageNm;
}

/*----------------------------------------------------------------------------*/
/* get stage spec                                                             */
/*----------------------------------------------------------------------------*/
StageSpec *Stage::pStageSpec(void) {
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
bool Stage::bEnqueueInvoke(void) {
  _precondition_(_MAGIC_, CN_STAGE, _FOOTER_);

  // would the start marker always be on ORIGIN???

  // get first stage from stageNdeVec
  Stream *pStream = pGet(0);
  StageSpec *pStageSpec = pStream->pStageSpec();

  if (pStageSpec->bNodeTopo() &&
      pStream->iNodeNo() == AM::pGrid()->iThisNodeNo()) {

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
/* create the stream instances for this stage based on stage topology         */
/*----------------------------------------------------------------------------*/
void Stage::vCreateStreams(void) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_STAGE, _FOOTER_);
  PtnId tPtnId;

  i8 iOriginNodeNo;
  i8 i_Stream;
  Stream *pStream;
  i8 iNodeNo;

  Node* pThisNode = MY::pNode();
  
  switch (m_pStageSpec->eStageTopology()) {

    //                      -- node topologies --                             //

    case YG_ALL_NODES: // create a stream for all nodes
      RASSERT(m_pStageSpec->bNodeTopo());
      i_Stream = 0;
      tPtnId = C_UNKNOWN;
      for (iOriginNodeNo = 0; iOriginNodeNo < AM::pGrid()->nKnownNodes(); iOriginNodeNo++) {
        iNodeNo = AM::GRID::iGetNthNodeNo(iOriginNodeNo);
        pStream = new Stream(this, m_pStageSpec, i_Stream, tPtnId, iNodeNo); //i_Node);
        vAppend(pStream);
        pStream->vCreateOps();
      }
      break;

    case YG_ORIGIN: // create a stream for origin node only
      RASSERT(m_pStageSpec->bNodeTopo());
      i_Stream = 0;
      tPtnId = C_UNKNOWN;
      iOriginNodeNo = m_pVertex->pPipeline()->iOriginNodeNo();

      pStream = new Stream(this, m_pStageSpec, i_Stream, tPtnId, iOriginNodeNo);
      vAppend(pStream);
      pStream->vCreateOps();
      break;

    case YG_NEXT_NODE: // create a stream for a next node only
      RASSERT(m_pStageSpec->bNodeTopo());

      i_Stream = 0;
      tPtnId = C_UNKNOWN;
      
      for (i8 i_Node = 0; i_Node < AM::pGrid()->nKnownNodes(); i_Node++) {
        iNodeNo = AM::GRID::iGetNthNodeNo(i_Node);
        //if (i_NodeNo == pThisNode->iNodeNo()) {
        pStream = new Stream(this, m_pStageSpec, i_Stream, tPtnId, iNodeNo);
        vAppend(pStream);
        pStream->vCreateOps();
          //break;
        //}
        //break;
      }
      break;

      //                    -- partition topologies --                          //

    case YG_ALL_PARTITIONS: // create a stream for all partitions in grid
      RASSERT(!m_pStageSpec->bNodeTopo());

      for (i8 iPtnNo = 0; iPtnNo < AM::PTNMAP::nPtnsInGrid(); iPtnNo++) {
        tPtnId = iPtnNo;
        iOriginNodeNo = AM::pPtnMap()->iWhichNodeNo(tPtnId);

        i_Stream = AM::pPtnMap()->i_WhichPtn(tPtnId);
        pStream = new Stream(this, m_pStageSpec, i_Stream, tPtnId, iOriginNodeNo);
        vAppend(pStream);
        pStream->vCreateOps();
      }
      break;

    case YG_ALL_PARTITIONS_ON_ORIGIN: // create a stream for all ptns on origin
      RASSERT(!m_pStageSpec->bNodeTopo());

      for (i8 iPtnNo = 0; iPtnNo < AM::PTNMAP::nPtnsInGrid(); iPtnNo++) {
        tPtnId = iPtnNo;
        iOriginNodeNo = AM::pPtnMap()->iWhichNodeNo(tPtnId);

        if (iOriginNodeNo == m_pVertex->pPipeline()->iOriginNodeNo()) {
          i_Stream = AM::pPtnMap()->i_WhichPtn(tPtnId);
          pStream = new Stream(this, m_pStageSpec, i_Stream, tPtnId, iOriginNodeNo);
          vAppend(pStream);
          pStream->vCreateOps();
        }
      }
      break;

    default:
      RASSERT(0);
  }

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
