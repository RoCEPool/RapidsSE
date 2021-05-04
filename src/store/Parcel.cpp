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
               Op*      pSrcOp,
               i8       iDstNode,
               tPN      tPtnNo)
: Object(CN_PARCEL)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARCEL,_FOOTER_);

  m_nObjects = 0;
  for (i8 i_Obj = 0; i_Obj < C_MAX_PARCEL_OBJ; i_Obj++)
    m_pObjects[i_Obj] = nullptr;

  vSetOperand(pOperand);
  m_iPipelineJobId = pSrcOp->pPipelineJob()->iJobId();
  m_eCrossTopo     = pSrcOp->eCrossTopo();
  
  m_iDstNode       = iDstNode;
  m_tPtnNo         = tPtnNo;
  m_iNextStage     = C_UNKNOWN;
  m_iNextOp        = C_UNKNOWN;
  
  _checkInitialised_(CN_PARCEL);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_PARCEL,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Parcel::Parcel(Operand* pOperand,
               i8 iPipelineJobId,
               i8 iDstNode,
               ePX eCrossTopo,
               i8 iNextStage,
               i8 iNextOp,
               tPN tPtnNo)
: Object(CN_PARCEL)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARCEL,_FOOTER_);
  
  m_iPipelineJobId = iPipelineJobId;
  m_iDstNode       = iDstNode;
  m_tPtnNo         = tPtnNo;
  
  m_eCrossTopo     = eCrossTopo;
  m_iNextStage     = iNextStage;
  m_iNextOp        = iNextOp;
  
  m_nObjects = 0;
  for (i8 i_Obj = 0; i_Obj < C_MAX_PARCEL_OBJ; i_Obj++)
    m_pObjects[i_Obj] = nullptr;
  
  vSetOperand(pOperand);

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
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* send out operand as parcel                                                 */
/*----------------------------------------------------------------------------*/
void Parcel::vSendOut(Parcel* pParcel, Operand* pOperand, Op* pSrcOp)
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
      vSendToOrigin(pParcel,pOperand,pSrcOp);
      break;
    }
      
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    case PX_ORIGIN_TO_ALL_PARTITIONS:
    case PX_ALL_NODES_TO_ALL_PARTITIONS:
    case PX_ALL_PARTITIONS_ON_ORIGIN_TO_ALL_PARTITIONS: {
      vSendToAllPartitions(pParcel,pOperand,pSrcOp,eSendKind);
      break;
    }
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    case PX_ORIGIN_TO_ALL_PARTITIONS_ON_ORIGIN: {
      vSendToAllOriginPartitions(pParcel,pOperand,pSrcOp,eSendKind);
      break;
    }
     
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    case PX_ORIGIN_TO_SPECIFIC_PARTITION: {
      Stream* pStream = pNextStage->pGetPartitionStream(tPtn);
      tPN tPtnNo = pStream->tPtnNo();
      vSendToPartition(pParcel,pOperand,pSrcOp,eSendKind,tPtnNo);
      break;
    }
      
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    case PX_ALL_PARTITIONS_TO_ALL_NODES: {
      i8 iDstNode = AM::pPtnMap()->i_WhichNode(tPtn);
      vSendToNode(pParcel,pOperand,pSrcOp,iDstNode);
      break;
    }
      
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    case PX_ALL_PARTITIONS_TO_ALL_PARTITIONS: {
      if (pOperand->bIsBuffer()) {
        vSendToPartition(pParcel,pOperand,pSrcOp,eSendKind,tPtn);
      }
      else {
        vSendToAllPartitions(pParcel,pOperand,pSrcOp,eSendKind);
      }
      break;
    }
     
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    case PX_ORIGIN_TO_ALL_NODES: {
      vSendToAllNodes(pParcel,pOperand,pSrcOp);
      break;
    }
      
    default:
      RASSERT(0);
  }

  
  // delete the operand
  AM::vDeleteOperand(pOperand);
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
/* send the parcel                                                            */
/*----------------------------------------------------------------------------*/
void Parcel::vSendAsParcel(Parcel* pParcel, Op* pSrcOp)
{
  _static_

  
//  pParcel->m_iDstNode       = C_UNKNOWN;
//  pParcel->m_tPtnNo         = C_UNKNOWN;
//  pParcel->m_eCrossTopo     = pSrcOp->eCrossTopo();
//  pParcel->m_iNextStage     = C_UNKNOWN;
//  pParcel->m_iNextOp        = C_UNKNOWN;

  // - - - - - - - - - - - - - - - -

  pParcel->m_iNextStage = pSrcOp->iNextStage();
  pParcel->m_iNextOp    = pSrcOp->iNextOp();
  AM::vSendParcel(pParcel);

  // - - - - - - - - - - - - - - - -
  
  i8 i_OtherStage = pSrcOp->iOtherStage();
  i8 i_OtherOp    = pSrcOp->iOtherOp();
  
  if (C_UNKNOWN != i_OtherStage) {

    pParcel->m_iNextStage = i_OtherStage;
    pParcel->m_iNextOp    = i_OtherOp;

    AM::vSendParcel(pParcel);
  }
  
  // - - - - - - - - - - - - - - - -

  delete pParcel;
}

/*----------------------------------------------------------------------------*/
/* send the operand as a parcel                                               */
/*----------------------------------------------------------------------------*/
void Parcel::vSendAsParcel(Parcel* ,
                           Operand* pOperand,
                           Op*      pSrcOp,
                           i8       iDstNode,
                           tPN      tPtnNo)
{
  _static_
  
  Parcel* pParcel = new Parcel(pOperand,pSrcOp,iDstNode,tPtnNo);

  pParcel->m_iDstNode = iDstNode;
  pParcel->m_tPtnNo   = tPtnNo;

//  Job* pPipelineJob = pSrcOp->pPipelineJob();
//  ePX eCrossTopo    = pSrcOp->eCrossTopo();
//  i8 i_NextStage    = pSrcOp->iNextStage();
//  i8 i_NextOp       = pSrcOp->iNextOp();

  // - - - - - - - - - - - - - - - -
  
//  Parcel* pParcel = new Parcel(pOperand,
//                               pPipelineJob->iJobId(),iDstNode,eCrossTopo,
//                               i_NextStage,i_NextOp,tPtnNo);
  
  Parcel::vSendAsParcel(pParcel,pSrcOp);

}
/*----------------------------------------------------------------------------*/
//void Parcel::vSendAsParcel(Operand* pOperand,
//                           Op*      pSrcOp,
//                           i8       iDstNode,
//                           tPN      tPtnNo)
//{
//  _static_
//
//  Job* pPipelineJob = pSrcOp->pPipelineJob();
//  ePX eCrossTopo    = pSrcOp->eCrossTopo();
//  i8 i_NextStage    = pSrcOp->iNextStage();
//  i8 i_NextOp       = pSrcOp->iNextOp();
//  i8 i_OtherStage   = pSrcOp->iOtherStage();
//  i8 i_OtherOp      = pSrcOp->iOtherOp();
//
//  // - - - - - - - - - - - - - - - -
//
//  Parcel* pParcel = new Parcel(pOperand,
//                               pPipelineJob->iJobId(),iDstNode,eCrossTopo,
//                               i_NextStage,i_NextOp,tPtnNo);
//  AM::vSendParcel(pParcel);
//
//  // - - - - - - - - - - - - - - - -
//
//  if (C_UNKNOWN != i_OtherStage) {
//
//    pParcel->m_iNextStage = i_OtherStage;
//    pParcel->m_iNextOp    = i_OtherOp;
//
//    AM::vSendParcel(pParcel);
//  }
//
//  // - - - - - - - - - - - - - - - -
//
//  delete pParcel;
//}
/*----------------------------------------------------------------------------*/
//void Parcel::vSendAsParcel(Operand* pOperand,
//                           Op*      pSrcOp,
//                           i8       iDstNode,
//                           tPN      tPtnNo)
//{
//  _static_
//
//  Job* pPipelineJob = pSrcOp->pPipelineJob();
//  ePX eCrossTopo    = pSrcOp->eCrossTopo();
//  i8 i_NextStage    = pSrcOp->iNextStage();
//  i8 i_NextOp       = pSrcOp->iNextOp();
//  i8 i_OtherStage   = pSrcOp->iOtherStage();
//  i8 i_OtherOp      = pSrcOp->iOtherOp();
//
//  Parcel* pParcel = new Parcel(pOperand,
//                               pPipelineJob->iJobId(),iDstNode,eCrossTopo,
//                               i_NextStage,i_NextOp,tPtnNo);
//  AM::vSendParcel(pParcel);
//  delete pParcel;
//
//  if (C_UNKNOWN != i_OtherStage) {
//
//    pParcel = new Parcel(pOperand,pPipelineJob->iJobId(),iDstNode,eCrossTopo,
//                         i_OtherStage,i_OtherOp,tPtnNo);
//    AM::vSendParcel(pParcel);
//    delete pParcel;
//  }
//}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* send the operand                                                           */
/*----------------------------------------------------------------------------*/
void Parcel::vSendToAllNodes(Parcel* pParcel,
                             Operand* pOperand,
                             Op*      pSrcOp)
{
  _static_
  
  Pipeline* pPipeline = pSrcOp->pPipeline();
  i8 i_NextStage = pSrcOp->iNextStage();

  // get the next stage
  Stage* pNextStage = pPipeline->pGet(i_NextStage);
  RASSERT(pNextStage);
    
  for (i8 iDstNode = 0; iDstNode < pNextStage->nItems(); iDstNode++)
    vSendAsParcel(pParcel,pOperand,pSrcOp,iDstNode,C_UNKNOWN);
}

/*----------------------------------------------------------------------------*/
/* send the operand                                                           */
/*----------------------------------------------------------------------------*/
void Parcel::vSendToAllOriginPartitions(Parcel* pParcel,
                                        Operand* pOperand,
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
        vSendAsParcel(pParcel,pOperand,pSrcOp,iDstNode,tPtnNo);
  
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
          vSendAsParcel(pParcel,pOperand,pSrcOp,iDstNode,tPtnNo);
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
void Parcel::vSendToAllPartitions(Parcel* pParcel,
                                  Operand* pOperand,
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

      vSendAsParcel(pParcel,pOperand,pSrcOp,iDstNode,tPtnNo);
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
      
      vSendAsParcel(pParcel,pOperand,pSrcOp,iDstNode,tPtnNo);
      break;
    }
      
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    case XK_NORMAL: {
      
      ASSERT(pNextStage->nItems() == AM::nPtnsInGrid());
      for (tPtnNo = 0; tPtnNo < pNextStage->nItems(); tPtnNo++) {
        
        // sendNode is the partition node
        i8 iDstNode = AM::pPtnMap()->i_WhichNode(tPtnNo);
        pSrcOp->vSendNode(iDstNode);
        vSendAsParcel(pParcel,pOperand,pSrcOp,iDstNode,tPtnNo);
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

      vSendAsParcel(pParcel,pOperand,pSrcOp,iDstNode,tPtnNo);
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
  
  vSendAsParcel(pParcel,pOperand,pSrcOp,iDstNode,C_UNKNOWN);
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
  
  vSendAsParcel(pParcel,pOperand,pSrcOp,iDstNode,C_UNKNOWN);
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

  vSendAsParcel(pParcel,pOperand,pSrcOp,iDstNode,tSendPtn);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
