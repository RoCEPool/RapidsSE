/*============================================================================*/
/* Parcel.hpp -- Send Parcel                                                  */
/*============================================================================*/
#pragma once

#include "eXK.h"
#include "eYS.h"
#include "eYG.h"
#include "eYF.h"
#include "eSY.h"

namespace STORE
{
  class Parcel : public Operand
  {
  private:
    i8    m_iPipelineJobId;  //    pipeline job id
    s32   m_sDemonNm;        // or demon name

    i8    m_iDstNode;        // destination node id
    i8    m_iDstStage;       // destination stage
    PtnId m_tDstPtn;         // destination partition (in grid)

    // info of op_send
    i8    m_iSrcNodeId;        // source node
    i8    m_iCodeLine;       // line in Parcel.cpp
    eYF   m_eParcelForward;  // how parcel is forwarded
    eXK   m_eSendCast;       // broadcast, hashcast, localRobin, robin, unicast
  
    bool  m_bDeleteOperand;
    eSY   m_eSendWhy;        // why was this parcel sent
    eRV   m_eRecvInstructions;   // receiver instructions

    bool    m_bOriginalParcel;
    i8      m_nItems;            // number of items in this parcel
    Object* m_paItems[C_MAX_PARCEL_ITEMS];

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Parcel(bool bOriginalParcel=false);
   ~Parcel(void) noexcept(false);

    static void vRepair(Object* pObject);

  public:
    static void vPostRecv(void* aParcel);

    void vAddItem(Object* pItem);
   
    Stz* szDemonNm(void);
    void vDemonNm(Stz* szDemonNm);
    void vDistribute(Op* pSendOp);
    i8   iDstStage(void);
    i8   iDstNode(void);
    PtnId tDstPtnId(void);
    PtnId tDstPtnId(i8 i_BktNo);
  
    Object*  pGetItem(i8 i_Item);
    Operand* pGetOperand(void);
    
    void vLabel(i8 iPipelineJobId, eYF eParcelForward);
  
   
    bool bOriginalParcel(void);
    void vOriginalParcel(bool bOriginalParcel);

    i8   nItems(void);

    i8   iPipelineJobId(void);
  
    void vRecvContents(Socket* pSocket);
    eRV  eRecvInstructions(void);
    void vRecvInstructions(eRV eRecvInstructions);
    Operand* pRemoveOperand(void);
    void vReset(void);

  private:

    void vAppraise_Forward_cast(Op* pSendOp);
  
    void vDistributeBy_Forward_cast(Op* pSendOp);
  
    void vNDE_AllNodes(Op* pSendOp);
    void vNDE_NextNode(Op* pSendOp);
    void vNDE_OriginNode(Op* pSendOp);
    void vNDE_OtherNode_TBD(Op* pSendOp);
    void vNDE_SameNode(Op* pSendOp);
  
    void vPTN_broadcast(Op* pSendOp);
    void vPTN_hashcast(Op* pSendOp);
    void vPTN_fancast(Op* pSendOp);
    void vPTN_robinLocal(Op* pSendOp);
    void vPTN_robinDist(Op* pSendOp);
    void vPTN_unicast(Op* pSendOp);

    void vDispatchParcel(Op *pSendOp, i8 iCodeLine,
                         i8 iDstNode, Stage* pDstStage, PtnId tDstPtnId);
  
    void vPipeBufferViaQueues(Op* pSendOp, Stage* pDstStage);
    void vPipeOperandViaQueues(Op* pSendOp, Stage* pDstStage);
    void vSendParcelViaSocket(Op* pSendOp);

  public:
    Footer* aFooter(void);
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Parcel* _pParcel(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
