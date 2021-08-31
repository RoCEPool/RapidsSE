/*============================================================================*/
/* Stage.cpp -- Stage of Pipeline                                             */
/*============================================================================*/

#include "BASE.h"

#include "ePX.h"

#include "Marker.hpp"

#include "PtnMap.hpp"

#include "Grid.hpp"

#include "Op.hpp"
#include "Stream.hpp"
#include "Stage.hpp"
#include "Pipeline.hpp"
#include "PipelineSpec.hpp"
#include "StageSpec.hpp"
#include "SP_Stage.hpp"

extern AttachMap gxAttachMap;

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,Stage,CN_STAGE)
__aNewFunction(Stage,CN_STAGE)
__vDeleteFunction(Stage,CN_STAGE)
__aFooterFunction(Stage)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Stage::Stage(StageSpec* pStageSpec, Pipeline* pPipeline)
: DeleteHelper_t<Stage>(0) // typeid(Stage))
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STAGE,_FOOTER_);
  
  m_sPipelineNm = pPipeline->szPipelineNm();
  m_sStageNm    = pStageSpec->szStageNm();
  m_i_Stage     = pStageSpec->i_StageSpec();
  
  m_pStageSpec  = pStageSpec;
  m_pPipeline   = pPipeline;

  _checkInitialised_(CN_STAGE);

  vCreateStreams();
  
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
  Stream*  pStream = pGet(0);
  StageSpec* pStageSpec = pStream->pStageSpec();
  
  if (pStageSpec->bNodeTopo() &&
      pStream->i_Node() == AM::pGrid()->iThisNodeNo()) {
    
    // get first operator of this first stage
    Op* pOp = pStream->pGet(0);
    RASSERT(pOp->szOpNm()->bSame("start"_Stz));
    // append a CANCELMARKER onto first operator
    pOp->vAppendLeft(new Marker(OM_CANCELMARKER),pTriggeringTask);
  }
}

/*----------------------------------------------------------------------------*/
/* get stream for given partition                                             */
/*----------------------------------------------------------------------------*/
Stream* Stage::pGetPartitionStream(i8 i_Ptn)
{
  _precondition_(_MAGIC_,CN_STAGE,_FOOTER_);
  
  Stream* pStream   = nullptr;
  StageSpec* pStageSpec = nullptr;
  
  for (i8 i_Stream = 0; i_Stream < nItems(); i_Stream++) {
    pStream  = pGet(i_Stream);
    pStageSpec = pStream->pStageSpec();
    
    if ( ! pStageSpec->bNodeTopo() &&
        pStream->tPtnNo() == i_Ptn)
      return pStream;
  }
  
  return pStream;
}

/*----------------------------------------------------------------------------*/
/* get stream for given node                                                  */
/*----------------------------------------------------------------------------*/
Stream* Stage::pGetNodeStream(i8 i_Node)
{
  _precondition_(_MAGIC_,CN_STAGE,_FOOTER_);
  
  Stream* pStream   = nullptr;
  StageSpec* pStageSpec = nullptr;
  
  for (i8 iNdeVec = 0; iNdeVec < nItems(); iNdeVec++) {
    pStream  = pGet(iNdeVec);
    pStageSpec = pStream->pStageSpec();
    
    if (pStageSpec->bNodeTopo() &&
        pStream->i_Node() == i_Node)
      return pStream;
  }
  
  return pStream;
}

/*----------------------------------------------------------------------------*/
/* get ops to be executed on given node                                       */
/*----------------------------------------------------------------------------*/
i8 Stage::nOpsOnNode(i8 iNode)
{
  _precondition_(_MAGIC_,CN_STAGE,_FOOTER_);

  i8 nOps = 0;

  for (i8 i_Node = 0; i_Node < nItems(); i_Node++) {
    Stream* pStream = pGet(i_Node);

    if (m_pStageSpec->bNodeTopo()) {
      // skip if Op not for this node
      if (pStream->i_Node() == iNode) {
        nOps += pStream->nItems();
      }
    }
    else {
      i8 iNode_exe = AM::pPtnMap()->iWhichNodeNo(pStream->tPtnNo());
      if (iNode_exe == iNode) {
        nOps += pStream->nItems();
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
Op* Stage::pPipeOutOp(ePX eCrossTopo, tPN tDstPtnNo, i8 i_DstOp)
{
  Op* pOp = nullptr;
  Stream* pNdeStream = nullptr;
  Stream* pPtnStream = nullptr;
  
  i8 i_ThisNode = AM::iThisNodeNo();
  
  switch (eCrossTopo) {
      
    case PX_ALL_NODES_TO_ORIGIN:
    case PX_ALL_NODES_TO_ALL_NODES:
    case PX_ALL_PARTITIONS_TO_ORIGIN:
    case PX_ORIGIN_TO_ALL_NODES:
    case PX_ORIGIN_TO_ORIGIN:
    case PX_SPECIFIC_PARTITION_TO_ORIGIN:
      pNdeStream = this->pGetNodeStream(i_ThisNode);
      pOp = pNdeStream->pGet(i_DstOp);
      break;
      
    case PX_ORIGIN_TO_ALL_PARTITIONS:
      pPtnStream = pGet(tDstPtnNo);
      pOp = pPtnStream->pGet(i_DstOp);
      break;
      
    case PX_ALL_NODES_TO_ALL_PARTITIONS:
    case PX_ORIGIN_TO_ALL_PARTITIONS_ON_ORIGIN:
    case PX_ORIGIN_TO_SPECIFIC_PARTITION:
      pPtnStream = pGetPartitionStream(tDstPtnNo);
      pOp = pPtnStream->pGet(i_DstOp);
      break;
      
    case PX_ALL_PARTITIONS_TO_ALL_NODES:
      pNdeStream = this->pGetNodeStream(i_ThisNode);
      pOp = pNdeStream->pGet(i_DstOp);
      break;
      
    case PX_ALL_PARTITIONS_TO_ALL_PARTITIONS:
      pPtnStream = pGetPartitionStream(tDstPtnNo);
      pOp = pPtnStream->pGet(i_DstOp);
      break;
      
    case PX_ALL_PARTITIONS_ON_ORIGIN_TO_ALL_PARTITIONS:
      pPtnStream = pGetPartitionStream(tDstPtnNo);
      pOp = pPtnStream->pGet(i_DstOp);
      break;
      
    case PX_SPECIFIC_PARTITION_TO_SPECIFIC_PARTITION:
      pPtnStream = pGet(0);
      ASSERT(pPtnStream->tPtnNo() == tDstPtnNo);
      pOp = pPtnStream->pGet(i_DstOp);
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
/* test if to send startMarker to start op of the stage                       */
/*----------------------------------------------------------------------------*/
bool Stage::bStartPipeline(void)
{
  _precondition_(_MAGIC_,CN_STAGE,_FOOTER_);
  
  // would the start marker always be on ORIGIN???
  
  // get first stage from stageNdeVec
  Stream*  pStream = pGet(0);
  StageSpec* pStageSpec = pStream->pStageSpec();
  
  if (pStageSpec->bNodeTopo() &&
      pStream->i_Node() == AM::pGrid()->iThisNodeNo()) {
    
    // get first operator of this first stage
    Op* pOp = pStream->pGet(0);
    if (! pOp->szOpNm()->bSame("eatAll"_Stz) &&
        ! pOp->szOpNm()->bSame("statWrite"_Stz)) {

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
/* create the streams for this stage based on stage topology                  */
/*----------------------------------------------------------------------------*/
void Stage::vCreateStreams(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STAGE,_FOOTER_);
  
  tPN tPtnNo;
  i8 i_Node;
  i8 i_Stream;
  Stream* pStream;
  i8 i_NodeNo;
  
  switch (m_pStageSpec->eTopology()) {
            
    //                      -- node topologies --                             //
      
    case PO_ALL_NODES: // create a stream for all nodes
      RASSERT(m_pStageSpec->bNodeTopo());
      i_Stream = 0;
      tPtnNo = C_UNKNOWN;
      for (i_Node = 0; i_Node < AM::pGrid()->nKnownNodes(); i_Node++) {
        i_NodeNo = AM::GRID::iGetNthNodeNo(i_Node);
        pStream = new Stream(m_pStageSpec,this,i_Stream,tPtnNo,i_NodeNo); //i_Node);
        vAdd(pStream);
      }
      break;
      
    case PO_ORIGIN: // create a stream for origin node only
      RASSERT(m_pStageSpec->bNodeTopo());
      i_Stream = 0;
      tPtnNo = C_UNKNOWN;
      i_Node = m_pPipeline->i_Origin();
      //if (i_Node < 100) {
      //  printf("BAD node\n");
      //  i_Node = AM::iGetNthNodeNo(i_Node);
      //}
      pStream = new Stream(m_pStageSpec,this,i_Stream,tPtnNo,i_Node);
      vAdd(pStream);
      break;
        
    case PO_SPECIFIC_NODE: // create a stream for a specific node only
      RASSERT(m_pStageSpec->bNodeTopo());
      RASSERT(0);
      break;
        
    //                    -- partition topologies --                          //

    case PO_ALL_PARTITIONS: // create a stream for all partitions in grid
      RASSERT(! m_pStageSpec->bNodeTopo());
      for (tPtnNo = 0; tPtnNo < AM::PTNMAP::nPtnsInGrid(); tPtnNo++) {
        i_Node = AM::pPtnMap()->iWhichNodeNo(tPtnNo);
        //if (i_Node < 100) printf("1 BAD node\n");
        i_Stream = AM::pPtnMap()->i_WhichPtn(tPtnNo);
        pStream = new Stream(m_pStageSpec,this,i_Stream,tPtnNo,i_Node);
        vAdd(pStream);
      }
      break;
      
    case PO_ALL_PARTITIONS_ON_ORIGIN: // create a stream for all ptns on origin
      RASSERT(! m_pStageSpec->bNodeTopo());
      for (tPtnNo = 0; tPtnNo < AM::PTNMAP::nPtnsInGrid(); tPtnNo++) {
        i_Node = AM::pPtnMap()->iWhichNodeNo(tPtnNo);
        //if (i_Node < 100) printf("2 BAD node\n");
        if (i_Node == m_pPipeline->i_Origin()) {
          i_Stream = AM::pPtnMap()->i_WhichPtn(tPtnNo);
          pStream = new Stream(m_pStageSpec,this,i_Stream,tPtnNo,i_Node);
          vAdd(pStream);
        }
      }
      break;
      
    case PO_SPECIFIC_PARTITION: // create a stream for a specific partition
      RASSERT(! m_pStageSpec->bNodeTopo());
      tPtnNo = m_pStageSpec->tSpecificPtnNo();
      i_Node = AM::pPtnMap()->iWhichNodeNo(tPtnNo);
      //if (i_Node < 100) printf("3 BAD node\n");
      i_Stream = AM::pPtnMap()->i_WhichPtn(tPtnNo);
      pStream = new Stream(m_pStageSpec,this,i_Stream,tPtnNo,i_Node);
      vAdd(pStream);
      break;
      
    default:
      RASSERT(0);
  }
    
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
