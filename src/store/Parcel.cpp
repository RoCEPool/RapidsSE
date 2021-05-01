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
               i8 iParentJobId,
               ePX eCrossTopo,
               i8 iNextStage,
               i8 iNextOp,
               i8 i_Node,
               tPN tPtnNo)
: Object(CN_PARCEL)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARCEL,_FOOTER_);
  
  m_iPipelineJobId = iParentJobId;
  m_i_Node         = i_Node;
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
/* send parcel                                                                */
/*----------------------------------------------------------------------------*/
void Parcel::vSendOut(Operand* pOperand,
                      Op*      pSrcOp,
                      Task*    pTriggeringTask)
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
      vSendToOrigin(pOperand,pSrcOp,pTriggeringTask,eSendKind);
      break;
    }
      
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    case PX_ORIGIN_TO_ALL_PARTITIONS:
    case PX_ALL_NODES_TO_ALL_PARTITIONS:
    case PX_ALL_PARTITIONS_ON_ORIGIN_TO_ALL_PARTITIONS: {
      vSendToAllPartitions(pOperand,pSrcOp,pTriggeringTask,eSendKind);
      break;
    }
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    case PX_ORIGIN_TO_ALL_PARTITIONS_ON_ORIGIN: {
      vSendToAllOriginPartitions(pOperand,pSrcOp,pTriggeringTask,eSendKind);
      break;
    }
     
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    case PX_ORIGIN_TO_SPECIFIC_PARTITION: {
      Stream* pStream = pNextStage->pGetPartitionStream(tPtn);
      tPN tPtnNo = pStream->tPtnNo();
      vSendToPartition(pOperand,pSrcOp,pTriggeringTask,eSendKind,tPtnNo);
      break;
    }
      
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    case PX_ALL_PARTITIONS_TO_ALL_NODES: {
      i8 i_Node = AM::pPtnMap()->i_WhichNode(tPtn);
      vSendToNode(pOperand,pSrcOp,pTriggeringTask,eSendKind,i_Node);
      break;
    }
      
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    case PX_ALL_PARTITIONS_TO_ALL_PARTITIONS: {
      if (pOperand->bIsBuffer()) {
        vSendToPartition(pOperand,pSrcOp,pTriggeringTask,eSendKind,tPtn);
      }
      else {
        vSendToAllPartitions(pOperand,pSrcOp,pTriggeringTask,eSendKind);
      }
      break;
    }
     
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    case PX_ORIGIN_TO_ALL_NODES: {
      vSendToAllNodes(pOperand,pSrcOp,pTriggeringTask,eSendKind);
      break;
    }
      
    default:
      RASSERT(0);
  }

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
/* send the operand                                                           */
/*----------------------------------------------------------------------------*/
void Parcel::vSendOperand(Operand* pOperand,
                          Op* pSrcOp,
                          i8 i_Node,
                          tPN tPtnNo)
{
  _static_
  
  Job* pPipelineJob = pSrcOp->pPipelineJob();
  ePX eCrossTopo = pSrcOp->eCrossTopo();      // cross topology
  i8 i_NextStage = pSrcOp->iNextStage();
  i8 i_NextOp = pSrcOp->iNextOp();
  i8 i_OtherStage = pSrcOp->iOtherStage();
  i8 i_OtherOp = pSrcOp->iOtherOp();

  Parcel* pParcel = new Parcel(pOperand,pPipelineJob->iJobId(),eCrossTopo,
                               i_NextStage,i_NextOp,i_Node,tPtnNo);
  AM::vSendParcel(pParcel);
  delete pParcel;

  if (i_OtherStage != C_UNKNOWN) {

    pParcel = new Parcel(pOperand,pPipelineJob->iJobId(),eCrossTopo,
                         i_OtherStage,i_OtherOp,i_Node,tPtnNo);
    AM::vSendParcel(pParcel);
    delete pParcel;
  }
}

/*----------------------------------------------------------------------------*/
/* send the operand                                                           */
/*----------------------------------------------------------------------------*/
void Parcel::vSendToAllNodes(Operand* pOperand,
                             Op* pSrcOp,
                             Task* pTriggeringTask,
                             eXK eSendKind)
{
  _static_
  
  UNUSED(pTriggeringTask);
  UNUSED(eSendKind);

  ASSERT(eSendKind == XK_NORMAL || eSendKind == XK_ROBIN);
  
  Pipeline* pPipeline = pSrcOp->pPipeline();
  i8 i_NextStage = pSrcOp->iNextStage();

  // get the next stage
  Stage* pNextStage = pPipeline->pGet(i_NextStage);
  RASSERT(pNextStage);
  
  //ASSERT(pNextStage->nItems() == AM::pGrid()->nBootNodes());
  
  for (i8 i_Node = 0; i_Node < pNextStage->nItems(); i_Node++) {
    vSendOperand(pOperand,pSrcOp,i_Node,C_UNKNOWN);
  }
  
  // delete the operand
  AM::vDeleteOperand(pOperand);
}

/*----------------------------------------------------------------------------*/
/* send the operand                                                           */
/*----------------------------------------------------------------------------*/
void Parcel::vSendToAllOriginPartitions(Operand* pOperand,
                                        Op* pSrcOp,
                                        Task* pTriggeringTask,
                                        eXK eSendKind)
{
  _static_
  
  UNUSED(pTriggeringTask);

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
      i8 i_SendNode = AM::pPtnMap()->i_WhichNode(tPtnNo);
      ASSERT(i_SendNode == pPipeline->i_Origin());
      pSrcOp->vSendNode(i_SendNode);

      if ( i_SendNode == pPipeline->i_Origin()) {
        
        vSendOperand(pOperand, pSrcOp, i_SendNode, tPtnNo);
        AM::vDeleteOperand(pOperand);
        
      }
      
      break;
    }
      
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    case XK_NORMAL: {
      
      ASSERT(pNextStage->nItems() == AM::nPtnsPerTable());
      for (i8 i_Stream = 0; i_Stream < pNextStage->nItems(); i_Stream++) {
        
        Stream* pStream = pNextStage->pGet(i_Stream);
        tPtnNo = pStream->tPtnNo();
        
        // sendNode is the partition node
    
        i8 i_SendNode = AM::pPtnMap()->i_WhichNode(tPtnNo);
        pSrcOp->vSendNode(i_SendNode);
        if ( i_SendNode == pPipeline->i_Origin()) {
          vSendOperand(pOperand, pSrcOp, i_SendNode, tPtnNo);
        }
        
      }
      AM::vDeleteOperand(pOperand);
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
                                  Op* pSrcOp,
                                  Task* pTriggeringTask,
                                  eXK eSendKind)
{
  _static_
  
  UNUSED(pTriggeringTask);

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
      i8 i_SendNode = AM::pPtnMap()->i_WhichNode(tPtnNo);
      pSrcOp->vSendNode(i_SendNode);

      vSendOperand(pOperand, pSrcOp, i_SendNode, tPtnNo);
      AM::vDeleteOperand(pOperand);

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
      i8 i_SendNode = AM::pPtnMap()->i_WhichNode(tPtnNo);
      pSrcOp->vSendNode(i_SendNode);
      RASSERT(i_SendNode == AM::pGrid()->iThisNode());
      
      vSendOperand(pOperand, pSrcOp, i_SendNode, tPtnNo);
      AM::vDeleteOperand(pOperand);
      break;
    }
      
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    case XK_NORMAL: {
      
      ASSERT(pNextStage->nItems() == AM::nPtnsInGrid());
      for (tPtnNo = 0; tPtnNo < pNextStage->nItems(); tPtnNo++) {
        
        // sendNode is the partition node
        i8 i_SendNode = AM::pPtnMap()->i_WhichNode(tPtnNo);
        pSrcOp->vSendNode(i_SendNode);
        vSendOperand(pOperand, pSrcOp, i_SendNode, tPtnNo);
      }
      AM::vDeleteOperand(pOperand);
      break;
    }
      
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    case XK_ROBIN : {
      i8 i_Robin = pSrcOp->iRobin();
      i_Robin++;
      pSrcOp->vRobin(i_Robin);
      
      tPtnNo = i_Robin % AM::nPtnsInGrid();
      
      // sendNode is the partition node
      i8 i_SendNode = AM::pPtnMap()->i_WhichNode(tPtnNo);
      pSrcOp->vSendNode(i_SendNode);

      vSendOperand(pOperand, pSrcOp, i_SendNode, tPtnNo);
      AM::vDeleteOperand(pOperand);
      break;
    }
      
    default:
      ASSERT(0);
  }
}

/*----------------------------------------------------------------------------*/
/* send the operand to a specific node                                        */
/*----------------------------------------------------------------------------*/
void Parcel::vSendToNode(Operand* pOperand,
                         Op* pSrcOp,
                         Task* pTriggeringTask,
                         eXK eSendKind,
                         i8 i_Node)
{
  _static_
  
  UNUSED(pTriggeringTask);
  UNUSED(eSendKind);

  ASSERT(eSendKind == XK_NORMAL || eSendKind == XK_ROBIN);
  
  Pipeline* pPipeline = pSrcOp->pPipeline();
  i8 i_NextStage = pSrcOp->iNextStage();
  
  // get the next stage
  Stage* pNextStage = pPipeline->pGet(i_NextStage);
  RASSERT(pNextStage);
  
  ASSERT(pNextStage->nItems() == AM::pGrid()->nConnectedNodes());
  
  vSendOperand(pOperand,pSrcOp,i_Node,C_UNKNOWN);
  
  // delete the operand
  AM::vDeleteOperand(pOperand);
}

/*----------------------------------------------------------------------------*/
/* send the operand to the origin                                             */
/*----------------------------------------------------------------------------*/
void Parcel::vSendToOrigin(Operand* pOperand,
                           Op* pSrcOp,
                           Task* pTriggeringTask,
                           eXK eSendKind)
{
  _static_
  
  UNUSED(pTriggeringTask);
  UNUSED(eSendKind);

  ASSERT(eSendKind == XK_NORMAL || eSendKind == XK_ROBIN);
  
  Pipeline* pPipeline = pSrcOp->pPipeline();

  i8 i_SendNode = pPipeline->i_Origin();
  pSrcOp->vSendNode(i_SendNode);
  
  vSendOperand(pOperand, pSrcOp, i_SendNode, C_UNKNOWN);
  
  // delete the operand
  AM::vDeleteOperand(pOperand);
}

/*----------------------------------------------------------------------------*/
/* send the operand to a partition                                            */
/*----------------------------------------------------------------------------*/
void Parcel::vSendToPartition(Operand* pOperand,
                              Op* pSrcOp,
                              Task* pTriggeringTask,
                              eXK eSendKind,
                              tPN tPtnNo)
{
  _static_
  
  i8 i_Robin;
  
  UNUSED(pTriggeringTask);

  tPN tSendPtn = C_UNKNOWN;
  
  ASSERT(eSendKind == XK_NORMAL ||
         eSendKind == XK_ROBIN  ||
         eSendKind == XK_HASHED);   // ray added XK_HASHED 26 jul 2020 re q301
  
  switch (eSendKind) {
      
    case XK_HASHED : {
      DataBuffer* pDataBuffer = _pDataBuffer(pOperand);
      tSendPtn = pDataBuffer->tPtnNo();
      break;
    }
      
    case XK_NORMAL:
      tSendPtn = tPtnNo;
      break;
      
    case XK_ROBIN:
      i_Robin = pSrcOp->iRobin();
      i_Robin++;
      pSrcOp->vRobin(i_Robin);
      tSendPtn = i_Robin % AM::nPtnsInGrid();
      break;
      
    default:
      ASSERT(0);
  }
  
  i8 i_SendNode = AM::pPtnMap()->i_WhichNode(tSendPtn);
  pSrcOp->vSendNode(i_SendNode);

  vSendOperand(pOperand,pSrcOp,i_SendNode,tSendPtn);
  
  // delete the operand
  AM::vDeleteOperand(pOperand);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
