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

  m_eIntraTopology = YO_NONE;
  m_eCrossTopology = YX_NONE;
  m_eSendKind      = XK_NONE;

  m_iDstNode       = C_UNKNOWN;
  m_iDstStage      = C_UNKNOWN;
  m_tDstPtn        = C_UNKNOWN;
  
  m_bOriginalParcel = false;
  
  m_iPipelineJobId = C_UNKNOWN;

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
  
  m_eCrossTopology = eCrossTopology;
  m_eIntraTopology = eIntraTopology;
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

      if (m_eSendKind == XK_BROADCAST)
        vSendToAllPartitions(pSendOp);
      else
      if (pGetOperand()->bIsBuffer())
        vSendToPartition(pSendOp);
      else
        vSendToAllPartitions(pSendOp);
        
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
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* send this parcel to all nodes                                              */
/*----------------------------------------------------------------------------*/
void Parcel::vSendToAllNodes(Op* pSendOp)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARCEL,_FOOTER_);

  PtnId tLogicalPtn = C_UNKNOWN;
  
  // assert origin
  Pipeline* pPipeline = pSendOp->pPipeline_op();
  i8 iOriginNode = pPipeline->i_Origin();
  RASSERT(iOriginNode == MY::pNode()->iNodeNo());
  
  // get the next stage
  Stage* pNextStage = pSendOp->pNextStage();
  i8 i_NextStage = pNextStage->iStage();

  RASSERT(m_eCrossTopology == YX_ORIGIN_TO_ALL_NODES);

  // on any node, send this parcel to
  //   next stage of pipeline on every node
  //   i.e. { X node , nextStage , ndePtnStream }
    
  for (Node_I oNode_I; oNode_I.bMore(); oNode_I.vNext()) {
    Node* pNode = (*oNode_I);
    
    i8 iNde = pNode->iNodeNo();
    tLogicalPtn.vNodeNo(iNde);
    
    pSendOp->vSendNode(iNde);
    vSendParcel(pSendOp,iNde,i_NextStage,tLogicalPtn);
  }
}

/*----------------------------------------------------------------------------*/
/* send this parcel to all partitions on origin                               */
/*----------------------------------------------------------------------------*/
void Parcel::vSendToAllOriginPartitions(Op* pSendOp)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARCEL,_FOOTER_);

  i8 iMappedNde;
  PtnId tLogicalPtn;
  Stream* pStream;

  // assert origin
  Pipeline* pPipeline = pSendOp->pPipeline_op();
  i8 iOriginNode = pPipeline->i_Origin();
  RASSERT(iOriginNode == MY::pNode()->iNodeNo());
  
  // get the next stage
  Stage* pNextStage = pSendOp->pNextStage();
  i8 i_NextStage = pNextStage->iStage();
  i8 nLogicalPtns = pNextStage->nItems();


  RASSERT(m_eCrossTopology == YX_ORIGIN_TO_ALL_PARTITIONS_ON_ORIGIN);
  
  switch (m_eSendKind) {
      
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    case XK_ROBIN : {
      
       
      // on origin, send this parcel to
      //   one partition-stream (by robin) of the next stage on this origin
      //   i.e. { origin , nextStage , ndePtnStream }

      i8 i_Robin = pSendOp->iRobinNext();
      tLogicalPtn = AM::PTNMAP::tRobinToNdePtnNo(i_Robin,iOriginNode);
      
      iMappedNde = AM::pPtnMap()->iWhichNodeNo(tLogicalPtn);
      RASSERT(iMappedNde == iOriginNode);
      
      pSendOp->vSendNode(iMappedNde);
      vSendParcel(pSendOp,iMappedNde,i_NextStage,tLogicalPtn);

      break;
    }
      
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    case XK_UNICAST: {                             // SHOULD THIS BE BROADCAST ?
            
      // on origin, send this parcel to
      //   all partition-streams of the next stage on this origin
      //   i.e. { origin , nextStage , X ndePtnStream }
      
      for (i8 i_Stream = 0; i_Stream < nLogicalPtns; i_Stream++) {
        pStream = pNextStage->pGet(i_Stream);
        
        tLogicalPtn = pStream->tPtnId();
        iMappedNde = AM::pPtnMap()->iWhichNodeNo(tLogicalPtn);
        RASSERT(iMappedNde == iOriginNode);

        pSendOp->vSendNode(iMappedNde);
        vSendParcel(pSendOp,iMappedNde,i_NextStage,tLogicalPtn);
      }
      
      break;
    }
      
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    default:
      ASSERT(0);
  }
}

/*----------------------------------------------------------------------------*/
/* send this parcel to all partitions                                         */
/*----------------------------------------------------------------------------*/
void Parcel::vSendToAllPartitions(Op* pSendOp)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARCEL,_FOOTER_);
  
  i8 iMappedNde;
  PtnId tLogicalPtn;
  Stream* pStream;
  
  // get the next stage
  Stage* pNextStage = pSendOp->pNextStage();
  i8 i_NextStage = pNextStage->iStage();
  i8 nLogicalPtns = pNextStage->nItems();
  
  RASSERT(   m_eCrossTopology == YX_ORIGIN_TO_ALL_PARTITIONS
          || m_eCrossTopology == YX_ALL_NODES_TO_ALL_PARTITIONS
          || m_eCrossTopology == YX_ALL_PARTITIONS_TO_ALL_PARTITIONS
          || m_eCrossTopology == YX_ALL_PARTITIONS_ON_ORIGIN_TO_ALL_PARTITIONS);

  switch (m_eSendKind) {

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    case XK_BROADCAST : {
      
      // on this node, send this parcel to
      //   all partition-streams of the next stage on any node
      //   i.e. parcel => { thisNode , nextStage , X ndePtnStream }

      for (i8 i_Stream = 0; i_Stream < nLogicalPtns; i_Stream++) {
        pStream = pNextStage->pGet(i_Stream);
        
        tLogicalPtn = pStream->tPtnId();
        iMappedNde = AM::pPtnMap()->iWhichNodeNo(tLogicalPtn);

        pSendOp->vSendNode(iMappedNde);
        vSendParcel(pSendOp,iMappedNde,i_NextStage,tLogicalPtn);
      }

      break;
    }

    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    case XK_HASHCAST : {
      
      // on this node, send this parcel to
      //   the partition-stream of the next stage on buffer forward node
      //   i.e. parcel => { mappedNode , nextStage , ndePtnStream of buffer }
      
      Operand* pOperand = pGetOperand();
      DataBuffer* pDataBuffer = _pDataBuffer(pOperand);
      
      tLogicalPtn = pDataBuffer->tPtnId();
      iMappedNde = AM::pPtnMap()->iWhichNodeNo(tLogicalPtn);
      
      pSendOp->vSendNode(iMappedNde);
      vSendParcel(pSendOp,iMappedNde,i_NextStage,tLogicalPtn);
      
      break;
    }
      
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    case XK_LOCALROBIN : {
      
      // on this node, send this parcel to
      //   one partition-stream (by robin) of the next stage on this node
      //   i.e. parcel => { thisNode , nextStage , rr ndePtnStream }

      i8 i_LocalRobin = pSendOp->iLocalRobinNext();
      tLogicalPtn = AM::PTNMAP::tRobinToLclPtnStream(i_LocalRobin);
      
      iMappedNde = AM::pPtnMap()->iWhichNodeNo(tLogicalPtn);
      RASSERT(iMappedNde == MY::pNode()->iNodeNo());

      pSendOp->vSendNode(iMappedNde);
      vSendParcel(pSendOp,iMappedNde,i_NextStage,tLogicalPtn);
      
      break;
    }
      
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    case XK_UNICAST: {
      
      // on this node, send this parcel to
      //   all streams of the next stage on every node
      //

      for (tLogicalPtn = 0; tLogicalPtn < nLogicalPtns; ++tLogicalPtn) {
        
        iMappedNde = AM::pPtnMap()->iWhichNodeNo(tLogicalPtn);
        
        pSendOp->vSendNode(iMappedNde);
        vSendParcel(pSendOp,iMappedNde,i_NextStage,tLogicalPtn);
      }
      
      break;
    }
      
    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    case XK_ROBIN : {
      
      // on node, send this parcel to
      //   one partition-stream (by robin) of the next stage on robin node
      //   i.e. parcel => { rr node , nextStage , rr ndePtnStream }

      i8 i_Robin = pSendOp->iRobinNext();
      tLogicalPtn = AM::PTNMAP::tRobinToDtdPtnStream(i_Robin);
      
      iMappedNde = AM::pPtnMap()->iWhichNodeNo(tLogicalPtn);

      pSendOp->vSendNode(iMappedNde);
      vSendParcel(pSendOp,iMappedNde,i_NextStage,tLogicalPtn);
      
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
  
  // from this node, send this parcel to
  //   the next stage on node mapped to given partition
  //   i.e. parcel => { mappedNode , nextStage , N/A }

  Stage* pNextStage = pSendOp->pNextStage();
  i8 i_NextStage = pNextStage->iStage();
  
  i8 iMappedNde;
  
  Operand* pOperand = pGetOperand();
  if (CN_METAGET == pOperand->eGetClassNo()) {
    iMappedNde = 6078; // HACK  hardcode to the existing node for debug
  }
  else {
    PtnId tSendToPtn = pSendOp->tPtnId();
    iMappedNde = AM::pPtnMap()->iWhichNodeNo(tSendToPtn);
  }
  
#ifdef WITH_BKTMAP_2N4P
  vTrace("vSendToNode iDstNode:%lld\n"_Stz, iMappedNde);
#endif

  PtnId tLogicalPtn = C_UNKNOWN;
  vSendParcel(pSendOp,iMappedNde,i_NextStage,tLogicalPtn);
}

/*----------------------------------------------------------------------------*/
/* send this parcel to origin                                                 */
/*----------------------------------------------------------------------------*/
void Parcel::vSendToOrigin(Op* pSendOp)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARCEL,_FOOTER_);

  // from this node, send this parcel to
  //   the next stage on origin node
  //   i.e. parcel => { originNode , nextStage , N/A }
  
  Pipeline* pPipeline = pSendOp->pPipeline_op();
  i8 iOriginNde = pPipeline->i_Origin();
  
  Stage* pNextStage = pSendOp->pNextStage();
  i8 i_NextStage = pNextStage->iStage();
  
  PtnId tLogicalPtn = C_UNKNOWN;
  
  pSendOp->vSendNode(iOriginNde);
  vSendParcel(pSendOp,iOriginNde,i_NextStage,tLogicalPtn);
}

/*----------------------------------------------------------------------------*/
/* send this parcel to given partition                                        */
/*----------------------------------------------------------------------------*/
void Parcel::vSendToPartition(Op* pSendOp)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARCEL,_FOOTER_);
  
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

/*----------------------------------------------------------------------------*/
/* send this parcel to given partition                                        */
/*----------------------------------------------------------------------------*/
void Parcel::vSendToPartition_hashcast(Op* pSendOp)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARCEL,_FOOTER_);
  
  // from this node, send this parcel to
  //   the partition-stream of the next stage on buffer forward node
  //   i.e. parcel => { mappedNode , nextStage , ndePtnStream of buffer }

  Stage* pNextStage = pSendOp->pNextStage();
  i8 i_NextStage = pNextStage->iStage();

  Operand* pOperand = pGetOperand();
  DataBuffer* pDataBuffer = _pDataBuffer(pOperand);
  
  PtnId tLogicalPtn = pDataBuffer->tPtnId();
  i8 iMappedNde = AM::pPtnMap()->iWhichNodeNo(tLogicalPtn);
  
  pSendOp->vSendNode(iMappedNde);
  vSendParcel(pSendOp,iMappedNde,i_NextStage,tLogicalPtn);
}


/*----------------------------------------------------------------------------*/
/* send this parcel to destination by round robin of partition                */
/*----------------------------------------------------------------------------*/
void Parcel::vSendToPartition_robin(Op* pSendOp)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARCEL,_FOOTER_);
  i8 i_NextStage = pSendOp->iNextStage();

  
  i8 i_Robin = pSendOp->iRobinNext();
  PtnId tDstPtn = AM::PTNMAP::tRobinToDtdPtnStream(i_Robin);
  i8 iDstNode = AM::pPtnMap()->iWhichNodeNo(tDstPtn);
  
  pSendOp->vSendNode(iDstNode);
  vSendParcel(pSendOp,iDstNode,i_NextStage,tDstPtn);
}

/*----------------------------------------------------------------------------*/
/* send this parcel to given partition                                        */
/*----------------------------------------------------------------------------*/
void Parcel::vSendToPartition_unicast(Op* pSendOp)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARCEL,_FOOTER_);
    
  i8 i_NextStage = pSendOp->iNextStage();

  
  PtnId tSendingPtn = pSendOp->tPtnId();

  Pipeline* pPipeline = pSendOp->pPipeline_op();
  Stage* pNextStage = pPipeline->pGet(i_NextStage);
  Stream* pStream = pNextStage->pGetStream_byPtnNo(tSendingPtn.iAsI8());
  PtnId tDstPtn = pStream->tPtnId();

  
  
  
  
  i8 iDstNode = AM::pPtnMap()->iWhichNodeNo(tDstPtn);
  pSendOp->vSendNode(iDstNode);
  vSendParcel(pSendOp,iDstNode,i_NextStage,tDstPtn);
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
void Parcel::vSendParcel(Op* pSendOp,
                         i8 iDstNode, i8 iDstStage, PtnId tDstLogicalPtn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PARCEL,_FOOTER_);

  //RASSERT(iDstNode == AM::pPtnMap()->iWhichNodeNo(tDstPtn));

  m_iDstNode  = iDstNode;
  m_iDstStage = iDstStage;
  m_tDstPtn   = tDstLogicalPtn;
  
  
  vSocketSendOfParcel(pSendOp);

  // - - - - - - - - - - - - - - - -
  
  i8 i_OtherStage = pSendOp->iOtherStage();
  if (C_UNKNOWN != i_OtherStage) {
    m_iDstStage = i_OtherStage;
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
