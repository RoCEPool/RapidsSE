/*============================================================================*/
/* Parcel.cpp -- Send Parcel                                                  */
/*============================================================================*/

#include "BASE.h"

#include "ePX.h"

#include "Job.hpp"
#include "Op.hpp"
#include "OpSpec.hpp"
#include "Parcel.hpp"
#include "Pipeline.hpp"
#include "Stage.hpp"
#include "Stream.hpp"
#include "PtnMap.hpp"
#include "Grid.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,Parcel,CN_PARCEL)
__aNewFunction(Parcel,CN_PARCEL)
__vDeleteFunction(Parcel,CN_PARCEL)
__aFooterFunction(Parcel)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Parcel::Parcel(Operand* pOperand,
               i8       iPipelineJobId,
               i8       iDstNode,
               tPN      tPtnNo,
               ePX      eCrossTopo)
: Object(CN_PARCEL)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARCEL,_FOOTER_);

  m_nObjects = 0;
  for (i8 i_Obj = 0; i_Obj < C_MAX_PARCEL_OBJ; i_Obj++)
    m_pObjects[i_Obj] = nullptr;

  vSetOperand(pOperand);
  m_iPipelineJobId = iPipelineJobId;
  m_eCrossTopo     = eCrossTopo;
  m_iDstNode       = iDstNode;
  m_tPtnNo         = tPtnNo;
  
  m_iNextStage     = C_UNKNOWN;
  m_iNextOp        = C_UNKNOWN;
  
  _checkInitialised_(CN_PARCEL);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_PARCEL,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Parcel::~Parcel(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARCEL,_FOOTER_);
  
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Add object to array                                                        */
/*----------------------------------------------------------------------------*/
void Parcel::vAddObject(Object *pObj)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARCEL,_FOOTER_);

  if (m_nObjects < C_MAX_PARCEL_OBJ) {
    m_pObjects[m_nObjects++] = pObj;
  }
}

/*----------------------------------------------------------------------------*/
/* Get an object from array                                                   */
/*----------------------------------------------------------------------------*/
Object* Parcel::pGetObject(i8 i_Object)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARCEL,_FOOTER_);

  if (i_Object < C_MAX_PARCEL_OBJ && i_Object < m_nObjects)
    return m_pObjects[i_Object];
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* Get Operand                                                                */
/*----------------------------------------------------------------------------*/
Operand* Parcel::pGetOperand(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARCEL,_FOOTER_);

  return (Operand*) m_pObjects[0];
}

/*----------------------------------------------------------------------------*/
/* send out operand as parcel                                                 */
/*----------------------------------------------------------------------------*/
void Parcel::vSendOut(Operand* pOperand, Op* pSrcOp)
{
  _static_
  
  Pipeline* pPipeline = pSrcOp->pPipeline();
  i8 i_NextStage = pSrcOp->iNextStage();

  // get the next stage
  Stage* pNextStage = pPipeline->pGet(i_NextStage);
  RASSERT(pNextStage);
  
  // figure out the send kind
  eXK eSendKind = pSrcOp->eSendKind();
  
  // is the operand is a marker or a streamDef ?
  if (pOperand->bIsMarker() || pOperand->bIsStreamDef())
    eSendKind = XK_NORMAL;
  
  // is the operand an InfoSet ?
  if (pOperand->bIsInfoSet() || pOperand->bIsProfile())
    eSendKind = XK_ROBIN;
  
  ePX eCrossTopo = pSrcOp->eCrossTopo();      // cross topology
  tPN tPtn = pSrcOp->tPtnNo();
  
  switch (eCrossTopo) {
      
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    case PX_ALL_NODES_TO_ORIGIN:
    case PX_ALL_PARTITIONS_TO_ORIGIN:
    case PX_ORIGIN_TO_ORIGIN:
    case PX_SPECIFIC_PARTITION_TO_ORIGIN: {
      vSendToOrigin(this,pOperand,pSrcOp);
      break;
    }
      
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    case PX_ORIGIN_TO_ALL_PARTITIONS:
    case PX_ALL_NODES_TO_ALL_PARTITIONS:
    case PX_ALL_PARTITIONS_ON_ORIGIN_TO_ALL_PARTITIONS: {
      vSendToAllPartitions(pOperand,pSrcOp,eSendKind);
      break;
    }
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    case PX_ORIGIN_TO_ALL_PARTITIONS_ON_ORIGIN: {
      vSendToAllOriginPartitions(pOperand,pSrcOp,eSendKind);
      break;
    }
     
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    case PX_ORIGIN_TO_SPECIFIC_PARTITION: {
      Stream* pStream = pNextStage->pGetPartitionStream(tPtn);
      tPN tPtnNo = pStream->tPtnNo();
      vSendToPartition(this,pOperand,pSrcOp,eSendKind,tPtnNo);
      break;
    }
      
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    case PX_ALL_PARTITIONS_TO_ALL_NODES: {
      i8 iDstNode = AM::pPtnMap()->i_WhichNode(tPtn);
      vSendToNode(this,pOperand,pSrcOp,iDstNode);
      break;
    }
      
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    case PX_ALL_PARTITIONS_TO_ALL_PARTITIONS: {
      if (pOperand->bIsBuffer()) {
        vSendToPartition(this,pOperand,pSrcOp,eSendKind,tPtn);
      }
      else {
        vSendToAllPartitions(pOperand,pSrcOp,eSendKind);
      }
      break;
    }
     
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    case PX_ORIGIN_TO_ALL_NODES: {
      vSendToAllNodes(pOperand,pSrcOp);
      break;
    }
      
    default:
      RASSERT(0);
  }

  
  // delete the operand
  AM::vDeleteOperand(pOperand);
}

/*----------------------------------------------------------------------------*/
/* Set Operand                                                                */
/*----------------------------------------------------------------------------*/
void Parcel::vSetOperand(Operand *pOperand)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARCEL,_FOOTER_);

  m_pObjects[0] = pOperand;
}


/*----------------------------------------------------------------------------*/
/*-- Private Static Functions ------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* send the operand as a parcel                                               */
/*----------------------------------------------------------------------------*/
void Parcel::vSendAsParcel(Operand* pOperand,
                           Op*      pSrcOp,
                           i8       iDstNode,
                           tPN      tPtnNo)
{
  _static_
    
  m_iDstNode   = iDstNode;
  m_tPtnNo     = tPtnNo;
  
  m_iNextStage = pSrcOp->iNextStage();
  m_iNextOp    = pSrcOp->iNextOp();
  AM::vSendParcel(this);

  // - - - - - - - - - - - - - - - -
  
  i8 i_OtherStage = pSrcOp->iOtherStage();
  i8 i_OtherOp    = pSrcOp->iOtherOp();
  
  if (C_UNKNOWN != i_OtherStage) {

    m_iNextStage = i_OtherStage;
    m_iNextOp    = i_OtherOp;

    AM::vSendParcel(this);
  }
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* send the operand                                                           */
/*----------------------------------------------------------------------------*/
void Parcel::vSendToAllNodes(Operand* pOperand,
                             Op*      pSrcOp)
{
  _static_
  
  Pipeline* pPipeline = pSrcOp->pPipeline();
  i8 i_NextStage = pSrcOp->iNextStage();

  // get the next stage
  Stage* pNextStage = pPipeline->pGet(i_NextStage);
  RASSERT(pNextStage);
    
  for (i8 iDstNode = 0; iDstNode < pNextStage->nItems(); iDstNode++)
    vSendAsParcel(pOperand,pSrcOp,iDstNode,C_UNKNOWN);
}

/*----------------------------------------------------------------------------*/
/* send the operand                                                           */
/*----------------------------------------------------------------------------*/
void Parcel::vSendToAllOriginPartitions(Operand* pOperand,
                                        Op*      pSrcOp,
                                        eXK      eSendKind)
{
  _static_
  
  tPN tPtnNo;
  i8 i_TablePtn;
  
  Pipeline* pPipeline = pSrcOp->pPipeline();
  i8 i_NextStage = pSrcOp->iNextStage();

  
  // get the next stage
  Stage* pNextStage = pPipeline->pGet(i_NextStage);
  RASSERT(pNextStage);
    
  // check the SendKind
  switch (eSendKind) {
      
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    case XK_ROBIN : {
      i8 i_Robin = pSrcOp->iRobin();
      i_Robin++;
      pSrcOp->vRobin(i_Robin);
      i_TablePtn = i_Robin % AM::nPtnsPerTable();
      tPtnNo = PtnMap::tStreamToLglPtnNo(i_TablePtn);
      
      // sendNode is the partition node
      i8 iDstNode = AM::pPtnMap()->i_WhichNode(tPtnNo);
      ASSERT(iDstNode == pPipeline->i_Origin());
      pSrcOp->vSendNode(iDstNode);

      if (iDstNode == pPipeline->i_Origin())
        vSendAsParcel(pOperand,pSrcOp,iDstNode,tPtnNo);
  
      break;
    }
      
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    case XK_NORMAL: {
      
      ASSERT(pNextStage->nItems() == AM::nPtnsPerTable());
      for (i8 i_Stream = 0; i_Stream < pNextStage->nItems(); i_Stream++) {
        
        Stream* pStream = pNextStage->pGet(i_Stream);
        tPtnNo = pStream->tPtnNo();
        
        // sendNode is the partition node
    
        i8 iDstNode = AM::pPtnMap()->i_WhichNode(tPtnNo);
        pSrcOp->vSendNode(iDstNode);
        if ( iDstNode == pPipeline->i_Origin())
          vSendAsParcel(pOperand,pSrcOp,iDstNode,tPtnNo);
      }
    }
    break;
      
    default:
      ASSERT(0);
  }
}

/*----------------------------------------------------------------------------*/
/* send the operand                                                           */
/*----------------------------------------------------------------------------*/
void Parcel::vSendToAllPartitions(Operand* pOperand,
                                  Op*      pSrcOp,
                                  eXK      eSendKind)
{
  _static_
  
  tPN tPtnNo;
  i8 i_TablePtn;
  
  Pipeline* pPipeline = pSrcOp->pPipeline();
  i8 i_NextStage = pSrcOp->iNextStage();
  
  // get the next stage
  Stage* pNextStage = pPipeline->pGet(i_NextStage);
  RASSERT(pNextStage);
    
  switch (eSendKind) {
      
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    case XK_HASHED : {
      DataBuffer* pDataBuffer = _pDataBuffer(pOperand);
      tPtnNo = pDataBuffer->tPtnNo();
      
      // sendNode is the partition node
      i8 iDstNode = AM::pPtnMap()->i_WhichNode(tPtnNo);
      pSrcOp->vSendNode(iDstNode);

      vSendAsParcel(pOperand,pSrcOp,iDstNode,tPtnNo);
      break;
    }
      
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    case XK_LOCALROBIN : {
      i8 i_LocalRobin = pSrcOp->iLocalRobin();
      i_LocalRobin++;
      pSrcOp->vLocalRobin(i_LocalRobin);

      i_TablePtn = i_LocalRobin % AM::nPtnsPerTable();
      tPtnNo = PtnMap::tStreamToLglPtnNo(i_TablePtn);
      
      // sendNode is this node
      i8 iDstNode = AM::pPtnMap()->i_WhichNode(tPtnNo);
      pSrcOp->vSendNode(iDstNode);
      RASSERT(iDstNode == AM::pGrid()->iThisNode());
      
      vSendAsParcel(pOperand,pSrcOp,iDstNode,tPtnNo);
      break;
    }
      
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    case XK_NORMAL: {
      
      ASSERT(pNextStage->nItems() == AM::nPtnsInGrid());
      for (tPtnNo = 0; tPtnNo < pNextStage->nItems(); tPtnNo++) {
        
        // sendNode is the partition node
        i8 iDstNode = AM::pPtnMap()->i_WhichNode(tPtnNo);
        pSrcOp->vSendNode(iDstNode);
        vSendAsParcel(pOperand,pSrcOp,iDstNode,tPtnNo);
      }
      break;
    }
      
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    case XK_ROBIN : {
      i8 i_Robin = pSrcOp->iRobin();
      i_Robin++;
      pSrcOp->vRobin(i_Robin);
      
      tPtnNo = i_Robin % AM::nPtnsInGrid();
      
      // sendNode is the partition node
      i8 iDstNode = AM::pPtnMap()->i_WhichNode(tPtnNo);
      pSrcOp->vSendNode(iDstNode);

      vSendAsParcel(pOperand,pSrcOp,iDstNode,tPtnNo);
      break;
    }
      
    default:
      ASSERT(0);
  }
}

/*----------------------------------------------------------------------------*/
/* send the operand to a specific node                                        */
/*----------------------------------------------------------------------------*/
void Parcel::vSendToNode(Parcel* pParcel,
                         Operand* pOperand,
                         Op*      pSrcOp,
                         i8       iDstNode)
{
  _static_
    
  Pipeline* pPipeline = pSrcOp->pPipeline();
  i8 i_NextStage = pSrcOp->iNextStage();
  
  // get the next stage
  Stage* pNextStage = pPipeline->pGet(i_NextStage);
  RASSERT(pNextStage);
  
  ASSERT(pNextStage->nItems() == AM::pGrid()->nConnectedNodes());
  
  pParcel->vSendAsParcel(pOperand,pSrcOp,iDstNode,C_UNKNOWN);
}

/*----------------------------------------------------------------------------*/
/* send the operand to the origin                                             */
/*----------------------------------------------------------------------------*/
void Parcel::vSendToOrigin(Parcel* pParcel,
                           Operand* pOperand,
                           Op*      pSrcOp)
{
  _static_
 
  Pipeline* pPipeline = pSrcOp->pPipeline();

  i8 iDstNode = pPipeline->i_Origin();
  pSrcOp->vSendNode(iDstNode);
  
  pParcel->vSendAsParcel(pOperand,pSrcOp,iDstNode,C_UNKNOWN);
}

/*----------------------------------------------------------------------------*/
/* send the operand to a partition                                            */
/*----------------------------------------------------------------------------*/
void Parcel::vSendToPartition(Parcel* pParcel,
                              Operand* pOperand,
                              Op*      pSrcOp,
                              eXK      eSendKind,
                              tPN      tPtnNo)
{
  _static_
  
  i8 i_Robin;
  
  tPN tSendPtn = C_UNKNOWN;
    
  DataBuffer* pDataBuffer = nullptr;
  
  switch (eSendKind) {
      
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    case XK_HASHED :
      pDataBuffer = _pDataBuffer(pOperand);
      tSendPtn = pDataBuffer->tPtnNo();
      break;
      
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    case XK_NORMAL:
      tSendPtn = tPtnNo;
      break;
      
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    case XK_ROBIN:
      i_Robin = pSrcOp->iRobin();
      i_Robin++;
      pSrcOp->vRobin(i_Robin);
      tSendPtn = i_Robin % AM::nPtnsInGrid();
      break;
      
    default:
      ASSERT(0);
  }
  
  i8 iDstNode = AM::pPtnMap()->i_WhichNode(tSendPtn);
  pSrcOp->vSendNode(iDstNode);

  pParcel->vSendAsParcel(pOperand,pSrcOp,iDstNode,tSendPtn);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
