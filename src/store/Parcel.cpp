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
#include "LogicUnit.hpp"


extern SizeHeap      gxaSizeHeap[];
class Table;
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
Parcel::Parcel(void)
: Operand(CN_PARCEL)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARCEL,_FOOTER_);

  m_eCrossTopo     = PX_NONE;
  m_eSendKind      = XK_NONE;

  m_iDstNode       = C_UNKNOWN;
  m_tDstPtn        = C_UNKNOWN;
  m_iDstStage      = C_UNKNOWN;
  m_iDstOp         = C_UNKNOWN;
  
  m_iPipelineJobId = C_UNKNOWN;

  m_bOriginalParcel = false;

  m_nItems = 0;
  for (i8 i_Obj = 0; i_Obj < C_MAX_PARCEL_ITEMS; i_Obj++)
  vSetItem(i_Obj,nullptr);

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
  
  for (i8 i_Obj = 0; i_Obj < C_MAX_PARCEL_ITEMS; i_Obj++)
    pParcel->vSetItem(i_Obj,nullptr);
}

/*----------------------------------------------------------------------------*/
/* repair the parcel                                                          */
/*----------------------------------------------------------------------------*/
void Parcel::vRepair(Object* pObject)
{
  _static_

  Parcel* pParcel = _pParcel(pObject);

  // delete the parcel
  delete pParcel;
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Add object to array                                                        */
/*----------------------------------------------------------------------------*/
void Parcel::vAddItem(Object* pItem)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARCEL,_FOOTER_);

  if (m_nItems < C_MAX_PARCEL_ITEMS) {
    m_pItems[m_nItems++] = pItem;
  }
}

/*----------------------------------------------------------------------------*/
/* get cross topology                                                         */
/*----------------------------------------------------------------------------*/
ePX Parcel::eCrossTopo(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARCEL,_FOOTER_);

  return m_eCrossTopo;
}

/*----------------------------------------------------------------------------*/
/* get destination node                                                       */
/*----------------------------------------------------------------------------*/
i8 Parcel::iDstNode(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARCEL,_FOOTER_);

  return m_iDstNode;
}

/*----------------------------------------------------------------------------*/
/* get destination op                                                         */
/*----------------------------------------------------------------------------*/
i8 Parcel::iDstOp(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARCEL,_FOOTER_);

  return m_iDstOp;
}

/*----------------------------------------------------------------------------*/
/* get destination ptn                                                        */
/*----------------------------------------------------------------------------*/
#ifdef WITH_PTNID
PtnId Parcel::tDstPtn(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARCEL,_FOOTER_);

#ifdef WITH_BKTMAP_2N4P
  Operand* pOperand = pGetOperand();
  if (CN_CRWBUFFER == pOperand->eGetClassNo()) {

    CrwBuffer* pBuffer = (CrwBuffer*)m_pItems[0];
    // Get the bucket number and partition number from buffer
    vTrace("Parcel::tDstPtn CN_CRWBUFFER bkt:%lld  Ptn:%lld\n"_Stz, pBuffer->iBucketNo(), (i8)pBuffer->tPtnNo());

    m_tDstPtn = pBuffer->iBucketNo();
  }
#endif
  return m_tDstPtn;
}
#else
tPN Parcel::tDstPtn(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARCEL,_FOOTER_);
  Operand* pOperand = pGetOperand();

#ifdef WITH_BKTMAP_2N4P
  if (CN_CRWBUFFER == pOperand->eGetClassNo()) {

    CrwBuffer* pBuffer = (CrwBuffer*)m_pItems[0];
    // Get the bucket number and partition number from buffer
    vTrace("Parcel::tDstPtn CN_CRWBUFFER bkt:%lld  Ptn:%lld\n"_Stz, pBuffer->iBucketNo(), (i8)pBuffer->tPtnNo());

    m_tDstPtn = pBuffer->iBucketNo();
  }
#endif
  return m_tDstPtn;
}
#endif

/*----------------------------------------------------------------------------*/
/* get destination stage                                                      */
/*----------------------------------------------------------------------------*/
i8 Parcel::iDstStage(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARCEL,_FOOTER_);

  return m_iDstStage;
}

/*----------------------------------------------------------------------------*/
/* Get an object from array                                                   */
/*----------------------------------------------------------------------------*/
Object* Parcel::pGetItem(i8 i_Item)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARCEL,_FOOTER_);

  if (i_Item < C_MAX_PARCEL_ITEMS && i_Item < m_nItems)
    return m_pItems[i_Item];
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* get Operand                                                                */
/*----------------------------------------------------------------------------*/
Operand* Parcel::pGetOperand(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARCEL,_FOOTER_);

  return (Operand*) m_pItems[0];
}

/*----------------------------------------------------------------------------*/
/* get number of objects in the parcel                                        */
/*----------------------------------------------------------------------------*/
i8 Parcel::nItems(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARCEL,_FOOTER_);

  return m_nItems;
}

/*----------------------------------------------------------------------------*/
/* label this parcel                                                          */
/*----------------------------------------------------------------------------*/
void Parcel::vLabel(ePX eCrossTopo, i8  iPipelineJobId)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARCEL,_FOOTER_);
  
  m_eCrossTopo     = eCrossTopo;
  m_iPipelineJobId = iPipelineJobId;
}

/*----------------------------------------------------------------------------*/
/* is this parcel an original (T or F                                         */
/*----------------------------------------------------------------------------*/
bool Parcel::bOriginalParcel(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARCEL,_FOOTER_);
  return m_bOriginalParcel;
}

/*----------------------------------------------------------------------------*/
/* set the state of origin (T or F)                                           */
/*----------------------------------------------------------------------------*/
void Parcel::vOriginalParcel(bool bOriginalParcel)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARCEL,_FOOTER_);
  
  m_bOriginalParcel = bOriginalParcel;
}

/*----------------------------------------------------------------------------*/
/* get job id of the pipeline job                                             */
/*----------------------------------------------------------------------------*/
i8 Parcel::iPipelineJobId(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARCEL,_FOOTER_);

  return m_iPipelineJobId;
}

/*----------------------------------------------------------------------------*/
/* recv contents of this parcel from given socket                             */
/*----------------------------------------------------------------------------*/
void Parcel::vRecvContents(Socket* pSocket)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARCEL,_FOOTER_);
  
  //vTrace("vRecvContents:\n"_Stz);
  for (i8 i_Object = 0; i_Object < m_nItems; i_Object++) {
    Object* pObject = pSocket->pRecvObject();
    vSetItem(i_Object,pObject);
    //vTrace(" %s\n"_Stz, aCN[pObject->eGetClassNo()]);
  }
}

/*----------------------------------------------------------------------------*/
/* reset item count                                                           */
/*----------------------------------------------------------------------------*/
void Parcel::vReset(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARCEL,_FOOTER_);

  m_nItems = 0;
}

/*----------------------------------------------------------------------------*/
/* get send kind                                                              */
/*----------------------------------------------------------------------------*/
eXK Parcel::eSendKind(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARCEL,_FOOTER_);

  return m_eSendKind;
}

/*----------------------------------------------------------------------------*/
/* send this parcel out                                                       */
/*----------------------------------------------------------------------------*/
void Parcel::vSendOut(Op* pSrcOp)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARCEL,_FOOTER_);
#ifdef WITH_PTNID
  PtnId tDstPtn;
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

  if (pOperand->eGetClassNo()==CN_TABLE) {
    vTrace(">>>>>> Parcel::vSendOut Tried to call vDeleteOperand() to delete this operand CN_TABLE\n"_Stz);
  }

  // delete the operand
  AM::vDeleteOperand(pOperand);
#else
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

  if (pOperand->eGetClassNo()==CN_TABLE) {
    vTrace(">>>>>> Parcel::vSendOut Tried to call vDeleteOperand() to delete this operand CN_TABLE\n"_Stz);
  }

  // delete the operand
  AM::vDeleteOperand(pOperand);
#endif
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
  i8 nObjects = this->nItems();
  
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
  for (i8 i_Item = 0; i_Item < nObjects; i_Item++) {
    pObject = pParcel->pGetItem(i_Item);
    eCN eClassNo = pObject->eGetClassNo();
    nObjectSize = gxaSizeHeap[eClassNo].m_nClassSize;

    if (CN_IDXVEC == eClassNo) {
      i8 iSize1 = gxaSizeHeap[CN_INDEX].m_nClassSize;
      i8 iSize2 = gxaSizeHeap[CN_IDXVEC].m_nClassSize;
      vTrace("vSendOverSocket SENT IdxVec: %lld bytes Index:%lld bytes \n"_Stz,iSize2, iSize1);

      //nObjectSize = gxaSizeHeap[pObject->eGetClassNo()].m_nClassSize + (gxaSizeHeap[CN_INDEX].m_nClassSize * 2);
    }

#ifdef WITH_PTNID
    if (CN_CRWBUFFER == eClassNo && AM::TRACE::bVerbose(C_VB_SEND_CRWBUF)) {
      CrwBuffer* pCrwBuffer = _pCrwBuffer(pObject);
      vTrace("Sending a CrwBuffer (%lld rows) to node %s tPtnNo %lld\n"_Stz
             ,pCrwBuffer->nRows()
             ,pDstNode->szNodeNm()
             ,pCrwBuffer->tPtnNo().iAsI8()
             );
    }
#else
    if (CN_CRWBUFFER == eClassNo && AM::TRACE::bVerbose(C_VB_SEND_CRWBUF)) {
      CrwBuffer* pCrwBuffer = _pCrwBuffer(pObject);
      vTrace("Sending a CrwBuffer (%lld rows) to node %s tPtnNo %lld\n"_Stz
             ,pCrwBuffer->nRows()
             ,pDstNode->szNodeNm()
             ,pCrwBuffer->tPtnNo()
             );
    }
#endif

    bResult = pSocket->bSendTo(pObject, nObjectSize, oSysCall);
    //vTrace("vSendOverSocket SENT %lld bytes class:%s  \n"_Stz,nObjectSize, aCN[pObject->eGetClassNo()]);

  }
  
  pSocket->vDropWrite();

  s256 sSupplemental;
  
  sSupplemental.vSprint("%s %s"_Stz,pDstNode->szNodeNm(),aXK[m_eSendKind]);
  
  pSrcOp->vOpTrace(this,(Stz*)__func__,true,sSupplemental);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* determine where/how this parcel is to be sent                              */
/*----------------------------------------------------------------------------*/
#ifdef WITH_PTNID
void Parcel::vAppraise(Op* pSrcOp, PtnId& tDstPtn, ePX& eCrossTopo, Stage*& pNextStage)
#else
void Parcel::vAppraise(Op* pSrcOp, tPN& tDstPtn, ePX& eCrossTopo, Stage*& pNextStage)
#endif
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARCEL,_FOOTER_);
  
  Pipeline* pPipeline = pSrcOp->pPipeline_op();
  i8 i_NextStage = pSrcOp->iNextStage();
  
  eCrossTopo = pSrcOp->eCrossTopo();
  RASSERT(eCrossTopo == m_eCrossTopo);
  
  tDstPtn = pSrcOp->tPtnNo();
  #ifdef WITH_PTNID
  //vTrace(">>>>>> Parcel::vAppraise Ptn:%lld\n"_Stz, tDstPtn.iAsI8());
  #else
  //vTrace(">>>>>> Parcel::vAppraise Ptn:%lld\n"_Stz, tDstPtn);
  #endif

  pNextStage = pPipeline->pGet(i_NextStage);
  RASSERT(pNextStage);

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
#ifdef WITH_PTNID
void Parcel::vSendCross(Op* pSrcOp, PtnId tDstPtn, ePX eCrossTopo, Stage* pNextStage)
#else
void Parcel::vSendCross(Op* pSrcOp, tPN tDstPtn, ePX eCrossTopo, Stage* pNextStage)
#endif
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARCEL,_FOOTER_);
  
  switch (eCrossTopo) {
      
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // PX cases which send this parcel to the origin
      
    case PX_ALL_NODES_TO_ORIGIN:
    case PX_ALL_PARTITIONS_TO_ORIGIN:
    case PX_ORIGIN_TO_ORIGIN:
    case PX_SPECIFIC_PARTITION_TO_ORIGIN: {
      vSendToOrigin(pSrcOp);
      break;
    }
      
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // PX cases which send this parcel to all partitions
      
    case PX_ORIGIN_TO_ALL_PARTITIONS:
    case PX_ALL_NODES_TO_ALL_PARTITIONS:
    case PX_ALL_PARTITIONS_ON_ORIGIN_TO_ALL_PARTITIONS:
      vSendToAllPartitions(pSrcOp);
      break;
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // PX cases which send this parcel to all partitions on origin
      
    case PX_ORIGIN_TO_ALL_PARTITIONS_ON_ORIGIN:
      vSendToAllOriginPartitions(pSrcOp);
      break;
     
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // PX cases which send this parcel to a specific partition
      
    case PX_ORIGIN_TO_SPECIFIC_PARTITION: {
      //vTrace("vSendToPartition  \n"_Stz);
      #ifdef WITH_PTNID
      Stream* pStream = pNextStage->pGetPartitionStream(tDstPtn.iAsI8());
      PtnId tDstPtnNo_stream = pStream->tPtnNo();
      vSendToPartition(pSrcOp,tDstPtnNo_stream);
      #else
      Stream* pStream = pNextStage->pGetPartitionStream(tDstPtn);
      tPN tDstPtnNo_stream = pStream->tPtnNo();
      vSendToPartition(pSrcOp,tDstPtnNo_stream);
      #endif
      break;
    }
      
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // PX cases which send this parcel to ???? ALKA
      
    case PX_ALL_PARTITIONS_TO_ALL_NODES: {
      i8 iDstNode = AM::pPtnMap()->iWhichNodeNo(tDstPtn);
      vSendToNode(pSrcOp,iDstNode);
      break;
    }
      
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // PX cases which send this parcel to ???? ALKA
      
    case PX_ALL_PARTITIONS_TO_ALL_PARTITIONS: {

      if (m_eSendKind == XK_BROADCAST) {
        vSendToAllPartitions(pSrcOp);
      }
      else {
        if (pGetOperand()->bIsBuffer()) {
          vSendToPartition(pSrcOp, tDstPtn);
        }
        else {
          vSendToAllPartitions(pSrcOp);
        }
      }

      break;
    }
     
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // PX cases which send this parcel to all nodes
      
    case PX_ORIGIN_TO_ALL_NODES: {
      vSendToAllNodes(pSrcOp);
      break;
    }
      
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      
    default:
      RASSERT(0);
  }
}

/*----------------------------------------------------------------------------*/
/* send parcel to given node,partition                                        */
/*----------------------------------------------------------------------------*/
#ifdef WITH_PTNID
void Parcel::vSendParcel(Op* pSrcOp, i8  iDstNode, PtnId tDstPtn)
#else
void Parcel::vSendParcel(Op* pSrcOp, i8  iDstNode, tPN tDstPtn)
#endif
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
/* send this parcel to all nodes                                              */
/*----------------------------------------------------------------------------*/
void Parcel::vSendToAllNodes(Op* pSrcOp)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARCEL,_FOOTER_);

  Pipeline* pPipeline = pSrcOp->pPipeline_op();
  i8 i_NextStage = pSrcOp->iNextStage();

  // get the next stage
  Stage* pNextStage = pPipeline->pGet(i_NextStage);
  RASSERT(pNextStage);

  //vTrace("Num of nodes:%lld\n"_Stz, pNextStage->nItems());

  //for (i8 iDstNode = 0; iDstNode < pNextStage->nItems(); iDstNode++)
    //vSendParcel(pSrcOp,iDstNode,C_UNKNOWN);

  i8 iDstNode = 0;
#ifdef WITH_PTNID
  PtnId tPtnId;
  tPtnId = C_UNKNOWN;
  for (Node_I oNode_I; oNode_I.bMore(); oNode_I.vNext()) {
    Node* pNode = (*oNode_I);
    iDstNode = pNode->iNodeNo();
    vSendParcel(pSrcOp, iDstNode, tPtnId);
  }
#else
  for (Node_I oNode_I; oNode_I.bMore(); oNode_I.vNext()) {
    Node* pNode = (*oNode_I);
    iDstNode = pNode->iNodeNo();
    vSendParcel(pSrcOp, iDstNode, C_UNKNOWN);
  }
#endif
}

/*----------------------------------------------------------------------------*/
/* send this parcel to all partitions on origin                               */
/*----------------------------------------------------------------------------*/
void Parcel::vSendToAllOriginPartitions(Op* pSrcOp)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARCEL,_FOOTER_);
#ifdef WITH_PTNID
  PtnId tPtnNo;
#else
  tPN tPtnNo;
#endif
  //i8 i_TablePtn;
  
  Pipeline* pPipeline = pSrcOp->pPipeline_op();
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
#ifdef WITH_PTNID
  PtnId tPtnNo;
#else
  tPN tPtnNo;
#endif
  
  Operand* pOperand = pGetOperand();
  
  Pipeline* pPipeline = pSrcOp->pPipeline_op();
  i8 i_NextStage = pSrcOp->iNextStage();
  
  // get the next stage
  Stage* pNextStage = pPipeline->pGet(i_NextStage);
  RASSERT(pNextStage);
  //vTrace("SendKind:%s\n"_Stz, aXK[m_eSendKind]);
  switch (m_eSendKind) {

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    case XK_BROADCAST : {
      ASSERT(pNextStage->nItems() == AM::PTNMAP::nPtnsInGrid());
#ifdef WITH_PTNID
      for (i8 iPtnNo = 0; iPtnNo < pNextStage->nItems(); iPtnNo++) {
        tPtnNo = iPtnNo;
        // sendNode is the partition node
        i8 iDstNode = AM::pPtnMap()->iWhichNodeNo(tPtnNo);
        pSrcOp->vSendNode(iDstNode);
        vSendParcel(pSrcOp,iDstNode,tPtnNo);
      }
#else
      for (tPtnNo = 0; tPtnNo < pNextStage->nItems(); tPtnNo++) {

        // sendNode is the partition node
        i8 iDstNode = AM::pPtnMap()->iWhichNodeNo(tPtnNo);
        pSrcOp->vSendNode(iDstNode);
        vSendParcel(pSrcOp,iDstNode,tPtnNo);
      }
#endif
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
#ifdef WITH_PTNID
      for (i8 iPtnNo = 0; iPtnNo < pNextStage->nItems(); iPtnNo++) {
        tPtnNo = iPtnNo;
        // sendNode is the partition node
        i8 iDstNode = AM::pPtnMap()->iWhichNodeNo(tPtnNo);
        pSrcOp->vSendNode(iDstNode);
        vSendParcel(pSrcOp,iDstNode,tPtnNo);
      }
#else
      for (tPtnNo = 0; tPtnNo < pNextStage->nItems(); tPtnNo++) {
        
        // sendNode is the partition node
        i8 iDstNode = AM::pPtnMap()->iWhichNodeNo(tPtnNo);
        pSrcOp->vSendNode(iDstNode);
        vSendParcel(pSrcOp,iDstNode,tPtnNo);
      }
#endif
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
#ifdef WITH_BKTMAP_2N4P
  vTrace("vSendToNode iDstNode:%lld\n"_Stz, iDstNode);
#endif
  Pipeline* pPipeline = pSrcOp->pPipeline_op();
  i8 i_NextStage = pSrcOp->iNextStage();
  
  // get the next stage
  Stage* pNextStage = pPipeline->pGet(i_NextStage);
  RASSERT(pNextStage);
  
  ASSERT(pNextStage->nItems() == AM::pGrid()->nKnownNodes());
#ifdef WITH_PTNID
  PtnId tPtnId;
  tPtnId = C_UNKNOWN;
  vSendParcel(pSrcOp,iDstNode,tPtnId);
#else
  vSendParcel(pSrcOp,iDstNode,C_UNKNOWN);
#endif
}

/*----------------------------------------------------------------------------*/
/* send this parcel to origin                                                 */
/*----------------------------------------------------------------------------*/
void Parcel::vSendToOrigin(Op* pSrcOp)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARCEL,_FOOTER_);

  Pipeline* pPipeline = pSrcOp->pPipeline_op();

  i8 iDstNode = pPipeline->i_Origin();
  pSrcOp->vSendNode(iDstNode);
  //vTrace("vSendToOrigin iDstNode:%lld\n"_Stz, iDstNode);
#ifdef WITH_PTNID
  PtnId tPtnId;
  tPtnId = C_UNKNOWN;
  vSendParcel(pSrcOp, iDstNode, tPtnId);
#else
  vSendParcel(pSrcOp,iDstNode,C_UNKNOWN);
#endif
}

/*----------------------------------------------------------------------------*/
/* send this parcel to given partition                                        */
/*----------------------------------------------------------------------------*/
#ifdef WITH_PTNID
void Parcel::vSendToPartition(Op* pSrcOp, PtnId tDstPtn)
#else
void Parcel::vSendToPartition(Op* pSrcOp, tPN tDstPtn)
#endif
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
  //vTrace("vSendToPartition iDstNode:%lld  tDstPtn:%lld\n"_Stz, iDstNode, tDstPtn);
  vSendParcel(pSrcOp,iDstNode,tDstPtn);
}

/*----------------------------------------------------------------------------*/
/* set item in array                                                          */
/*----------------------------------------------------------------------------*/
void Parcel::vSetItem(i8 i_Item, Object* pItem)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARCEL,_FOOTER_);

  m_pItems[i_Item] = pItem;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
