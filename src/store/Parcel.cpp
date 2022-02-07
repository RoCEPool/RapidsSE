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
Parcel::Parcel(bool bOriginalParcel)
: Operand(CN_PARCEL)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PARCEL, _FOOTER_);

  m_iPipelineJobId = C_UNKNOWN;
  m_sDemonNm = C_NO_NAME;

  m_iDstNode = C_UNKNOWN;
  m_iDstStage = C_UNKNOWN;
  m_tDstPtn = C_UNKNOWN;

  m_iSrcNode = C_UNKNOWN;
  m_iCodeLine = C_UNKNOWN;

  m_eParcelForward = YF_NONE;
  m_eSendCast = XK_NONE;

  m_bOriginalParcel = bOriginalParcel;
      
  m_bDeleteOperand = true;
  m_eSendWhy = SY_NONE;
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
/* get demon name                                                             */
/*----------------------------------------------------------------------------*/
Stz* Parcel::szDemonNm(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PARCEL, _FOOTER_);

  return m_sDemonNm;
}

/*----------------------------------------------------------------------------*/
/* set demon name                                                             */
/*----------------------------------------------------------------------------*/
void Parcel::vDemonNm(Stz* szDemonNm)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PARCEL, _FOOTER_);

  m_sDemonNm = szDemonNm;
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
void Parcel::vLabel(i8 iPipelineJobId, eYF eParcelForward)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PARCEL, _FOOTER_);

  m_iPipelineJobId = iPipelineJobId;
  m_eParcelForward = eParcelForward;
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

  vAppraise_Forward_cast(pSendOp);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Operand *pOperand = pGetOperand();
  /*
  if (CN_METAGET == pOperand->eGetClassNo()) {
    // Send metaGet to other node, not to the origin node itself
    vNDE_OtherNode_TBD(pSendOp); // HACK  hardcode to the existing node for debug
    if (m_bDeleteOperand)
      AM::vDeleteOperand(pOperand);
    return;
  }
   */
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  vDistributeBy_Forward_cast(pSendOp);

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
void Parcel::vAppraise_Forward_cast(Op *pSendOp)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PARCEL, _FOOTER_);

  m_eSendCast = pSendOp->eSendCast();

  Operand *pOperand = pGetOperand();
  eCN eClass = pOperand->eGetClassNo();
  
  switch (eClass) {
    
    case CN_INDEXDEF:     m_eSendCast = XK_BROADCAST; break;
    case CN_INFOSET:
      m_eSendCast = XK_ROBINDIST;
      break;
    case CN_MARKER:       m_eSendCast = XK_BROADCAST; break;
    case CN_PROFILE:      m_eSendCast = XK_ROBINDIST; break;
    case CN_STREAMDEF:    m_eSendCast = XK_BROADCAST; break;
    case CN_STREAMDEFRHT: m_eSendCast = XK_BROADCAST; break;
    case CN_STREAMSET:    m_eSendCast = XK_BROADCAST; break;
      
    default:
      RASSERT(pOperand->bIsBuffer());
  }
  
  if (YF_ALL_PARTITIONS_ON_ORIGIN == pSendOp->pOpSpec()->eParcelForward()) {

    switch (eClass) {
      
      case CN_INDEXDEF:     m_eSendCast = XK_FANCAST;    break;
      case CN_INFOSET:
        m_eSendCast = XK_ROBINLOCAL;
        break;
      case CN_MARKER:       m_eSendCast = XK_FANCAST;    break;
      case CN_PROFILE:      m_eSendCast = XK_ROBINLOCAL; break;
      case CN_STREAMDEF:    m_eSendCast = XK_FANCAST;    break;
      case CN_STREAMDEFRHT: m_eSendCast = XK_FANCAST;    break;
      case CN_STREAMSET:    m_eSendCast = XK_FANCAST;    break;
        
      default:
        RASSERT(pOperand->bIsBuffer());
        m_eSendCast = XK_ROBINLOCAL;
    }

  }

//  vTrace("B Operand %s %s\n"_Stz
//         ,aCN[eClass]
//         ,aXK[m_eSendCast]
//        );
}

/*----------------------------------------------------------------------------*/
/* cross-topo send of this parcel                                             */
/*----------------------------------------------------------------------------*/
void Parcel::vDistributeBy_Forward_cast(Op *pSendOp)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PARCEL, _FOOTER_);
  
  switch (m_eParcelForward) {

    case YF_ALL_NODES : vNDE_AllNodes(pSendOp);    break;
    case YF_NEXT_NODE : vNDE_NextNode(pSendOp);    break;
    case YF_ORIGIN    : vNDE_OriginNode(pSendOp);  break;
    case YF_SAME_NODE : vNDE_SameNode(pSendOp);    break;

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    case YF_ALL_PARTITIONS:
    case YF_ALL_PARTITIONS_ON_ORIGIN:

      switch (m_eSendCast) {

        case XK_BROADCAST  : vPTN_broadcast(pSendOp);   break;
        case XK_FANCAST    : vPTN_fancast(pSendOp);     break;
        case XK_HASHCAST   : vPTN_hashcast(pSendOp);    break;
        case XK_ROBINDIST  : vPTN_robinDist(pSendOp);   break;
        case XK_ROBINLOCAL : vPTN_robinLocal(pSendOp);  break;
        case XK_UNICAST    : vPTN_unicast(pSendOp);     break;

        default:
          ASSERT(0);
      }
      break;

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    default:
      RASSERT(0);
  }
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Parcel::vNDE_AllNodes(Op *pSendOp)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PARCEL, _FOOTER_);

  // stage topo
  Stage *pNextStage = pSendOp->pNextStage();

  m_eSendWhy = SY_ALL_NODES;
  m_eRecvInstructions = RV_ONE_PTN;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // partition not-applicable
  PtnId tPtn_na = C_UNKNOWN;

  for (Node_I oNode_I; oNode_I.bMore(); oNode_I.vNext()) {

    i8 iNde = oNode_I->iNodeNo();

    vDispatchParcel(pSendOp, __LINE__, iNde, pNextStage, tPtn_na);
  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Parcel::vNDE_NextNode(Op *pSendOp)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PARCEL, _FOOTER_);

  // stage topo of next stage
  Stage *pNextStage = pSendOp->pNextStage();

  m_eSendWhy = SY_NEXT_NODE;
  m_eRecvInstructions = RV_ONE_PTN;

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
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Parcel::vNDE_OriginNode(Op *pSendOp)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PARCEL, _FOOTER_);

  // stage topo
  Stage *pNextStage = pSendOp->pNextStage();

  m_eSendWhy = SY_ORIGIN_NODE;
  m_eRecvInstructions = RV_ONE_PTN;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Pipeline *pPipeline = pSendOp->pPipeline_op();
  i8 iOriginNde = pPipeline->i_Origin();

  PtnId tPtn_na = C_UNKNOWN;
  vDispatchParcel(pSendOp, __LINE__, iOriginNde, pNextStage, tPtn_na);
}

/*----------------------------------------------------------------------------*/
/* forwar this parcel to given destination node                               */
/*----------------------------------------------------------------------------*/
void Parcel::vNDE_OtherNode_TBD(Op *pSendOp)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PARCEL, _FOOTER_);
  vTrace("vNDE_OtherNode_TBD\n"_Stz);
  NEVER_REACH;

  // stage topo
  Stage *pNextStage = pSendOp->pNextStage();

  m_eSendWhy = SY_NONE;
  m_eRecvInstructions = RV_ONE_PTN;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  i8 iMappedNde;

  Operand *pOperand = pGetOperand();
  if (CN_METAGET == pOperand->eGetClassNo()) {
    Node* pOtherNode = AM::pGrid()->pOtherNode();
    iMappedNde = pOtherNode->iNodeNo();
  }
  else {
    RASSERT(0);
    PtnId tSendToPtn = pSendOp->tPtnId();
    iMappedNde = AM::pPtnMap()->iMappedNodeNo(tSendToPtn);
  }

  PtnId tPtn_na = C_UNKNOWN;
  vDispatchParcel(pSendOp, __LINE__, iMappedNde, pNextStage, tPtn_na);
}

/*----------------------------------------------------------------------------*/
/* forwar this parcel to same node                                            */
/*----------------------------------------------------------------------------*/
void Parcel::vNDE_SameNode(Op *pSendOp)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PARCEL, _FOOTER_);

  // stage topo
  Stage *pNextStage = pSendOp->pNextStage();

  m_eSendWhy = SY_SAME_NODE;
  m_eRecvInstructions = RV_ONE_PTN;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  i8 iMappedNde;

  Operand *pOperand = pGetOperand();
  if (CN_METAGET == pOperand->eGetClassNo()) {
    RASSERT(0);
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
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Parcel::vPTN_broadcast(Op *pSendOp)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PARCEL, _FOOTER_);

  //  Operand *pOperand = pGetOperand();
  //  RASSERT(pOperand->bIsBuffer());
  m_eSendWhy = SY_BROADCAST;
  m_eRecvInstructions = RV_ALL_PTNS;
  

  Stage *pDstStage = pSendOp->pNextStage();

  PtnId tPtn_na = C_UNKNOWN;
  

  for (Node_I oNode_I; oNode_I.bMore(); oNode_I.vNext()) {
    i8 iNde = oNode_I->iNodeNo();
     
    vDispatchParcel(pSendOp, __LINE__, iNde, pDstStage, tPtn_na);
  }
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Parcel::vPTN_fancast(Op *pSendOp)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PARCEL, _FOOTER_);

  Operand *pOperand = pGetOperand();
  RASSERT(! pOperand->bIsBuffer());

  // wont be sent
  m_eSendWhy = SY_FANCAST;
  m_eRecvInstructions = RV_NONE;

  i8 iCurNode = MY::pNode()->iNodeNo();

  Stage *pDstStage = pSendOp->pNextStage();
   
  PtnId tPtn_na = C_UNKNOWN;
  vDispatchParcel(pSendOp, __LINE__, iCurNode, pDstStage, tPtn_na);
//  i8 nStreamPtns = pDstStage->nItems();
//  for (i8 i_Stream = 0; i_Stream < nStreamPtns; i_Stream++) {
//    Stream* pDstStream = pDstStage->pGet(i_Stream);
//
//    PtnId tDstPtn = pDstStream->tPtnId();
//    i8 iDstNde = AM::pPtnMap()->iMappedNodeNo(tDstPtn);
//
//    RASSERT(iDstNde == iCurNode);
//
//    vDispatchParcel(pSendOp, __LINE__, iDstNde, pDstStage, tDstPtn);
//  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Parcel::vPTN_hashcast(Op *pSendOp)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PARCEL, _FOOTER_);

  Operand *pOperand = pGetOperand();
  m_eSendWhy = SY_HASHCAST;
  m_eRecvInstructions = RV_ONE_PTN;

  
  Stage *pDstStage = pSendOp->pNextStage();

  DataBuffer *pDataBuffer = _pDataBuffer(pOperand);

  PtnId tDstPtn = pDataBuffer->tPtnId();
  i8 iDstNde = AM::pPtnMap()->iMappedNodeNo(tDstPtn);

  vDispatchParcel(pSendOp, __LINE__, iDstNde, pDstStage, tDstPtn);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Parcel::vPTN_robinDist(Op *pSendOp)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PARCEL, _FOOTER_);

  m_eSendWhy = SY_ROBINDIST;
  m_eRecvInstructions = RV_ONE_PTN;

  
  // stage topo
  Stage *pDstStage = pSendOp->pNextStage();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  i8    i_Robin    = pSendOp->iRobinNext();
  PtnId tDstPtn = AM::PTNMAP::tRobinToDtdPtnStream(i_Robin);
  i8    iDstNde = AM::pPtnMap()->iMappedNodeNo(tDstPtn);

  vDispatchParcel(pSendOp, __LINE__, iDstNde, pDstStage, tDstPtn);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Parcel::vPTN_robinLocal(Op *pSendOp)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PARCEL, _FOOTER_);

  m_eSendWhy = SY_ROBINLOCAL;
  m_eRecvInstructions = RV_ONE_PTN;

  i8 iCurNode = MY::pNode()->iNodeNo();

  
  Stage *pDstStage = pSendOp->pNextStage();

  i8    i_LocalRobin = pSendOp->iLocalRobinNext();
  
  PtnId tDstPtn   = AM::PTNMAP::tRobinToLclPtnStream(i_LocalRobin);
  i8    iDstNde   = AM::pPtnMap()->iMappedNodeNo(tDstPtn);
  RASSERT(iDstNde == iCurNode);

  vDispatchParcel(pSendOp, __LINE__, iDstNde, pDstStage, tDstPtn);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Parcel::vPTN_unicast(Op *pSendOp)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PARCEL, _FOOTER_);

  m_eSendWhy = SY_UNICAST;
  m_eRecvInstructions = RV_ONE_PTN;

  
  Stage *pDstStage = pSendOp->pNextStage();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  PtnId tSendToPtn = pSendOp->tPtnId();

  Stream *pDstStream = pDstStage->pGetStream_byPtnNo(tSendToPtn.iAsI8());
  
  PtnId tDstPtn = pDstStream->tPtnId();
  
  i8 iDstNde = AM::pPtnMap()->iMappedNodeNo(tDstPtn);

  vDispatchParcel(pSendOp, __LINE__, iDstNde, pDstStage, tDstPtn);
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

  i8 iDstStage = pDstStage->i_Stage();

  m_iDstNode  = iDstNode;
  m_iDstStage = iDstStage;
  m_tDstPtn   = tDstPtn;

  m_iSrcNode  = MY::pNode()->iNodeNo();
  m_iCodeLine = iCodeLine;

  pSendOp->vSendNode(m_iDstNode);

//  switch (AM::SYSARGS::iNetOpt()) {
//
//    case 1:
//      vDispatchParcel_netopt1(pSendOp,pDstStage);
//      break;
//
//    default:
//      NEVER_REACH;
//  }

  // this function is called for each node
  
  i8 iCurNode = MY::pNode()->iNodeNo();
  Operand* pOperand = pGetOperand();

  if (m_iDstNode == iCurNode) {
    if (pOperand->bIsBuffer())
      vPipeBufferViaQueues(pSendOp,pDstStage);
    else
      vPipeOperandViaQueues(pSendOp,pDstStage);
  }
  else {
    pSendOp->vSendNode(m_iDstNode);
    vSendParcelViaSocket(pSendOp);
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  i8 i_OtherStage = pSendOp->iOtherStage();
  if (C_UNKNOWN == i_OtherStage)
    return;
  
  m_iDstStage = i_OtherStage;
  //vSendParcelViaSocket(pSendOp);
  
  Stage* pDstStage_other = pSendOp->pOtherStage();
  
  if (m_iDstNode == iCurNode) {
    if (pOperand->bIsBuffer())
      vPipeBufferViaQueues(pSendOp,pDstStage_other);
    else
      vPipeOperandViaQueues(pSendOp,pDstStage_other);
  }
  else {
    pSendOp->vSendNode(m_iDstNode);
    vSendParcelViaSocket(pSendOp);
  }


}

/*----------------------------------------------------------------------------*/
/* pipe shared buffer-operand to next ops                                     */
/*----------------------------------------------------------------------------*/
void Parcel::vPipeBufferViaQueues(Op *pSendOp, Stage* pDstStage)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PARCEL, _FOOTER_);

  m_bDeleteOperand = true;

  Operand* pOperand = pGetOperand();
  DataBuffer*  pBuffer  = _pDataBuffer(pOperand);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // forward to this node, dst stage is a YS_NODE_STREAM
  
  if (YS_NODE_STREAM == pDstStage->pStageSpec()->eStreamTopology()) {
    i8 i_ThisNode = AM::iThisNodeNo();
    Stream* pDstStream = pDstStage->pGetStream_byNodeNo(i_ThisNode);

//    eCN eClass = pOperand->eGetClassNo();
//    i8 nSize = gxaSizeHeap[eClass].m_nClassSize;
//
//    Operand* pDuplicateOperand = (Operand*)Heaps::pAllocate(eClass);
//    MemLib::vCopy(pDuplicateOperand,pOperand,nSize,eClass);
    Operand* pDuplicateOperand = (Operand*)pOperand->pMemoryCopy();
    eCN eClass = pDuplicateOperand->eGetClassNo();

    if (gxaSizeHeap[eClass].m_fpPostRecvFn)
      gxaSizeHeap[eClass].m_fpPostRecvFn(pDuplicateOperand);
    
    Task* pCurTask = MY::pTask();
    
    Op* pDstOp = pDstStream->pGet(0);
 
    AM::OP::vPipeToOp(pDstOp,pDuplicateOperand,pCurTask);
    
    return;
  }
  

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // broadcasting to this node, dst stage is a YS_PTN_STREAM

  if (XK_BROADCAST == m_eSendCast) {
    
    i8 iCurNode = MY::pNode()->iNodeNo();

    i8 nStreamPtns = pDstStage->nItems();
    for (i8 i_Stream = 0; i_Stream < nStreamPtns; i_Stream++) {
      Stream* pDstStream = pDstStage->pGet(i_Stream);

      PtnId tDstPtn = pDstStream->tPtnId();
      i8 iDstNde = AM::pPtnMap()->iMappedNodeNo(tDstPtn);

      if (iDstNde != iCurNode)
        continue;
      
//      eCN eClass = pOperand->eGetClassNo();
//      i8 nSize = gxaSizeHeap[eClass].m_nClassSize;
//
//      Operand* pDuplicateOperand = (Operand*)Heaps::pAllocate(eClass);
//      MemLib::vCopy(pDuplicateOperand,pOperand,nSize,eClass);
      Operand* pDuplicateOperand = (Operand*)pOperand->pMemoryCopy();
      eCN eClass = pDuplicateOperand->eGetClassNo();

      if (gxaSizeHeap[eClass].m_fpPostRecvFn)
        gxaSizeHeap[eClass].m_fpPostRecvFn(pDuplicateOperand);

      Task* pCurTask = MY::pTask();
      
      Op* pDstOp = pDstStream->pGet(0);
   
      AM::OP::vPipeToOp(pDstOp,pDuplicateOperand,pCurTask);
    }

    
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // forward to this node, dst stage is a YS_PTN_STREAM

  Op* pDstOp = pDstStage->pPipeDstOp(tDstPtn(),0);

//  eCN eClass = pOperand->eGetClassNo();
//  i8 nSize = gxaSizeHeap[eClass].m_nClassSize;
//
//  Operand* pDuplicateOperand = (Operand*)Heaps::pAllocate(eClass);
//  MemLib::vCopy(pDuplicateOperand,pOperand,nSize,eClass);

  Operand* pDuplicateOperand = (Operand*)pOperand->pMemoryCopy();
  eCN eClass = pDuplicateOperand->eGetClassNo();
  
  if (gxaSizeHeap[eClass].m_fpPostRecvFn)
    gxaSizeHeap[eClass].m_fpPostRecvFn(pDuplicateOperand);
  
  Task* pCurTask = MY::pTask();
  
  AM::OP::vPipeToOp(pDstOp,pDuplicateOperand,pCurTask);

}

/*----------------------------------------------------------------------------*/
/* pipe non-shared operand to next ops                                        */
/*----------------------------------------------------------------------------*/
void Parcel::vPipeOperandViaQueues(Op *pSendOp, Stage* pDstStage)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_PARCEL, _FOOTER_);

  m_bDeleteOperand = true;

  Operand* pOperand = pGetOperand();
  
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  if (YS_NODE_STREAM == pDstStage->pStageSpec()->eStreamTopology()) {
    i8 i_ThisNode = AM::iThisNodeNo();
    Stream* pDstStream = pDstStage->pGetStream_byNodeNo(i_ThisNode);

//    eCN eClass = pOperand->eGetClassNo();
//    i8 nSize = gxaSizeHeap[eClass].m_nClassSize;
//
//    Operand* pDuplicateOperand = (Operand*)Heaps::pAllocate(eClass);
//    MemLib::vCopy(pDuplicateOperand,pOperand,nSize,eClass);

    Operand* pDuplicateOperand = (Operand*)pOperand->pMemoryCopy();

    Task* pCurTask = MY::pTask();
    
    Op* pDstOp = pDstStream->pGet(0);
 
    AM::OP::vPipeToOp(pDstOp,pDuplicateOperand,pCurTask);
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  i8 iCurNode = MY::pNode()->iNodeNo();

  i8 nStreamPtns = pDstStage->nItems();
  for (i8 i_Stream = 0; i_Stream < nStreamPtns; i_Stream++) {
    Stream* pDstStream = pDstStage->pGet(i_Stream);

    PtnId tDstPtn = pDstStream->tPtnId();
    i8 iDstNde = AM::pPtnMap()->iMappedNodeNo(tDstPtn);

    if (iDstNde != iCurNode)
      continue;
    
//    eCN eClass = pOperand->eGetClassNo();
//    i8 nSize = gxaSizeHeap[eClass].m_nClassSize;
//
//    Operand* pDuplicateOperand = (Operand*)Heaps::pAllocate(eClass);
//    MemLib::vCopy(pDuplicateOperand,pOperand,nSize,eClass);

    Operand* pDuplicateOperand = (Operand*)pOperand->pMemoryCopy();

    Task* pCurTask = MY::pTask();
    
    Op* pDstOp = pDstStream->pGet(0);
 
    AM::OP::vPipeToOp(pDstOp,pDuplicateOperand,pCurTask);
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

  if (RV_NONE == m_eRecvInstructions) {
    vTrace("vSendParcelViaSocket has RV_NONE\n"_Stz);
    RASSERT(0);
  }
  
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
  bResult = pSocket->bSendObject(pParcel,oSysCall,m_eSendWhy);

  //if (!bResult) {
  //  vTrace("vSendParcelViaSocket SENT FAILED\n"_Stz);
  //} else {
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

      //vTrace("vSocketSendOfParcel SENT:%lld IdxVec: %lld bytes Index:%lld bytes %lld  obj:%lld\n"_Stz,
      //       nObjectSize,
      //       gxaSizeHeap[CN_IDXVEC].m_nClassSize,
      //       gxaSizeHeap[CN_INDEX].m_nClassSize,
      //       sizeof(VectorNL_t<CN_IDXVEC,Index,C_MAX_INDEXES>),
      //       pObject->nSizeOfObject());
      // vSocketSendOfParcel SENT:160 IdxVec: 160 bytes Index:528 bytes 160  obj:160
    }

    if (CN_CRWBUFFER == eClassNo && AM::TRACE::bVerbose(C_VB_SEND_CRWBUF)) {
      CrwBuffer *pCrwBuffer = _pCrwBuffer(pObject);
      vTrace("Sending a CrwBuffer (%lld rows) to node %s tPtnId %lld\n"_Stz, pCrwBuffer->nRows(), pDstNode->szNodeNm(),
             pCrwBuffer->tPtnId().iAsI8()
      );
    }

    bResult = pSocket->bSendObject(pObject,oSysCall,m_eSendWhy);
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
/*----------------------------------------------------------------------------*/
/* forward this parcel to next partition-stages on all nodes                  */
/*                                                                            */
/*   STAGE prv <topology> :                                                   */
/*                                                                            */
/*     | dst [ BROADCAST | HASHCAST | LOCALROBIN | UNICAST | DISTROBIN ]      */
/*                                                                            */
/*   STAGE dst ON ALL PARTITIONS :                                            */
/*                                                                            */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* forward this parcel to the next partition-stages on origin                 */
/*                                                                            */
/*   STAGE prv <topology> :                                                   */
/*                                                                            */
/*     | dst [ DISTROBIN | UNICAST ]                                          */
/*                                                                            */
/*   STAGE dst ALL PARTITIONS ON ORIGIN :                                     */
/*                                                                            */
/*----------------------------------------------------------------------------*/
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
