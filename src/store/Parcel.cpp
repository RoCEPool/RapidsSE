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
#include "StageSpec.hpp"
#include "Stream.hpp"
#include "PtnMap.hpp"
#include "Node.hpp"
#include "Grid.hpp"
#include "Socket.hpp"
#include "Node_I.hpp"

#include "TagConstants.h"

#include "CrwBuffer.hpp"
#include "LogicUnit.hpp"


extern SizeHeap gxaSizeHeap[];

class Table;
/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, Parcel, CN_PARCEL)

__aNewFunction(Parcel, CN_PARCEL)

__vDeleteFunction(Parcel, CN_PARCEL)

__aFooterFunction(Parcel)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Parcel::Parcel(void)
: Operand(CN_PARCEL)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PARCEL, _FOOTER_);

  m_iPipelineJobId = C_UNKNOWN;

  m_iDstNode = C_UNKNOWN;
  m_iDstStage = C_UNKNOWN;
  m_tDstPtn = C_UNKNOWN;

  m_iSrcNode = C_UNKNOWN;
  m_iCodeLine = C_UNKNOWN;

  m_eParcelForward = YF_NONE;
  m_eCrossTopology = YX_NONE;
  m_eSendCast = XK_NONE;

  m_bOriginalParcel = false;
      
  m_bDeleteOperand = true;
  m_eRecvInstructions = RV_NONE;

  m_nItems = 0;
  for (i8 i_Item = 0; i_Item < C_MAX_PARCEL_ITEMS; i_Item++)
    m_paItems[i_Item] = nullptr;

  _checkInitialised_(CN_PARCEL);

  _postcondition_(_ADDRESS_, _MAGIC_, CN_PARCEL, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Parcel::~Parcel(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PARCEL, _FOOTER_);  
}


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* post recv function                                                         */
/*----------------------------------------------------------------------------*/
void Parcel::vPostRecv(void *aParcel)
{
  _static_

  Parcel *pParcel = _pParcel(aParcel);

  for (i8 i_Item = 0; i_Item < C_MAX_PARCEL_ITEMS; i_Item++)
    pParcel->m_paItems[i_Item] = nullptr;
}

/*----------------------------------------------------------------------------*/
/* repair the parcel                                                          */
/*----------------------------------------------------------------------------*/
void Parcel::vRepair(Object *pObject)
{
  _static_

  Parcel *pParcel = _pParcel(pObject);

  // delete the parcel
  delete pParcel;
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Add object to array                                                        */
/*----------------------------------------------------------------------------*/
void Parcel::vAddItem(Object *pItem)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PARCEL, _FOOTER_);

  if (m_nItems < C_MAX_PARCEL_ITEMS) {
    m_paItems[m_nItems++] = pItem;
  }
}

/*----------------------------------------------------------------------------*/
/* get cross topology                                                         */
/*----------------------------------------------------------------------------*/
eYX Parcel::eCrossTopology(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PARCEL, _FOOTER_);

  return m_eCrossTopology;
}

/*----------------------------------------------------------------------------*/
/* get destination node                                                       */
/*----------------------------------------------------------------------------*/
i8 Parcel::iDstNode(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PARCEL, _FOOTER_);

  return m_iDstNode;
}

/*----------------------------------------------------------------------------*/
/* get destination ptn                                                        */
/*----------------------------------------------------------------------------*/
PtnId Parcel::tDstPtn(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PARCEL, _FOOTER_);

#if 0
  Operand *pOperand = pGetOperand();
  if (CN_CRWBUFFER == pOperand->eGetClassNo()) {

    CrwBuffer *pBuffer = (CrwBuffer *) m_paItems[0];
    // Get the bucket number and partition number from buffer
    vTrace("Parcel::tDstPtn CN_CRWBUFFER bkt:%lld  Ptn:%lld\n"_Stz, pBuffer->iBucketNo(), (i8) pBuffer->tPtnId().iAsI8());

    //m_tDstPtn = pBuffer->iBucketNo();
  }
#endif
  //vTrace("Parcel::tDstPtn  Ptn:%lld\n"_Stz, (i8) m_tDstPtn.iAsI8());
  return m_tDstPtn;
}


/*----------------------------------------------------------------------------*/
/* get destination stage                                                      */
/*----------------------------------------------------------------------------*/
i8 Parcel::iDstStage(void) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_PARCEL, _FOOTER_);

  return m_iDstStage;
}

/*----------------------------------------------------------------------------*/
/* Get an object from array                                                   */
/*----------------------------------------------------------------------------*/
Object *Parcel::pGetItem(i8 i_Item)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PARCEL, _FOOTER_);

  if (i_Item < C_MAX_PARCEL_ITEMS && i_Item < m_nItems)
    return m_paItems[i_Item];

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* get Operand                                                                */
/*----------------------------------------------------------------------------*/
Operand *Parcel::pGetOperand(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PARCEL, _FOOTER_);

  return (Operand *) m_paItems[0];
}

/*----------------------------------------------------------------------------*/
/* get number of objects in the parcel                                        */
/*----------------------------------------------------------------------------*/
i8 Parcel::nItems(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PARCEL, _FOOTER_);

  return m_nItems;
}

/*----------------------------------------------------------------------------*/
/* label this parcel                                                          */
/*----------------------------------------------------------------------------*/
void Parcel::vLabel(i8 iPipelineJobId, eYF eParcelForward, eYX eCrossTopology)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PARCEL, _FOOTER_);

  m_iPipelineJobId = iPipelineJobId;
  m_eParcelForward = eParcelForward;
  m_eCrossTopology = eCrossTopology;
  
  //RASSERT(0);
}

/*----------------------------------------------------------------------------*/
/* is this parcel an original (T or F                                         */
/*----------------------------------------------------------------------------*/
bool Parcel::bOriginalParcel(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PARCEL, _FOOTER_);
  return m_bOriginalParcel;
}

/*----------------------------------------------------------------------------*/
/* set the state of origin (T or F)                                           */
/*----------------------------------------------------------------------------*/
void Parcel::vOriginalParcel(bool bOriginalParcel)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PARCEL, _FOOTER_);

  m_bOriginalParcel = bOriginalParcel;
}

/*----------------------------------------------------------------------------*/
/* get job id of the pipeline job                                             */
/*----------------------------------------------------------------------------*/
i8 Parcel::iPipelineJobId(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PARCEL, _FOOTER_);

  return m_iPipelineJobId;
}

/*----------------------------------------------------------------------------*/
/* recv contents of this parcel from given socket                             */
/*----------------------------------------------------------------------------*/
void Parcel::vRecvContents(Socket *pSocket)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PARCEL, _FOOTER_);

  //vTrace("vRecvContents:\n"_Stz);
  for (i8 i_Item = 0; i_Item < m_nItems; i_Item++) {
    Object *pObject = pSocket->pRecvObject();
    m_paItems[i_Item] = pObject;
    //vTrace(" %s\n"_Stz, aCN[pObject->eGetClassNo()]);
  }
}

/*----------------------------------------------------------------------------*/
/* get receiver instructions                                                  */
/*----------------------------------------------------------------------------*/
eRV Parcel::eRecvInstructions(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PARCEL, _FOOTER_);

  return m_eRecvInstructions;
}

/*----------------------------------------------------------------------------*/
/* set receiver instructions                                                  */
/*----------------------------------------------------------------------------*/
void Parcel::vRecvInstructions(eRV eRecvInstructions)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PARCEL, _FOOTER_);

  m_eRecvInstructions = eRecvInstructions;
}


/*----------------------------------------------------------------------------*/
/* remove Operand                                                             */
/*----------------------------------------------------------------------------*/
Operand *Parcel::pRemoveOperand(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PARCEL, _FOOTER_);

  ASSERT(1 == m_nItems);
  
  m_nItems--;
  
  return (Operand *) m_paItems[0];
}

/*----------------------------------------------------------------------------*/
/* reset item count                                                           */
/*----------------------------------------------------------------------------*/
void Parcel::vReset(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PARCEL, _FOOTER_);

  m_nItems = 0;
}

/*----------------------------------------------------------------------------*/
/* send this parcel out                                                       */
/*----------------------------------------------------------------------------*/
void Parcel::vDistribute(Op *pSendOp)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PARCEL, _FOOTER_);

  RASSERT(pSendOp->szOpNm()->bSame("send"_Stz));

  vAppraiseCastKind(pSendOp);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Operand *pOperand = pGetOperand();
  if (CN_METAGET == pOperand->eGetClassNo()) {
    // Send metaGet to other node, not to the origin node itself
    vFwdTo_OnNode(pSendOp); // HACK  hardcode to the existing node for debug
    if (m_bDeleteOperand)
      AM::vDeleteOperand(pOperand);
    return;
  }
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  vFwdTo_Topology_cast(pSendOp);

  if (pOperand->eGetClassNo() == CN_TABLE) {
    vTrace(">>>>>> Parcel::vSendOut Tried to call vDeleteOperand() to delete this operand CN_TABLE\n"_Stz);
  }

  // delete the operand
  if (m_bDeleteOperand)
    AM::vDeleteOperand(pOperand);
}


/*----------------------------------------------------------------------------*/
/*-- Private Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* determine where/how this parcel is to be forwarded                         */
/*----------------------------------------------------------------------------*/
void Parcel::vAppraiseCastKind(Op *pSendOp)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PARCEL, _FOOTER_);

  // figure out the send kind
  m_eSendCast = pSendOp->eSendCast();

  // is the operand is a marker or a streamDef ?
  Operand *pOperand = pGetOperand();
  if (pOperand->bIsMarker() || pOperand->bIsStreamDef())
    m_eSendCast = XK_UNICAST;

  // is the operand an InfoSet ?
  if (pOperand->bIsInfoSet() || pOperand->bIsProfile())
    m_eSendCast = XK_ROBIN;
}

/*----------------------------------------------------------------------------*/
/* cross-topo send of this parcel                                             */
/*----------------------------------------------------------------------------*/
void Parcel::vFwdTo_Topology_cast(Op *pSendOp)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PARCEL, _FOOTER_);

  //vTrace("m_eParcelForward %s\n"_Stz,aYF[m_eParcelForward]);

  switch (m_eParcelForward) {


    case YF_ORIGIN:
      vFwdTo_OnOrigin(pSendOp);
      break;


    case YF_ALL_PARTITIONS_ON_ORIGIN:
      vFwdTo_OnAllPartitionsOnOrigin(pSendOp);
      break;

//    case YF_ALL_NODES:
//      vFwdTo_OnNode(pSendOp);
//      break;


    case YF_ALL_PARTITIONS:

      if (m_eSendCast == XK_BROADCAST)
        vFwdTo_OnAllPartitions(pSendOp);
      else if (pGetOperand()->bIsBuffer())
        vFwdBufferTo_OnePtn(pSendOp);
      else
        vFwdTo_OnAllPartitions(pSendOp);

      break;
  
    case YF_ALL_NODES:
      vFwdTo_OnAllNodes(pSendOp);
      break;
  
    case YF_NEXT_NODE:
      vFwdTo_OnNextNode(pSendOp);
      break;

    default:
      RASSERT(0);
  }
}

/*----------------------------------------------------------------------------*/
//void Parcel::vFwdTo_Topology_cast(Op *pSendOp)
//{
//  _precondition_(_ADDRESS_, _MAGIC_, CN_PARCEL, _FOOTER_);
//
//  //vTrace("m_eParcelForward %s\n"_Stz,aYF[m_eParcelForward]);
//
//  switch (m_eCrossTopology) {
//
//    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//    // PX cases which send this parcel to the origin
//
//    case YX_ALL_NODES_TO_ORIGIN:
//    case YX_ALL_PARTITIONS_TO_ORIGIN:
//    case YX_NEXT_NODE_TO_ORIGIN:
//    case YX_ORIGIN_TO_ORIGIN: {
//      RASSERT(YF_ORIGIN == m_eParcelForward);
//      vFwdTo_OnOrigin(pSendOp);
//      break;
//    }
//
//
//    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//    // PX cases which send this parcel to all partitions on origin
//
//    case YX_ORIGIN_TO_ALL_PARTITIONS_ON_ORIGIN:
//      RASSERT(YF_ALL_PARTITIONS_ON_ORIGIN == m_eParcelForward);
//      vFwdTo_OnAllPartitionsOnOrigin(pSendOp);
//      break;
//
//    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//    // PX cases which send this parcel to ???? ALKA
//
//    case YX_ALL_PARTITIONS_TO_ALL_NODES: {
//      RASSERT(YF_ALL_NODES == m_eParcelForward);
//      vFwdTo_OnNode(pSendOp);
//      break;
//    }
//
//    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//    // PX cases which send this parcel to ???? ALKA
//
//    case YX_ORIGIN_TO_ALL_PARTITIONS:
//    case YX_ALL_NODES_TO_ALL_PARTITIONS:
//    case YX_ALL_PARTITIONS_ON_ORIGIN_TO_ALL_PARTITIONS:
//
//    case YX_ALL_PARTITIONS_TO_ALL_PARTITIONS:
//      RASSERT(YF_ALL_PARTITIONS == m_eParcelForward);
//
//      if (m_eSendCast == XK_BROADCAST)
//        vFwdTo_OnAllPartitions(pSendOp);
//      else if (pGetOperand()->bIsBuffer())
//        vFwdBufferTo_OnePtn(pSendOp);
//      else
//        vFwdTo_OnAllPartitions(pSendOp);
//
//      break;
//
//
//    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//    // PX cases which send this parcel to all nodes
//
//    case YX_ALL_PARTITIONS_ON_ORIGIN_TO_ALL_NODES:
//    case YX_ORIGIN_TO_ALL_NODES: {
//      RASSERT(YF_ALL_NODES == m_eParcelForward);
//      vFwdTo_OnAllNodes(pSendOp);
//      break;
//    }
//
//    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//    // PX cases which send this parcel to next node
//
//    case YX_ORIGIN_TO_NEXT_NODE: {
//      RASSERT(YF_NEXT_NODE == m_eParcelForward);
//      vFwdTo_OnNextNode(pSendOp);
//      break;
//    }
//
//    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//
//    default:
//      RASSERT(0);
//  }
//}


/*----------------------------------------------------------------------------*/
//void Parcel::vFwdTo_Topology_cast(Op *pSendOp)
//{
//  _precondition_(_ADDRESS_, _MAGIC_, CN_PARCEL, _FOOTER_);
//
//  //vTrace("m_eParcelForward %s\n"_Stz,aYF[m_eParcelForward]);
//
//  switch (m_eCrossTopology) {
//
//    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//    // PX cases which send this parcel to the origin
//
//    case YX_ALL_NODES_TO_ORIGIN:
//    case YX_ALL_PARTITIONS_TO_ORIGIN:
//    case YX_NEXT_NODE_TO_ORIGIN:
//    case YX_ORIGIN_TO_ORIGIN: {
//      RASSERT(YF_ORIGIN == m_eParcelForward);
//      vFwdTo_OnOrigin(pSendOp);
//      break;
//    }
//
//    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//    // PX cases which send this parcel to all partitions
//
//    case YX_ORIGIN_TO_ALL_PARTITIONS:
//    case YX_ALL_NODES_TO_ALL_PARTITIONS:
//    case YX_ALL_PARTITIONS_ON_ORIGIN_TO_ALL_PARTITIONS:
//      RASSERT(YF_ALL_PARTITIONS == m_eParcelForward);
//      vFwdTo_OnAllPartitions(pSendOp);
//      break;
//
//    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//    // PX cases which send this parcel to all partitions on origin
//
//    case YX_ORIGIN_TO_ALL_PARTITIONS_ON_ORIGIN:
//      RASSERT(YF_ALL_PARTITIONS_ON_ORIGIN == m_eParcelForward);
//      vFwdTo_OnAllPartitionsOnOrigin(pSendOp);
//      break;
//
//    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//    // PX cases which send this parcel to ???? ALKA
//
//    case YX_ALL_PARTITIONS_TO_ALL_NODES: {
//      RASSERT(YF_ALL_NODES == m_eParcelForward);
//      vFwdTo_OnNode(pSendOp);
//      break;
//    }
//
//    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//    // PX cases which send this parcel to ???? ALKA
//
//    case YX_ALL_PARTITIONS_TO_ALL_PARTITIONS: {
//      RASSERT(YF_ALL_PARTITIONS == m_eParcelForward);
//
//      if (m_eSendCast == XK_BROADCAST)
//        vFwdTo_OnAllPartitions(pSendOp);
//      else if (pGetOperand()->bIsBuffer())
//        vFwdBufferTo_OnePtn(pSendOp);
//      else
//        vFwdTo_OnAllPartitions(pSendOp);
//
//      break;
//    }
//
//    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//    // PX cases which send this parcel to all nodes
//
//    case YX_ALL_PARTITIONS_ON_ORIGIN_TO_ALL_NODES:
//    case YX_ORIGIN_TO_ALL_NODES: {
//      RASSERT(YF_ALL_NODES == m_eParcelForward);
//      vFwdTo_OnAllNodes(pSendOp);
//      break;
//    }
//
//    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//    // PX cases which send this parcel to next node
//
//    case YX_ORIGIN_TO_NEXT_NODE: {
//      RASSERT(YF_NEXT_NODE == m_eParcelForward);
//      vFwdTo_OnNextNode(pSendOp);
//      break;
//    }
//
//    //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//
//    default:
//      RASSERT(0);
//  }
//}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* forward this parcel to the next node-stage on all nodes                    */
/*                                                                            */
/*   STAGE prv <topology> :                                                   */
/*                                                                            */
/*     | dst                                                                  */
/*                                                                            */
/*   STAGE dst ON ALL NODES :                                                 */
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Parcel::vFwdTo_OnAllNodes(Op *pSendOp)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PARCEL, _FOOTER_);

  // intra & cross topo
//  RASSERT(   m_eCrossTopology == YX_ALL_PARTITIONS_ON_ORIGIN_TO_ALL_NODES
//          || m_eCrossTopology == YX_ORIGIN_TO_ALL_NODES);

  // stage topo
  Stage *pNextStage = pSendOp->pNextStage();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // partition not-applicable
  PtnId tPtn_na = C_UNKNOWN;

  for (Node_I oNode_I; oNode_I.bMore(); oNode_I.vNext()) {

    i8 iNde = oNode_I->iNodeNo();

    vDispatchParcel(pSendOp, __LINE__, iNde, pNextStage, tPtn_na);
  }
}

/*----------------------------------------------------------------------------*/
/* forward this parcel to next partition-stages on all nodes                  */
/*                                                                            */
/*   STAGE prv <topology> :                                                   */
/*                                                                            */
/*     | dst [ BROADCAST | HASHCAST | LOCALROBIN | UNICAST | ROBIN ]          */
/*                                                                            */
/*   STAGE dst ON ALL PARTITIONS :                                            */
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Parcel::vFwdTo_OnAllPartitions(Op *pSendOp)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PARCEL, _FOOTER_);

  switch (m_eSendCast) {

    case XK_BROADCAST  : vFwdTo_OnAllPartitions_broadcast(pSendOp);  break;
    case XK_HASHCAST   : vFwdTo_OnAllPartitions_hashcast(pSendOp);   break;
    case XK_LOCALROBIN : vFwdTo_OnAllPartitions_localrobin(pSendOp); break;
    case XK_ROBIN      : vFwdTo_OnAllPartitions_robin(pSendOp);      break;
    case XK_UNICAST    : vFwdTo_OnAllPartitions_unicast(pSendOp);    break;

    default:
      RASSERT(0);
  }
}
/*----------------------------------------------------------------------------*/
void Parcel::vFwdTo_OnAllPartitions_broadcast(Op *pSendOp)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PARCEL, _FOOTER_);

  // cross & intra topo
//  RASSERT(m_eCrossTopology == YX_ORIGIN_TO_ALL_PARTITIONS
//          || m_eCrossTopology == YX_ALL_NODES_TO_ALL_PARTITIONS
//          || m_eCrossTopology == YX_ALL_PARTITIONS_TO_ALL_PARTITIONS
//          || m_eCrossTopology == YX_ALL_PARTITIONS_ON_ORIGIN_TO_ALL_PARTITIONS);

  // stage topo
  Stage *pNextStage = pSendOp->pNextStage();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  i8 nStreamPtns = pNextStage->nItems();
  for (i8 i_Stream = 0; i_Stream < nStreamPtns; i_Stream++) {
    Stream* pStream = pNextStage->pGet(i_Stream);

    PtnId tStreamPtn = pStream->tPtnId();
    i8 iMappedNde = AM::pPtnMap()->iMappedNodeNo(tStreamPtn);

    vDispatchParcel(pSendOp, __LINE__, iMappedNde, pNextStage, tStreamPtn);
  }
}
/*----------------------------------------------------------------------------*/
void Parcel::vFwdTo_OnAllPartitions_hashcast(Op *pSendOp)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PARCEL, _FOOTER_);

  // cross & intra topo
//  RASSERT(m_eCrossTopology == YX_ORIGIN_TO_ALL_PARTITIONS
//          || m_eCrossTopology == YX_ALL_NODES_TO_ALL_PARTITIONS
//          || m_eCrossTopology == YX_ALL_PARTITIONS_TO_ALL_PARTITIONS
//          || m_eCrossTopology == YX_ALL_PARTITIONS_ON_ORIGIN_TO_ALL_PARTITIONS);

  // stage topo
  Stage *pNextStage = pSendOp->pNextStage();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Operand *pOperand = pGetOperand();
  DataBuffer *pDataBuffer = _pDataBuffer(pOperand);

  PtnId tStreamPtn = pDataBuffer->tPtnId();
  i8    iMappedNde = AM::pPtnMap()->iMappedNodeNo(tStreamPtn);

  vDispatchParcel(pSendOp, __LINE__, iMappedNde, pNextStage, tStreamPtn);
}
/*----------------------------------------------------------------------------*/
void Parcel::vFwdTo_OnAllPartitions_localrobin(Op *pSendOp)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PARCEL, _FOOTER_);

  // cross & intra topo
//  RASSERT(m_eCrossTopology == YX_ORIGIN_TO_ALL_PARTITIONS
//          || m_eCrossTopology == YX_ALL_NODES_TO_ALL_PARTITIONS
//          || m_eCrossTopology == YX_ALL_PARTITIONS_TO_ALL_PARTITIONS
//          || m_eCrossTopology == YX_ALL_PARTITIONS_ON_ORIGIN_TO_ALL_PARTITIONS);

  // stage topo
  Stage *pNextStage = pSendOp->pNextStage();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  i8    i_LocalRobin = pSendOp->iLocalRobinNext();
  PtnId tStreamPtn   = AM::PTNMAP::tRobinToLclPtnStream(i_LocalRobin);
  i8    iMappedNde   = AM::pPtnMap()->iMappedNodeNo(tStreamPtn);
  RASSERT(iMappedNde == MY::pNode()->iNodeNo());

  vDispatchParcel(pSendOp, __LINE__, iMappedNde, pNextStage, tStreamPtn);
}
/*----------------------------------------------------------------------------*/
void Parcel::vFwdTo_OnAllPartitions_robin(Op *pSendOp)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PARCEL, _FOOTER_);

  // cross & intra topo
//  RASSERT(m_eCrossTopology == YX_ORIGIN_TO_ALL_PARTITIONS
//          || m_eCrossTopology == YX_ALL_NODES_TO_ALL_PARTITIONS
//          || m_eCrossTopology == YX_ALL_PARTITIONS_TO_ALL_PARTITIONS
//          || m_eCrossTopology == YX_ALL_PARTITIONS_ON_ORIGIN_TO_ALL_PARTITIONS);

  // stage topo
  Stage *pNextStage = pSendOp->pNextStage();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  i8    i_Robin    = pSendOp->iRobinNext();
  PtnId tStreamPtn = AM::PTNMAP::tRobinToDtdPtnStream(i_Robin);
  i8    iMappedNde = AM::pPtnMap()->iMappedNodeNo(tStreamPtn);

  vDispatchParcel(pSendOp, __LINE__, iMappedNde, pNextStage, tStreamPtn);
}
/*----------------------------------------------------------------------------*/
void Parcel::vFwdTo_OnAllPartitions_unicast(Op *pSendOp)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PARCEL, _FOOTER_);

  // cross & intra topo
//  RASSERT(m_eCrossTopology == YX_ORIGIN_TO_ALL_PARTITIONS
//          || m_eCrossTopology == YX_ALL_NODES_TO_ALL_PARTITIONS
//          || m_eCrossTopology == YX_ALL_PARTITIONS_TO_ALL_PARTITIONS
//          || m_eCrossTopology == YX_ALL_PARTITIONS_ON_ORIGIN_TO_ALL_PARTITIONS);

  // stage topo
  Stage *pNextStage = pSendOp->pNextStage();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  i8 nStreamPtns = pNextStage->nItems();
  for (PtnId tStreamPtn = 0; tStreamPtn < nStreamPtns; ++tStreamPtn) {
    i8 iMappedNde = AM::pPtnMap()->iMappedNodeNo(tStreamPtn);

    vDispatchParcel(pSendOp, __LINE__, iMappedNde, pNextStage, tStreamPtn);
  }
}

/*----------------------------------------------------------------------------*/
/* forward this parcel to the next partition-stages on origin                 */
/*                                                                            */
/*   STAGE prv <topology> :                                                   */
/*                                                                            */
/*     | dst [ ROBIN | UNICAST ]                                              */
/*                                                                            */
/*   STAGE dst ALL PARTITIONS ON ORIGIN :                                     */
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Parcel::vFwdTo_OnAllPartitionsOnOrigin(Op *pSendOp)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PARCEL, _FOOTER_);

  switch (m_eSendCast) {

    case XK_ROBIN   : vFwdTo_OnAllPartitionsOnOrigin_robin(pSendOp);   break;
    case XK_UNICAST : vFwdTo_OnAllPartitionsOnOrigin_unicast(pSendOp); break;

    default:
      RASSERT(0);
  }
}
/*----------------------------------------------------------------------------*/
void Parcel::vFwdTo_OnAllPartitionsOnOrigin_robin(Op *pSendOp)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PARCEL, _FOOTER_);

  // cross & intra topo
//  RASSERT(m_eCrossTopology == YX_ORIGIN_TO_ALL_PARTITIONS_ON_ORIGIN);

  // stage topo
  Stage *pNextStage = pSendOp->pNextStage();

  // assert origin
  Pipeline *pPipeline = pSendOp->pPipeline_op();
  i8 iOriginNode = pPipeline->i_Origin();
  RASSERT(iOriginNode == MY::pNode()->iNodeNo());

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  i8    i_Robin    = pSendOp->iRobinNext();
  PtnId tStreamPtn = AM::PTNMAP::tRobinToNdePtnNo(i_Robin);
  i8    iMappedNde = AM::pPtnMap()->iMappedNodeNo(tStreamPtn);
  RASSERT(iMappedNde == iOriginNode);

  vDispatchParcel(pSendOp, __LINE__, iMappedNde, pNextStage, tStreamPtn);
}
/*----------------------------------------------------------------------------*/
void Parcel::vFwdTo_OnAllPartitionsOnOrigin_unicast(Op *pSendOp)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PARCEL, _FOOTER_);

  // cross & intra topo
//  RASSERT(m_eCrossTopology == YX_ORIGIN_TO_ALL_PARTITIONS_ON_ORIGIN);

  // stage topo
  Stage *pNextStage = pSendOp->pNextStage();

  // assert origin
  Pipeline *pPipeline = pSendOp->pPipeline_op();
  i8 iOriginNode = pPipeline->i_Origin();
  RASSERT(iOriginNode == MY::pNode()->iNodeNo());

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  i8 nStreamPtns = pNextStage->nItems();
  for (i8 i_Stream = 0; i_Stream < nStreamPtns; i_Stream++) {
    Stream* pStream = pNextStage->pGet(i_Stream);

    PtnId tStreamPtn = pStream->tPtnId();
    i8   iMappedNde  = AM::pPtnMap()->iMappedNodeNo(tStreamPtn);
    RASSERT(iMappedNde == iOriginNode);

    vDispatchParcel(pSendOp, __LINE__, iMappedNde, pNextStage, tStreamPtn);
  }
}

/*----------------------------------------------------------------------------*/
/* forward this parcel to the next node-stage on next node                    */
/*                                                                            */
/*   STAGE prv ON ORIGIN :                                                    */
/*                                                                            */
/*     | dst                                                                  */
/*                                                                            */
/*   STAGE dst ON NEXT NODE :                                                 */
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Parcel::vFwdTo_OnNextNode(Op *pSendOp)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PARCEL, _FOOTER_);

  // intra & cross topo
//  RASSERT(m_eCrossTopology == YX_ORIGIN_TO_NEXT_NODE);

  // stage topo of next stage
  Stage *pNextStage = pSendOp->pNextStage();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // partition not-applicable
  PtnId tPtn_na = C_UNKNOWN;

  Node* pThisNode = MY::pNode();
  
  for (Node_I oNode_I; oNode_I.bMore(); oNode_I.vNext()) {

    if (pThisNode != * oNode_I) {
      i8 iNde = oNode_I->iNodeNo();
      vDispatchParcel(pSendOp, __LINE__, iNde, pNextStage, tPtn_na);
      break;
    }
  }
}

/*----------------------------------------------------------------------------*/
/* forwar this parcel to given destination node                               */
/*                                                                            */
/*                                                                            */
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Parcel::vFwdTo_OnNode(Op *pSendOp)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PARCEL, _FOOTER_);

  // intra & cross topo
  //RASSERT(m_eCrossTopology == YX_ALL_PARTITIONS_TO_ALL_NODES);

  // stage topo
  Stage *pNextStage = pSendOp->pNextStage();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  i8 iMappedNde;

  Operand *pOperand = pGetOperand();
  if (CN_METAGET == pOperand->eGetClassNo()) {
    Node* pOtherNode = AM::pGrid()->pOtherNode();
    iMappedNde = pOtherNode->iNodeNo();
  }
  else {
    PtnId tSendToPtn = pSendOp->tPtnId();
    iMappedNde = AM::pPtnMap()->iMappedNodeNo(tSendToPtn);
  }

  PtnId tPtn_na = C_UNKNOWN;
  vDispatchParcel(pSendOp, __LINE__, iMappedNde, pNextStage, tPtn_na);
}

/*----------------------------------------------------------------------------*/
/* forward this parcel to next node-stage on origin                           */
/*                                                                            */
/*   STAGE prv <topology> :                                                   */
/*                                                                            */
/*     | dst                                                                  */
/*                                                                            */
/*   STAGE dst ON ORIGIN :                                                    */
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Parcel::vFwdTo_OnOrigin(Op *pSendOp)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PARCEL, _FOOTER_);

  // intra & cross topo
//  RASSERT(m_eCrossTopology == YX_ALL_NODES_TO_ORIGIN ||
//          m_eCrossTopology == YX_NEXT_NODE_TO_ORIGIN ||
//          m_eCrossTopology == YX_ORIGIN_TO_ORIGIN    ||
//          m_eCrossTopology == YX_ALL_PARTITIONS_TO_ORIGIN);

  // stage topo
  Stage *pNextStage = pSendOp->pNextStage();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Pipeline *pPipeline = pSendOp->pPipeline_op();
  i8 iOriginNde = pPipeline->i_Origin();

  PtnId tPtn_na = C_UNKNOWN;
  vDispatchParcel(pSendOp, __LINE__, iOriginNde, pNextStage, tPtn_na);
}

/*----------------------------------------------------------------------------*/
/* forward this parcelled buffer to to a single next partition-stage          */
/*                                                                            */
/*   STAGE prv ON ALL PARTITIONS :                                            */
/*                                                                            */
/*     | dst [ HASHCAST | ROBIN | UNICAST ]                                   */
/*                                                                            */
/*   STAGE dst ON ALL PARTITIONS :                                            */
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Parcel::vFwdBufferTo_OnePtn(Op *pSendOp)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PARCEL, _FOOTER_);

  if (XK_HASHCAST == m_eSendCast)
    vFwdBufferTo_OnePtn_hashcast(pSendOp);
  else if (XK_UNICAST == m_eSendCast)
    vFwdBufferTo_OnePtn_unicast(pSendOp);
  else if (XK_ROBIN == m_eSendCast)
    vFwdBufferTo_OnePtn_robin(pSendOp);
  else
    NEVER_REACH;
}

/*----------------------------------------------------------------------------*/
/* forward this parcelled buffer to a single next partition-stage (by hashcast*/
/*                                                                            */
/*   STAGE prv ON ALL PARTITIONS :                                            */
/*                                                                            */
/*     | dst HASHCAST                                                         */
/*                                                                            */
/*   STAGE dst ON ALL PARTITIONS :                                            */
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Parcel::vFwdBufferTo_OnePtn_hashcast(Op *pSendOp)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PARCEL, _FOOTER_);

  // intra & cross topo
  //RASSERT(m_eCrossTopology == YX_ALL_PARTITIONS_TO_ALL_PARTITIONS);

  // stage topo
  Stage *pNextStage = pSendOp->pNextStage();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Operand *pOperand = pGetOperand();
  DataBuffer *pDataBuffer = _pDataBuffer(pOperand);

  PtnId tStreamPtn = pDataBuffer->tPtnId();

  i8 iMappedNde = AM::pPtnMap()->iMappedNodeNo(tStreamPtn);

  vDispatchParcel(pSendOp, __LINE__, iMappedNde, pNextStage, tStreamPtn);
}

/*----------------------------------------------------------------------------*/
/* forward this parcelled buffer to a single next partition-stage (by robin)  */
/*                                                                            */
/*   STAGE prv ON ALL PARTITIONS :                                            */
/*                                                                            */
/*     | dst ROBIN                                                            */
/*                                                                            */
/*   STAGE dst ON ALL PARTITIONS :                                            */
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Parcel::vFwdBufferTo_OnePtn_robin(Op *pSendOp)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PARCEL, _FOOTER_);

  NEVER_REACH; // and if you do reach here talk to Ray

  // intra & cross topo
//  RASSERT(m_eCrossTopology == YX_ALL_PARTITIONS_TO_ALL_PARTITIONS);

  // stage topo
  Stage *pNextStage = pSendOp->pNextStage();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  i8 i_Robin = pSendOp->iRobinNext();

  PtnId tStreamPtn = AM::PTNMAP::tRobinToDtdPtnStream(i_Robin);
  
  i8 iMappedNde = AM::pPtnMap()->iMappedNodeNo(tStreamPtn);

  vDispatchParcel(pSendOp, __LINE__, iMappedNde, pNextStage, tStreamPtn);
}

/*----------------------------------------------------------------------------*/
/* forward this parcelled buffer to a single next partition-stage (by unicast)*/
/*                                                                            */
/*   STAGE prv ON ALL PARTITIONS :                                            */
/*                                                                            */
/*     | dst UNICAST                                                          */
/*                                                                            */
/*   STAGE dst ON ALL PARTITIONS :                                            */
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Parcel::vFwdBufferTo_OnePtn_unicast(Op *pSendOp)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PARCEL, _FOOTER_);

  // intra & cross topo
  //RASSERT(m_eCrossTopology == YX_ALL_PARTITIONS_TO_ALL_PARTITIONS);

  Stage *pNextStage = pSendOp->pNextStage();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  PtnId tSendToPtn = pSendOp->tPtnId();

  Stream *pStream = pNextStage->pGetStream_byPtnNo(tSendToPtn.iAsI8());
  
  PtnId tStreamPtn = pStream->tPtnId();
  
  i8 iMappedNde = AM::pPtnMap()->iMappedNodeNo(tStreamPtn);

  vDispatchParcel(pSendOp, __LINE__, iMappedNde, pNextStage, tStreamPtn);
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* send parcel to given node,partition                                        */
/*----------------------------------------------------------------------------*/
void Parcel::vDispatchParcel(Op* pSendOp,
                             i8  iCodeLine,
                             i8  iDstNode,
                             Stage* pDstStage,
                             PtnId  tDstPtn)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PARCEL, _FOOTER_);

//  i8 iCurNode = MY::pNode()->iNodeNo();
//
//  if (iDstNode == iCurNode) {
//    Task* pCurTask = MY::pTask();
//    //Op* pDstOp = pDstStage->pPipeDstOp(eIntraTopology(),tDstPtn,0);
//    Operand* pOperand = pGetOperand();
//    m_bDeleteOperand = false;
//    AM::OP::vPipeToNextOp(pSendOp,pOperand,pCurTask);
//    return;
//  }
  
  i8 iDstStage = pDstStage->iStage();

  m_iDstNode = iDstNode;
  m_iDstStage = iDstStage;
  m_tDstPtn = tDstPtn;

  m_iSrcNode = MY::pNode()->iNodeNo();
  m_iCodeLine = iCodeLine;

  pSendOp->vSendNode(m_iDstNode);

  vSendParcelViaSocket(pSendOp);

  // - - - - - - - - - - - - - - - -

  i8 i_OtherStage = pSendOp->iOtherStage();
  if (C_UNKNOWN != i_OtherStage) {
    m_iDstStage = i_OtherStage;
    vSendParcelViaSocket(pSendOp);
  }
}

/*----------------------------------------------------------------------------*/
/* send parcel (and its contents) out over socket                             */
/*----------------------------------------------------------------------------*/
void Parcel::vSendParcelViaSocket(Op *pSendOp)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PARCEL, _FOOTER_);

  bool bResult;
  i8 nObjectSize;
  SysCall oSysCall;
  Object *pObject;
  Parcel *pParcel;

  // get preliminary information
  Node *pDstNode = AM::pGrid()->pFindNode(m_iDstNode);
  //vTrace("vSendParcelViaSocket prepared to send to Node:%lld Port:%lld \n"_Stz,pDstNode->iNodeNo(), pDstNode->iPortNo());
  Socket *pSocket = pDstNode->pSendSocket();
  i8 nObjects = this->nItems();

  pSocket->vGrabWrite();

  //vTrace("vSendParcelViaSocket got a lock, prepared to send %lld Object(s)to Node:%lld Port:%lld \n"_Stz,nObjects, pDstNode->iNodeNo(), pDstNode->iPortNo());

  // send the parcel instance
  pParcel = this;
  nObjectSize = gxaSizeHeap[pParcel->eGetClassNo()].m_nClassSize;
  bResult = pSocket->bSendTo(pParcel, nObjectSize, oSysCall);

  //if (!bResult) {
  //  vTrace("vSendParcelViaSocket SENT FAILED\n"_Stz);
  //} else {
  //  vTrace("vSendParcelViaSocket SENT %lld bytes Parcel:%s to Node:%lld Port:%lld \n"_Stz,nObjectSize, aCN[pParcel->pGetOperand()->eGetClassNo()], pDstNode->iNodeNo(), pDstNode->iPortNo());
  //}

  // send all the objects within the parcel instance
  for (i8 i_Item = 0; i_Item < nObjects; i_Item++) {
    pObject = pParcel->pGetItem(i_Item);
    eCN eClassNo = pObject->eGetClassNo();
    nObjectSize = gxaSizeHeap[eClassNo].m_nClassSize;

    if (CN_IDXVEC == eClassNo) {
      i8 iSize1 = gxaSizeHeap[CN_INDEX].m_nClassSize;
      i8 iSize2 = gxaSizeHeap[CN_IDXVEC].m_nClassSize;
      vTrace("vSendParcelViaSocket SENT IdxVec: %lld bytes Index:%lld bytes \n"_Stz, iSize2, iSize1);

    }

    if (CN_CRWBUFFER == eClassNo && AM::TRACE::bVerbose(C_VB_SEND_CRWBUF)) {
      CrwBuffer *pCrwBuffer = _pCrwBuffer(pObject);
      vTrace("Sending a CrwBuffer (%lld rows) to node %s tPtnId %lld\n"_Stz, pCrwBuffer->nRows(), pDstNode->szNodeNm(),
             pCrwBuffer->tPtnId().iAsI8()
      );
    }

    bResult = pSocket->bSendTo(pObject, nObjectSize, oSysCall);
    //vTrace("vSendParcelViaSocket SENT %lld bytes class:%s  \n"_Stz,nObjectSize, aCN[pObject->eGetClassNo()]);

  }

  pSocket->vDropWrite();

  s256 sSupplemental;

  sSupplemental.vSprint("%s %s"_Stz, pDstNode->szNodeNm(), aXK[m_eSendCast]);

  pSendOp->vOpTrace(this, (Stz *) __func__, true, sSupplemental);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
