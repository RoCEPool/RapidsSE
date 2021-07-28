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
#include "Node.hpp"
#include "Grid.hpp"
#include "Socket.hpp"
#include "Node_I.hpp"

#include "TagConstants.h"

#include "CrwBuffer.hpp"

extern SizeHeap      gxaSizeHeap[];

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
               tPN      tDstPtn,
               ePX      eCrossTopo)
: Operand(CN_PARCEL)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARCEL,_FOOTER_);

  m_bOriginalParcel = false;

  m_nObjects = 0;
  for (i8 i_Obj = 0; i_Obj < C_MAX_PARCEL_OBJ; i_Obj++)
    vSetObject(i_Obj,nullptr);

  if (pOperand)
    vAddObject(pOperand);

  m_iPipelineJobId = iPipelineJobId;
  m_eCrossTopo     = eCrossTopo;
  m_eSendKind      = XK_NONE;
  m_bAppendRight   = false;
  
  m_iDstNode       = iDstNode;
  m_tDstPtn        = tDstPtn; 
  m_iDstStage      = C_UNKNOWN;
  m_iDstOp         = C_UNKNOWN;
  
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
/* post recv function                                                         */
/*----------------------------------------------------------------------------*/
void Parcel::vPostRecv(void* aParcel)
{
  _static_
  
  Parcel* pParcel = _pParcel(aParcel);
  
  for (i8 i_Obj = 0; i_Obj < C_MAX_PARCEL_OBJ; i_Obj++)
    pParcel->vSetObject(i_Obj,nullptr);
}


/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Set label for this Parcel                                                  */
/*----------------------------------------------------------------------------*/
void Parcel::vLabel(i8      iPipelineJobId,
                   i8       iDstNode,
                   tPN      tDstPtn,
                   ePX      eCrossTopo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARCEL,_FOOTER_);
  m_iPipelineJobId = iPipelineJobId;
  m_eCrossTopo     = eCrossTopo;

  m_iDstNode       = iDstNode;
  m_tDstPtn        = tDstPtn;
}

/*----------------------------------------------------------------------------*/
/* reset item count                                                           */
/*----------------------------------------------------------------------------*/
void Parcel::vReset(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARCEL,_FOOTER_);

  m_nObjects = 0;
}

/*----------------------------------------------------------------------------*/
/* Add object to array                                                        */
/*----------------------------------------------------------------------------*/
void Parcel::vAddObject(Object* pObject)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARCEL,_FOOTER_);

  if (m_nObjects < C_MAX_PARCEL_OBJ) {
    m_pObjects[m_nObjects++] = pObject;
  }
}

/*----------------------------------------------------------------------------*/
/* append to right queue ?                                                     */
/*----------------------------------------------------------------------------*/
bool Parcel::bAppendRight(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARCEL,_FOOTER_);

  return m_bAppendRight;
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
/* get Operand                                                                */
/*----------------------------------------------------------------------------*/
Operand* Parcel::pGetOperand(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARCEL,_FOOTER_);

  return (Operand*) m_pObjects[0];
}

/*----------------------------------------------------------------------------*/
/* is this parcel an original (T or F                                         */
/*----------------------------------------------------------------------------*/
bool Parcel::bIsOriginal(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARCEL,_FOOTER_);
  return m_bOriginalParcel;
}


/*----------------------------------------------------------------------------*/
/* set the state of origin (T or F)                                           */
/*----------------------------------------------------------------------------*/
void Parcel::vSetOriginState(bool bState)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARCEL,_FOOTER_);
  m_bOriginalParcel = bState;
}

/*----------------------------------------------------------------------------*/
/* get number of objects in the parcel                                        */
/*----------------------------------------------------------------------------*/
i8 Parcel::nObjects(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARCEL,_FOOTER_);

  return m_nObjects;
}

/*----------------------------------------------------------------------------*/
/* recv contents of this parcel from given socket                             */
/*----------------------------------------------------------------------------*/
void Parcel::vRecvContents(Socket* pSocket)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARCEL,_FOOTER_);
  
  //vTrace("vRecvContents:\n"_Stz);
  for (i8 i_Object = 0; i_Object < m_nObjects; i_Object++) {
    Object* pObject = pSocket->pRecvObject();
    vSetObject(i_Object,pObject);
    //vTrace(" %s\n"_Stz, aCN[pObject->eGetClassNo()]);
  }
}

/*----------------------------------------------------------------------------*/
/* send this parcel out                                                       */
/*----------------------------------------------------------------------------*/
void Parcel::vSendOut(Op* pSrcOp)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARCEL,_FOOTER_);


//  Pipeline* pPipeline = pSrcOp->pPipeline();
//  i8 i_NextStage = pSrcOp->iNextStage();
//  ePX eCrossTopo = pSrcOp->eCrossTopo();
//  RASSERT(eCrossTopo == m_eCrossTopo);
//  tPN tDstPtn = pSrcOp->tPtnNo();
//
//  // get operand and next stage
//  Operand* pOperand = pGetOperand();
//  Stage* pNextStage = pPipeline->pGet(i_NextStage);
//  RASSERT(pNextStage);
//
//  // figure out the queue
//  m_bAppendRight = pSrcOp->bAppendRight();
//
//  // figure out the send kind
//  m_eSendKind = pSrcOp->eSendKind();
//
//  // is the operand is a marker or a streamDef ?
//  if (pOperand->bIsMarker() || pOperand->bIsStreamDef())
//    m_eSendKind = XK_UNICAST;
//
//  // is the operand an InfoSet ?
//  if (pOperand->bIsInfoSet() || pOperand->bIsProfile())
//    m_eSendKind = XK_ROBIN;
  
  tPN tDstPtn;
  ePX eCrossTopo;
  Stage* pNextStage;
  
  vAppraise(pSrcOp,tDstPtn,eCrossTopo,pNextStage);
  
  Operand* pOperand = pGetOperand();
  if (CN_METAGET == pOperand->eGetClassNo()) {
    // Send metaGet to other node, not to the origin node itself
    vSendToNode(pSrcOp,6078); // HACK  hardcode to the existing node for debug
    AM::vDeleteOperand(pOperand);
    return;
  }

  vSendCross(pSrcOp,tDstPtn,eCrossTopo,pNextStage);
  
  //>>>>>>>>>>>  Need to ask Ray, after sent the Parcel, we get the operand which is the table and try to delete
  //>>>>>>>>>>>  If we don't get the operand then we will delete the parcel
  if (pOperand->eGetClassNo()==CN_TABLE)
  {
    vTrace(">>>>>> Parcel::vSendOut Tried to call vDeleteOperand() to delete this operand CN_TABLE\n"_Stz);
  }

  // delete the operand
  AM::vDeleteOperand(pOperand);
}

/*----------------------------------------------------------------------------*/
/* set object in array                                                        */
/*----------------------------------------------------------------------------*/
void Parcel::vSetObject(i8 i_Object, Object* pObject)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARCEL,_FOOTER_);

  m_pObjects[i_Object] = pObject;
}


/*----------------------------------------------------------------------------*/
/*-- Private Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* send parcel (and its contents) out over socket                             */
/*----------------------------------------------------------------------------*/
void Parcel::vSendOverSocket(Op* pSrcOp)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARCEL,_FOOTER_);
  
  bool bResult;
  i8 nObjectSize;
  SysCall oSysCall;
  Object* pObject;
  Parcel* pParcel;
  
  // get preliminary information
  Node* pDstNode = AM::pGrid()->pFindNode(m_iDstNode);
  //vTrace("vSendOverSocket prepared to send to Node:%lld Port:%lld \n"_Stz,pDstNode->iNodeNo(), pDstNode->iPortNo());
  Socket* pSocket = pDstNode->pSendSocket();
  i8 nObjects = this->nObjects();
  
  pSocket->vGrabWrite();

  //vTrace("vSendOverSocket got a lock, prepared to send %lld Object(s)to Node:%lld Port:%lld \n"_Stz,nObjects, pDstNode->iNodeNo(), pDstNode->iPortNo());

  // send the parcel instance
  pParcel = this;
  nObjectSize = gxaSizeHeap[pParcel->eGetClassNo()].m_nClassSize;
  bResult = pSocket->bSendTo(pParcel, nObjectSize, oSysCall);

  //if (!bResult) {
  //  vTrace("vSendOverSocket SENT FAILED\n"_Stz);
  //} else {
  //  vTrace("vSendOverSocket SENT %lld bytes Parcel:%s to Node:%lld Port:%lld \n"_Stz,nObjectSize, aCN[pParcel->pGetOperand()->eGetClassNo()], pDstNode->iNodeNo(), pDstNode->iPortNo());
  //}

  // send all the objects within the parcel instance
  for (i8 i_Object = 0; i_Object < nObjects; i_Object++) {
    pObject = pParcel->pGetObject(i_Object);
    eCN eClassNo = pObject->eGetClassNo();
    nObjectSize = gxaSizeHeap[eClassNo].m_nClassSize;

    if (CN_IDXVEC == eClassNo) {
      i8 iSize1 = gxaSizeHeap[CN_INDEX].m_nClassSize;
      i8 iSize2 = gxaSizeHeap[CN_IDXVEC].m_nClassSize;
      vTrace("vSendOverSocket SENT IdxVec: %lld bytes Index:%lld bytes \n"_Stz,iSize2, iSize1);

      //nObjectSize = gxaSizeHeap[pObject->eGetClassNo()].m_nClassSize + (gxaSizeHeap[CN_INDEX].m_nClassSize * 2);
    }

    if (CN_CRWBUFFER == eClassNo && AM::TRACE::bVerbose(C_VB_SEND_CRWBUF)) {
      CrwBuffer* pCrwBuffer = _pCrwBuffer(pObject);
      vTrace("Sending a CrwBuffer (%lld rows) to node %s tPtnNo %lld\n"_Stz
             ,pCrwBuffer->nRows()
             ,pDstNode->szNodeNm()
             ,pCrwBuffer->tPtnNo()
             );
    }

    bResult = pSocket->bSendTo(pObject, nObjectSize, oSysCall);
    //vTrace("vSendOverSocket SENT %lld bytes class:%s  \n"_Stz,nObjectSize, aCN[pObject->eGetClassNo()]);
  }
  
  pSocket->vDropWrite();
  
  s256 sSupplemental;
  
  sSupplemental.vSprint("%s %s"_Stz,pDstNode->szNodeNm(),aXK[m_eSendKind]);
  
  pSrcOp->vOpTrace(this,(Stz*)__func__,true,sSupplemental);
}

/*----------------------------------------------------------------------------*/
/* send parcel to given node,partition                                        */
/*----------------------------------------------------------------------------*/
void Parcel::vSendParcel(Op* pSrcOp, i8  iDstNode, tPN tDstPtn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARCEL,_FOOTER_);

  m_iDstNode = iDstNode;
  m_tDstPtn   = tDstPtn;
  
  m_iDstStage = pSrcOp->iNextStage();
  m_iDstOp    = pSrcOp->iNextOp();
  
  vSendOverSocket(pSrcOp);

  // - - - - - - - - - - - - - - - -
  
  i8 i_OtherStage = pSrcOp->iOtherStage();
  i8 i_OtherOp    = pSrcOp->iOtherOp();
  
  if (C_UNKNOWN != i_OtherStage) {

    m_iDstStage = i_OtherStage;
    m_iDstOp    = i_OtherOp;

    vSendOverSocket(pSrcOp);
  }
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* determine where/how this parcel is to be sent                              */
/*----------------------------------------------------------------------------*/
void Parcel::vAppraise(Op* pSrcOp, tPN& tDstPtn, ePX& eCrossTopo, Stage*& pNextStage)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARCEL,_FOOTER_);
  
  Pipeline* pPipeline = pSrcOp->pPipeline();
  i8 i_NextStage = pSrcOp->iNextStage();
  
  eCrossTopo = pSrcOp->eCrossTopo();
  RASSERT(eCrossTopo == m_eCrossTopo);
  
  tDstPtn = pSrcOp->tPtnNo();

  pNextStage = pPipeline->pGet(i_NextStage);
  RASSERT(pNextStage);

  // figure out the queue
  m_bAppendRight = pSrcOp->bAppendRight();

  // figure out the send kind
  m_eSendKind = pSrcOp->eSendKind();
    
  // is the operand is a marker or a streamDef ?
  Operand* pOperand = pGetOperand();
  if (pOperand->bIsMarker() || pOperand->bIsStreamDef())
    m_eSendKind = XK_UNICAST;
  
  // is the operand an InfoSet ?
  if (pOperand->bIsInfoSet() || pOperand->bIsProfile())
    m_eSendKind = XK_ROBIN;
}

/*----------------------------------------------------------------------------*/
/* cross-topo send of this parcel                                             */
/*----------------------------------------------------------------------------*/
void Parcel::vSendCross(Op* pSrcOp, tPN tDstPtn, ePX eCrossTopo, Stage* pNextStage)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARCEL,_FOOTER_);
  
  switch (eCrossTopo) {
      
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    case PX_ALL_NODES_TO_ORIGIN:
    case PX_ALL_PARTITIONS_TO_ORIGIN:
    case PX_ORIGIN_TO_ORIGIN:
    case PX_SPECIFIC_PARTITION_TO_ORIGIN: {
      vSendToOrigin(pSrcOp);
      break;
    }
      
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    case PX_ORIGIN_TO_ALL_PARTITIONS:
    case PX_ALL_NODES_TO_ALL_PARTITIONS:
    case PX_ALL_PARTITIONS_ON_ORIGIN_TO_ALL_PARTITIONS: {
      vSendToAllPartitions(pSrcOp);
      break;
    }
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    case PX_ORIGIN_TO_ALL_PARTITIONS_ON_ORIGIN: {
      vSendToAllOriginPartitions(pSrcOp);
      break;
    }
     
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    case PX_ORIGIN_TO_SPECIFIC_PARTITION: {
      Stream* pStream = pNextStage->pGetPartitionStream(tDstPtn);
      tPN tDstPtnNo_stream = pStream->tPtnNo();
      vSendToPartition(pSrcOp,tDstPtnNo_stream);
      break;
    }
      
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    case PX_ALL_PARTITIONS_TO_ALL_NODES: {
      i8 iDstNode = AM::pPtnMap()->iWhichNodeNo(tDstPtn);
      vSendToNode(pSrcOp,iDstNode);
      break;
    }
      
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    case PX_ALL_PARTITIONS_TO_ALL_PARTITIONS: {

      if (m_eSendKind == XK_BROADCAST) {
        vSendToAllPartitions(pSrcOp);
      }
      else {
        if (pGetOperand()->bIsBuffer())
          vSendToPartition(pSrcOp, tDstPtn);
        else
          vSendToAllPartitions(pSrcOp);
      }
      break;
    }
     
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    case PX_ORIGIN_TO_ALL_NODES: {
      vSendToAllNodes(pSrcOp);
      break;
    }
      
    default:
      RASSERT(0);
  }

}

/*----------------------------------------------------------------------------*/
/* send this parcel to all nodes                                              */
/*----------------------------------------------------------------------------*/
void Parcel::vSendToAllNodes(Op* pSrcOp)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARCEL,_FOOTER_);

  Pipeline* pPipeline = pSrcOp->pPipeline();
  i8 i_NextStage = pSrcOp->iNextStage();

  // get the next stage
  Stage* pNextStage = pPipeline->pGet(i_NextStage);
  RASSERT(pNextStage);

  //vTrace("Num of nodes:%lld\n"_Stz, pNextStage->nItems());

  //for (i8 iDstNode = 0; iDstNode < pNextStage->nItems(); iDstNode++)
    //vSendParcel(pSrcOp,iDstNode,C_UNKNOWN);

  i8 iDstNode = 0;

  for (Node_I oNode_I; oNode_I.bMore(); oNode_I.vNext()) {
    Node* pNode = (*oNode_I);
    iDstNode = pNode->iNodeNo();
    vSendParcel(pSrcOp, iDstNode, C_UNKNOWN);
    //vTrace("Node :%lld\n"_Stz, iDstNode);
  }

}

/*----------------------------------------------------------------------------*/
/* send this parcel to all partitions on origin                               */
/*----------------------------------------------------------------------------*/
void Parcel::vSendToAllOriginPartitions(Op* pSrcOp)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARCEL,_FOOTER_);

  tPN tPtnNo;
  i8 i_TablePtn;
  
  Pipeline* pPipeline = pSrcOp->pPipeline();
  i8 i_NextStage = pSrcOp->iNextStage();

  
  // get the next stage
  Stage* pNextStage = pPipeline->pGet(i_NextStage);
  RASSERT(pNextStage);
    
  // check the SendKind
  switch (m_eSendKind) {
      
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    case XK_ROBIN : {
      i8 i_Robin = pSrcOp->iRobinNext();
      tPtnNo = AM::PTNMAP::tRobinToNdePtnNo(i_Robin,pPipeline->i_Origin());
      i8 iDstNodeNo = AM::pPtnMap()->iWhichNodeNo(tPtnNo);
      pSrcOp->vSendNode(iDstNodeNo);

      RASSERT(iDstNodeNo == pPipeline->i_Origin());
      vSendParcel(pSrcOp,iDstNodeNo,tPtnNo);

      break;
    }
      
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    case XK_UNICAST: {
      
      ASSERT(pNextStage->nItems() == AM::PTNMAP::nPtnsPerNode());
      for (i8 i_Stream = 0; i_Stream < pNextStage->nItems(); i_Stream++) {
        
        Stream* pStream = pNextStage->pGet(i_Stream);
        tPtnNo = pStream->tPtnNo();
        
        // sendNode is the partition node
    
        i8 iDstNode = AM::pPtnMap()->iWhichNodeNo(tPtnNo);
        pSrcOp->vSendNode(iDstNode);
        if ( iDstNode == pPipeline->i_Origin())
          vSendParcel(pSrcOp,iDstNode,tPtnNo);
      }
    }
    break;
      
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    default:
      ASSERT(0);
  }
}
/*----------------------------------------------------------------------------*/
// discuss with alka
#if 0
void Parcel::vSendToAllOriginPartitions(Op* pSrcOp)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARCEL,_FOOTER_);

  tPN tPtnNo;
  i8 i_TablePtn;
  
  Pipeline* pPipeline = pSrcOp->pPipeline();
  i8 i_NextStage = pSrcOp->iNextStage();

  
  // get the next stage
  Stage* pNextStage = pPipeline->pGet(i_NextStage);
  RASSERT(pNextStage);
    
  // check the SendKind
  switch (m_eSendKind) {
      
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    case XK_ROBIN : {
      i8 i_Robin = pSrcOp->iRobinNext();
      tPtnNo = AM::PTNMAP::tRobinToDtdPtnNo(i_Robin);
      i8 iDstNodeNo = AM::pPtnMap()->iWhichNodeNo(tPtnNo);

      pSrcOp->vSendNode(iDstNodeNo);

      if (iDstNodeNo == pPipeline->i_Origin())
        vSendParcel(pSrcOp,iDstNodeNo,tPtnNo);

      break;
    }
      
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    case XK_UNICAST: {
      
      ASSERT(pNextStage->nItems() == AM::PTNMAP::nPtnsPerNode());
      for (i8 i_Stream = 0; i_Stream < pNextStage->nItems(); i_Stream++) {
        
        Stream* pStream = pNextStage->pGet(i_Stream);
        tPtnNo = pStream->tPtnNo();
        
        // sendNode is the partition node
    
        i8 iDstNode = AM::pPtnMap()->iWhichNodeNo(tPtnNo);
        pSrcOp->vSendNode(iDstNode);
        if ( iDstNode == pPipeline->i_Origin())
          vSendParcel(pSrcOp,iDstNode,tPtnNo);
      }
    }
    break;
      
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    default:
      ASSERT(0);
  }
}
#endif

/*----------------------------------------------------------------------------*/
/* send this parcel to all partitions                                         */
/*----------------------------------------------------------------------------*/
void Parcel::vSendToAllPartitions(Op* pSrcOp)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARCEL,_FOOTER_);

  tPN tPtnNo;
  i8 i_TablePtn;
  
  Operand* pOperand = pGetOperand();
  
  Pipeline* pPipeline = pSrcOp->pPipeline();
  i8 i_NextStage = pSrcOp->iNextStage();
  
  // get the next stage
  Stage* pNextStage = pPipeline->pGet(i_NextStage);
  RASSERT(pNextStage);
    
  switch (m_eSendKind) {

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    case XK_BROADCAST : {
      ASSERT(pNextStage->nItems() == AM::PTNMAP::nPtnsInGrid());
      for (tPtnNo = 0; tPtnNo < pNextStage->nItems(); tPtnNo++) {

        // sendNode is the partition node
        i8 iDstNode = AM::pPtnMap()->iWhichNodeNo(tPtnNo);
        pSrcOp->vSendNode(iDstNode);
        vSendParcel(pSrcOp,iDstNode,tPtnNo);
      }
      break;
    }

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    case XK_HASHCAST : {
      DataBuffer* pDataBuffer = _pDataBuffer(pOperand);
      tPtnNo = pDataBuffer->tPtnNo();
      
      // sendNode is the partition node
      i8 iDstNode = AM::pPtnMap()->iWhichNodeNo(tPtnNo);
      pSrcOp->vSendNode(iDstNode);

      vSendParcel(pSrcOp,iDstNode,tPtnNo);
      break;
    }
      
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    case XK_LOCALROBIN : {
      i8 i_LocalRobin = pSrcOp->iLocalRobinNext();
      tPtnNo = AM::PTNMAP::tRobinToLclPtnNo(i_LocalRobin);
      
      // sendNode is this node
      i8 iDstNode = AM::pPtnMap()->iWhichNodeNo(tPtnNo);
      pSrcOp->vSendNode(iDstNode);
      RASSERT(iDstNode == AM::pGrid()->iThisNodeNo());
      
      vSendParcel(pSrcOp,iDstNode,tPtnNo);
      break;
    }
      
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    case XK_UNICAST: {
      
      ASSERT(pNextStage->nItems() == AM::PTNMAP::nPtnsInGrid());
      for (tPtnNo = 0; tPtnNo < pNextStage->nItems(); tPtnNo++) {
        
        // sendNode is the partition node
        i8 iDstNode = AM::pPtnMap()->iWhichNodeNo(tPtnNo);
        pSrcOp->vSendNode(iDstNode);
        vSendParcel(pSrcOp,iDstNode,tPtnNo);
      }
      break;
    }
      
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    case XK_ROBIN : {
      i8 i_Robin = pSrcOp->iRobinNext();
      tPtnNo = AM::PTNMAP::tRobinToDtdPtnNo(i_Robin);
      i8 iDstNodeNo = AM::pPtnMap()->iWhichNodeNo(tPtnNo);

      pSrcOp->vSendNode(iDstNodeNo);

      vSendParcel(pSrcOp,iDstNodeNo,tPtnNo);
      break;
    }
      
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    default:
      ASSERT(0);
  }
}

/*----------------------------------------------------------------------------*/
/* send this parcel to given destination node                                 */
/*----------------------------------------------------------------------------*/
void Parcel::vSendToNode(Op* pSrcOp, i8 iDstNode)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARCEL,_FOOTER_);
  //vTrace("vSendToNode iDstNode:%lld\n"_Stz, iDstNode);
  Pipeline* pPipeline = pSrcOp->pPipeline();
  i8 i_NextStage = pSrcOp->iNextStage();
  
  // get the next stage
  Stage* pNextStage = pPipeline->pGet(i_NextStage);
  RASSERT(pNextStage);
  
  ASSERT(pNextStage->nItems() == AM::pGrid()->nKnownNodes());
  
  vSendParcel(pSrcOp,iDstNode,C_UNKNOWN);
}

/*----------------------------------------------------------------------------*/
/* send this parcel to origin                                                 */
/*----------------------------------------------------------------------------*/
void Parcel::vSendToOrigin(Op* pSrcOp)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARCEL,_FOOTER_);

  Pipeline* pPipeline = pSrcOp->pPipeline();

  i8 iDstNode = pPipeline->i_Origin();
  pSrcOp->vSendNode(iDstNode);
  //vTrace("vSendToOrigin iDstNode:%lld\n"_Stz, iDstNode);
  vSendParcel(pSrcOp,iDstNode,C_UNKNOWN);
}

/*----------------------------------------------------------------------------*/
/* send this parcel to given partition                                        */
/*----------------------------------------------------------------------------*/
void Parcel::vSendToPartition(Op* pSrcOp, tPN tDstPtn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARCEL,_FOOTER_);

  Operand* pOperand = pGetOperand();

  i8 i_Robin;
      
  DataBuffer* pDataBuffer = nullptr;
  
  switch (m_eSendKind) {

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    case XK_BROADCAST :
      RASSERT(0);
      break;

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    case XK_HASHCAST :
      pDataBuffer = _pDataBuffer(pOperand);
      tDstPtn = pDataBuffer->tPtnNo();
      break;
      
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    case XK_UNICAST:
      break;
      
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    case XK_ROBIN: {
      i_Robin = pSrcOp->iRobinNext();
      tDstPtn = AM::PTNMAP::tRobinToDtdPtnNo(i_Robin);
      break;
    }
      
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    default:
      ASSERT(0);
  }
  
  i8 iDstNode = AM::pPtnMap()->iWhichNodeNo(tDstPtn);
  pSrcOp->vSendNode(iDstNode);
  vTrace("vSendToPartition iDstNode:%lld  tDstPtn:%lld\n"_Stz, iDstNode, tDstPtn);
  vSendParcel(pSrcOp,iDstNode,tDstPtn);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
