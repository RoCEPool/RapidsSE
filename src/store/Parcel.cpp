/*============================================================================*/
/* Parcel.cpp -- Send Parcel                                                  */
/*============================================================================*/

#include "BASE.h"

#include "eYX.h"

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

  m_eCrossTopology     = YX_NONE;
  m_eIntraTopology    = YO_NONE;
  
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
eYX Parcel::eCrossTopology(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARCEL,_FOOTER_);

  return m_eCrossTopology;
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
PtnId Parcel::tDstPtn(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARCEL,_FOOTER_);

#ifdef WITH_BKTMAP_2N4P
  Operand* pOperand = pGetOperand();
  if (CN_CRWBUFFER == pOperand->eGetClassNo()) {

    CrwBuffer* pBuffer = (CrwBuffer*)m_pItems[0];
    // Get the bucket number and partition number from buffer
    vTrace("Parcel::tDstPtn CN_CRWBUFFER bkt:%lld  Ptn:%lld\n"_Stz, pBuffer->iBucketNo(), (i8)pBuffer->tPtnId());

    m_tDstPtn = pBuffer->iBucketNo();
  }
#endif
  return m_tDstPtn;
}


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
void Parcel::vLabel(eYX eCrossTopology, eYO eIntraTopology, i8  iPipelineJobId)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARCEL,_FOOTER_);
  
  m_eCrossTopology     = eCrossTopology;
  m_eIntraTopology    = eIntraTopology;
  m_iPipelineJobId = iPipelineJobId;
}

/*----------------------------------------------------------------------------*/
/* get op topology                                                            */
/*----------------------------------------------------------------------------*/
eYO Parcel::eIntraTopology(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARCEL,_FOOTER_);

  return m_eIntraTopology;
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
void Parcel::vSendOut(Op* pSendOp)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARCEL,_FOOTER_);
  
  RASSERT(pSendOp->szOpNm()->bSame("send"_Stz));

  vAppraiseSendKind(pSendOp);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Operand* pOperand = pGetOperand();
  if (CN_METAGET == pOperand->eGetClassNo()) {
    // Send metaGet to other node, not to the origin node itself
    vSendToNode(pSendOp); // HACK  hardcode to the existing node for debug
    AM::vDeleteOperand(pOperand);
    return;
  }
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  vSendCrossTopology(pSendOp);

  if (pOperand->eGetClassNo()==CN_TABLE) {
    vTrace(">>>>>> Parcel::vSendOut Tried to call vDeleteOperand() to delete this operand CN_TABLE\n"_Stz);
  }

  // delete the operand
  AM::vDeleteOperand(pOperand);
}


/*----------------------------------------------------------------------------*/
/*-- Private Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* determine where/how this parcel is to be sent                              */
/*----------------------------------------------------------------------------*/
void Parcel::vAppraiseSendKind(Op* pSendOp)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARCEL,_FOOTER_);
    
  // figure out the send kind
  m_eSendKind = pSendOp->eSendKind();
    
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
void Parcel::vSendCrossTopology(Op* pSendOp)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARCEL,_FOOTER_);
  
  switch (m_eCrossTopology) {
      
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // PX cases which send this parcel to the origin
      
    case YX_ALL_NODES_TO_ORIGIN:
    case YX_ALL_PARTITIONS_TO_ORIGIN:
    case YX_ORIGIN_TO_ORIGIN:
    case YX_SPECIFIC_PARTITION_TO_ORIGIN: {
      vSendToOrigin(pSendOp);
      break;
    }
      
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // PX cases which send this parcel to all partitions
      
    case YX_ORIGIN_TO_ALL_PARTITIONS:
    case YX_ALL_NODES_TO_ALL_PARTITIONS:
    case YX_ALL_PARTITIONS_ON_ORIGIN_TO_ALL_PARTITIONS:
      vSendToAllPartitions(pSendOp);
      break;
    
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // PX cases which send this parcel to all partitions on origin
      
    case YX_ORIGIN_TO_ALL_PARTITIONS_ON_ORIGIN:
      vSendToAllOriginPartitions(pSendOp);
      break;
     
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // PX cases which send this parcel to a specific partition
      
    case YX_ORIGIN_TO_SPECIFIC_PARTITION: {
      RASSERT(XK_UNICAST == m_eSendKind);
      vSendToPartition_unicast(pSendOp);
      break;
    }
      
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // PX cases which send this parcel to ???? ALKA
      
    case YX_ALL_PARTITIONS_TO_ALL_NODES: {
      vSendToNode(pSendOp);
      break;
    }
      
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // PX cases which send this parcel to ???? ALKA
      
    case YX_ALL_PARTITIONS_TO_ALL_PARTITIONS: {

      if (m_eSendKind == XK_BROADCAST) {
        vSendToAllPartitions(pSendOp);
      }
      else {
        if (pGetOperand()->bIsBuffer()) {
          if (XK_HASHCAST == m_eSendKind)
            vSendToPartition_hashcast(pSendOp);
          else
          if (XK_UNICAST == m_eSendKind)
            vSendToPartition_unicast(pSendOp);
          else
          if (XK_ROBIN == m_eSendKind)
            vSendToPartition_robin(pSendOp);
          else
            NEVER_REACH;
        }
        else {
          vSendToAllPartitions(pSendOp);
        }
      }

      break;
    }
     
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // PX cases which send this parcel to all nodes
      
    case YX_ORIGIN_TO_ALL_NODES: {
      vSendToAllNodes(pSendOp);
      break;
    }
      
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      
    default:
      RASSERT(0);
  }
}

/*----------------------------------------------------------------------------*/
/* send this parcel to all nodes                                              */
/*----------------------------------------------------------------------------*/
void Parcel::vSendToAllNodes(Op* pSendOp)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARCEL,_FOOTER_);

  Pipeline* pPipeline = pSendOp->pPipeline_op();
  i8 i_NextStage = pSendOp->iNextStage();

  // get the next stage
  Stage* pNextStage = pPipeline->pGet(i_NextStage);
  RASSERT(pNextStage);

  //vTrace("Num of nodes:%lld\n"_Stz, pNextStage->nItems());

  //for (i8 iDstNode = 0; iDstNode < pNextStage->nItems(); iDstNode++)
    //vSendParcel(pSendOp,iDstNode,C_UNKNOWN);

  i8 iDstNode = 0;

  PtnId tPtnId;
  tPtnId = C_UNKNOWN;
  for (Node_I oNode_I; oNode_I.bMore(); oNode_I.vNext()) {
    Node* pNode = (*oNode_I);
    iDstNode = pNode->iNodeNo();
    vSendParcel(pSendOp, iDstNode, tPtnId);
  }
}

/*----------------------------------------------------------------------------*/
/* send this parcel to all partitions on origin                               */
/*----------------------------------------------------------------------------*/
void Parcel::vSendToAllOriginPartitions(Op* pSendOp)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARCEL,_FOOTER_);

  PtnId tPtnId;
  
  Pipeline* pPipeline = pSendOp->pPipeline_op();
  i8 i_NextStage = pSendOp->iNextStage();

  
  // get the next stage
  Stage* pNextStage = pPipeline->pGet(i_NextStage);
  RASSERT(pNextStage);
    
  // check the SendKind
  switch (m_eSendKind) {
      
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    case XK_ROBIN : {
      i8 i_Robin = pSendOp->iRobinNext();
      tPtnId = AM::PTNMAP::tRobinToNdePtnNo(i_Robin,pPipeline->i_Origin());
      i8 iDstNodeNo = AM::pPtnMap()->iWhichNodeNo(tPtnId);
      pSendOp->vSendNode(iDstNodeNo);

      RASSERT(iDstNodeNo == pPipeline->i_Origin());
      vSendParcel(pSendOp,iDstNodeNo,tPtnId);

      break;
    }
      
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    case XK_UNICAST: {
      
      ASSERT(pNextStage->nItems() == AM::PTNMAP::nPtnsPerNode());
      for (i8 i_Stream = 0; i_Stream < pNextStage->nItems(); i_Stream++) {
        
        Stream* pStream = pNextStage->pGet(i_Stream);
        tPtnId = pStream->tPtnId();
        
        // sendNode is the partition node
    
        i8 iDstNode = AM::pPtnMap()->iWhichNodeNo(tPtnId);
        pSendOp->vSendNode(iDstNode);
        if ( iDstNode == pPipeline->i_Origin())
          vSendParcel(pSendOp,iDstNode,tPtnId);
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
void Parcel::vSendToAllOriginPartitions(Op* pSendOp)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARCEL,_FOOTER_);

  PtnId tPtnId;
  i8 i_TablePtn;
  
  Pipeline* pPipeline = pSendOp->pPipeline();
  i8 i_NextStage = pSendOp->iNextStage();

  
  // get the next stage
  Stage* pNextStage = pPipeline->pGet(i_NextStage);
  RASSERT(pNextStage);
    
  // check the SendKind
  switch (m_eSendKind) {
      
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    case XK_ROBIN : {
      i8 i_Robin = pSendOp->iRobinNext();
      tPtnId = AM::PTNMAP::tRobinToDtdPtnNo(i_Robin);
      i8 iDstNodeNo = AM::pPtnMap()->iWhichNodeNo(tPtnId);

      pSendOp->vSendNode(iDstNodeNo);

      if (iDstNodeNo == pPipeline->i_Origin())
        vSendParcel(pSendOp,iDstNodeNo,tPtnId);

      break;
    }
      
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    case XK_UNICAST: {
      
      ASSERT(pNextStage->nItems() == AM::PTNMAP::nPtnsPerNode());
      for (i8 i_Stream = 0; i_Stream < pNextStage->nItems(); i_Stream++) {
        
        Stream* pStream = pNextStage->pGet(i_Stream);
        tPtnId = pStream->tPtnId();
        
        // sendNode is the partition node
    
        i8 iDstNode = AM::pPtnMap()->iWhichNodeNo(tPtnId);
        pSendOp->vSendNode(iDstNode);
        if ( iDstNode == pPipeline->i_Origin())
          vSendParcel(pSendOp,iDstNode,tPtnId);
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
void Parcel::vSendToAllPartitions(Op* pSendOp)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARCEL,_FOOTER_);
  PtnId tPtnId;
  
  Operand* pOperand = pGetOperand();
  
  Pipeline* pPipeline = pSendOp->pPipeline_op();
  i8 i_NextStage = pSendOp->iNextStage();
  
  // get the next stage
  Stage* pNextStage = pPipeline->pGet(i_NextStage);
  RASSERT(pNextStage);
  //vTrace("SendKind:%s\n"_Stz, aXK[m_eSendKind]);
  switch (m_eSendKind) {

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    case XK_BROADCAST : {
      ASSERT(pNextStage->nItems() == AM::PTNMAP::nPtnsInGrid());

      for (i8 iPtnNo = 0; iPtnNo < pNextStage->nItems(); iPtnNo++) {
        tPtnId = iPtnNo;
        // sendNode is the partition node
        i8 iDstNode = AM::pPtnMap()->iWhichNodeNo(tPtnId);
        pSendOp->vSendNode(iDstNode);
        vSendParcel(pSendOp,iDstNode,tPtnId);
      }

      break;
    }

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    case XK_HASHCAST : {
      DataBuffer* pDataBuffer = _pDataBuffer(pOperand);
      tPtnId = pDataBuffer->tPtnId();
      
      // sendNode is the partition node
      i8 iDstNode = AM::pPtnMap()->iWhichNodeNo(tPtnId);
      pSendOp->vSendNode(iDstNode);

      vSendParcel(pSendOp,iDstNode,tPtnId);
      break;
    }
      
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    case XK_LOCALROBIN : {
      i8 i_LocalRobin = pSendOp->iLocalRobinNext();
      tPtnId = AM::PTNMAP::tRobinToLclPtnNo(i_LocalRobin);
      
      // sendNode is this node
      i8 iDstNode = AM::pPtnMap()->iWhichNodeNo(tPtnId);
      pSendOp->vSendNode(iDstNode);
      RASSERT(iDstNode == AM::pGrid()->iThisNodeNo());
      
      vSendParcel(pSendOp,iDstNode,tPtnId);
      break;
    }
      
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    case XK_UNICAST: {
      ASSERT(pNextStage->nItems() == AM::PTNMAP::nPtnsInGrid());

      for (i8 iPtnNo = 0; iPtnNo < pNextStage->nItems(); iPtnNo++) {
        tPtnId = iPtnNo;
        // sendNode is the partition node
        i8 iDstNode = AM::pPtnMap()->iWhichNodeNo(tPtnId);
        pSendOp->vSendNode(iDstNode);
        vSendParcel(pSendOp,iDstNode,tPtnId);
      }
      break;
    }
      
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    case XK_ROBIN : {
      i8 i_Robin = pSendOp->iRobinNext();
      tPtnId = AM::PTNMAP::tRobinToDtdPtnId(i_Robin);
      i8 iDstNodeNo = AM::pPtnMap()->iWhichNodeNo(tPtnId);

      pSendOp->vSendNode(iDstNodeNo);

      vSendParcel(pSendOp,iDstNodeNo,tPtnId);
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
void Parcel::vSendToNode(Op* pSendOp)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARCEL,_FOOTER_);
  
  i8 iDstNode;
  
  Operand* pOperand = pGetOperand();
  if (CN_METAGET == pOperand->eGetClassNo()) {
    iDstNode = 6078; // HACK  hardcode to the existing node for debug
  }
  else {
    PtnId tSendingPtn = pSendOp->tPtnId();
    iDstNode = AM::pPtnMap()->iWhichNodeNo(tSendingPtn);               // WRONG!

  }
  
#ifdef WITH_BKTMAP_2N4P
  vTrace("vSendToNode iDstNode:%lld\n"_Stz, iDstNode);
#endif

  
  Pipeline* pPipeline = pSendOp->pPipeline_op();
  i8 i_NextStage = pSendOp->iNextStage();
  
  // get the next stage
  Stage* pNextStage = pPipeline->pGet(i_NextStage);
  RASSERT(pNextStage);
  
  ASSERT(pNextStage->nItems() == AM::pGrid()->nKnownNodes());

  PtnId tPtnId;
  tPtnId = C_UNKNOWN;
  vSendParcel(pSendOp,iDstNode,tPtnId);
    
}
/*----------------------------------------------------------------------------*/
//void Parcel::vSendToNode(Op* pSendOp, i8 iDstNode)
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_PARCEL,_FOOTER_);
//#ifdef WITH_BKTMAP_2N4P
//  vTrace("vSendToNode iDstNode:%lld\n"_Stz, iDstNode);
//#endif
//  Pipeline* pPipeline = pSendOp->pPipeline_op();
//  i8 i_NextStage = pSendOp->iNextStage();
//
//  // get the next stage
//  Stage* pNextStage = pPipeline->pGet(i_NextStage);
//  RASSERT(pNextStage);
//
//  ASSERT(pNextStage->nItems() == AM::pGrid()->nKnownNodes());
//
//  PtnId tPtnId;
//  tPtnId = C_UNKNOWN;
//  vSendParcel(pSendOp,iDstNode,tPtnId);
//}

/*----------------------------------------------------------------------------*/
/* send this parcel to origin                                                 */
/*----------------------------------------------------------------------------*/
void Parcel::vSendToOrigin(Op* pSendOp)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARCEL,_FOOTER_);

  Pipeline* pPipeline = pSendOp->pPipeline_op();

  i8 iDstNode = pPipeline->i_Origin();
  pSendOp->vSendNode(iDstNode);

  PtnId tPtnId;
  tPtnId = C_UNKNOWN;
  vSendParcel(pSendOp, iDstNode, tPtnId);
}

/*----------------------------------------------------------------------------*/
/* send this parcel to given partition                                        */
/*----------------------------------------------------------------------------*/
void Parcel::vSendToPartition_hashcast(Op* pSendOp)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARCEL,_FOOTER_);
  
  Operand* pOperand = pGetOperand();
  DataBuffer* pDataBuffer = nullptr;

  pDataBuffer = _pDataBuffer(pOperand);
  PtnId tDstPtn = pDataBuffer->tPtnId();
  i8 iDstNode = AM::pPtnMap()->iWhichNodeNo(tDstPtn);
  
  pSendOp->vSendNode(iDstNode);
  vSendParcel(pSendOp,iDstNode,tDstPtn);
}


/*----------------------------------------------------------------------------*/
/* send this parcel to destination by round robin of partition                */
/*----------------------------------------------------------------------------*/
void Parcel::vSendToPartition_robin(Op* pSendOp)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARCEL,_FOOTER_);

  
  i8 i_Robin = pSendOp->iRobinNext();
  PtnId tDstPtn = AM::PTNMAP::tRobinToDtdPtnId(i_Robin);
  i8 iDstNode = AM::pPtnMap()->iWhichNodeNo(tDstPtn);
  
  pSendOp->vSendNode(iDstNode);
  vSendParcel(pSendOp,iDstNode,tDstPtn);
}

/*----------------------------------------------------------------------------*/
/* send this parcel to given partition                                        */
/*----------------------------------------------------------------------------*/
void Parcel::vSendToPartition_unicast(Op* pSendOp)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARCEL,_FOOTER_);
    
  
  
  PtnId tSendingPtn = pSendOp->tPtnId();

  Pipeline* pPipeline = pSendOp->pPipeline_op();
  i8 i_NextStage = pSendOp->iNextStage();
  Stage* pNextStage = pPipeline->pGet(i_NextStage);
  Stream* pStream = pNextStage->pGetStream_byPtnNo(tSendingPtn.iAsI8());
  PtnId tDstPtn = pStream->tPtnId();

  
  
  
  
  i8 iDstNode = AM::pPtnMap()->iWhichNodeNo(tDstPtn);
  pSendOp->vSendNode(iDstNode);
  vSendParcel(pSendOp,iDstNode,tDstPtn);
}

/*----------------------------------------------------------------------------*/
/* set item in array                                                          */
/*----------------------------------------------------------------------------*/
void Parcel::vSetItem(i8 i_Item, Object* pItem)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARCEL,_FOOTER_);

  m_pItems[i_Item] = pItem;
}



/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* send parcel to given node,partition                                        */
/*----------------------------------------------------------------------------*/
void Parcel::vSendParcel(Op* pSendOp, i8  iDstNode, PtnId tDstPtn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARCEL,_FOOTER_);

  m_iDstNode = iDstNode;
  m_tDstPtn   = tDstPtn;
  
  m_iDstStage = pSendOp->iNextStage();
  m_iDstOp    = pSendOp->iNextOp();
  
  vSocketSendOfParcel(pSendOp);

  // - - - - - - - - - - - - - - - -
  
  i8 i_OtherStage = pSendOp->iOtherStage();
  i8 i_OtherOp    = pSendOp->iOtherOp();
  
  if (C_UNKNOWN != i_OtherStage) {

    m_iDstStage = i_OtherStage;
    m_iDstOp    = i_OtherOp;

    vSocketSendOfParcel(pSendOp);
  }
}


/*----------------------------------------------------------------------------*/
/* send parcel (and its contents) out over socket                             */
/*----------------------------------------------------------------------------*/
void Parcel::vSocketSendOfParcel(Op* pSendOp)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARCEL,_FOOTER_);
  
  bool bResult;
  i8 nObjectSize;
  SysCall oSysCall;
  Object* pObject;
  Parcel* pParcel;

  // get preliminary information
  Node* pDstNode = AM::pGrid()->pFindNode(m_iDstNode);
  //vTrace("vSocketSendOfParcel prepared to send to Node:%lld Port:%lld \n"_Stz,pDstNode->iNodeNo(), pDstNode->iPortNo());
  Socket* pSocket = pDstNode->pSendSocket();
  i8 nObjects = this->nItems();
  
  pSocket->vGrabWrite();

  //vTrace("vSocketSendOfParcel got a lock, prepared to send %lld Object(s)to Node:%lld Port:%lld \n"_Stz,nObjects, pDstNode->iNodeNo(), pDstNode->iPortNo());

  // send the parcel instance
  pParcel = this;
  nObjectSize = gxaSizeHeap[pParcel->eGetClassNo()].m_nClassSize;
  bResult = pSocket->bSendTo(pParcel, nObjectSize, oSysCall);

  //if (!bResult) {
  //  vTrace("vSocketSendOfParcel SENT FAILED\n"_Stz);
  //} else {
  //  vTrace("vSocketSendOfParcel SENT %lld bytes Parcel:%s to Node:%lld Port:%lld \n"_Stz,nObjectSize, aCN[pParcel->pGetOperand()->eGetClassNo()], pDstNode->iNodeNo(), pDstNode->iPortNo());
  //}

  // send all the objects within the parcel instance
  for (i8 i_Item = 0; i_Item < nObjects; i_Item++) {
    pObject = pParcel->pGetItem(i_Item);
    eCN eClassNo = pObject->eGetClassNo();
    nObjectSize = gxaSizeHeap[eClassNo].m_nClassSize;

    if (CN_IDXVEC == eClassNo) {
      i8 iSize1 = gxaSizeHeap[CN_INDEX].m_nClassSize;
      i8 iSize2 = gxaSizeHeap[CN_IDXVEC].m_nClassSize;
      vTrace("vSocketSendOfParcel SENT IdxVec: %lld bytes Index:%lld bytes \n"_Stz,iSize2, iSize1);

      //nObjectSize = gxaSizeHeap[pObject->eGetClassNo()].m_nClassSize + (gxaSizeHeap[CN_INDEX].m_nClassSize * 2);
    }

    if (CN_CRWBUFFER == eClassNo && AM::TRACE::bVerbose(C_VB_SEND_CRWBUF)) {
      CrwBuffer* pCrwBuffer = _pCrwBuffer(pObject);
      vTrace("Sending a CrwBuffer (%lld rows) to node %s tPtnId %lld\n"_Stz
             ,pCrwBuffer->nRows()
             ,pDstNode->szNodeNm()
             ,pCrwBuffer->tPtnId().iAsI8()
             );
    }

    bResult = pSocket->bSendTo(pObject, nObjectSize, oSysCall);
    //vTrace("vSocketSendOfParcel SENT %lld bytes class:%s  \n"_Stz,nObjectSize, aCN[pObject->eGetClassNo()]);

  }
  
  pSocket->vDropWrite();

  s256 sSupplemental;
  
  sSupplemental.vSprint("%s %s"_Stz,pDstNode->szNodeNm(),aXK[m_eSendKind]);
  
  pSendOp->vOpTrace(this,(Stz*)__func__,true,sSupplemental);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
