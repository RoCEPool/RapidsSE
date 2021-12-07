/*============================================================================*/
/* Parcel.hpp -- Send Parcel                                                  */
/*============================================================================*/
#pragma once

#include "eXK.h"
#include "eYS.h"
#include "eYG.h"
#include "eYF.h"

namespace STORE
{
  class Parcel : public Operand
  {
  private:
    i8    m_iPipelineJobId;  // pipeline job id

    i8    m_iDstNode;        // destination node id
    i8    m_iDstStage;       // destination stage
    PtnId m_tDstPtn;         // destination partition (in grid)

    // info of op_send
    i8    m_iSrcNode;        // source node
    i8    m_iCodeLine;       // line in Parcel.cpp
    eYF   m_eParcelForward;  // how parcel is forwarded
    eYX   m_eCrossTopology;  // cross topology
    eXK   m_eSendCast;       // broadcast, hashcast, localRobin, robin, unicast
  
    bool  m_bDeleteOperand;
    eRV   m_eRecvInstructions;   // receiver instructions

    bool    m_bOriginalParcel;
    i8      m_nItems;            // number of items in this parcel
    Object* m_paItems[C_MAX_PARCEL_ITEMS];

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Parcel(void);
    ~Parcel(void) noexcept(false);

    static void vRepair(Object* pObject);

  public:
    static void vPostRecv(void* aParcel);

    void vAddItem(Object* pItem);
  
    eYX  eCrossTopology(void);
 
    void vDistribute(Op* pSendOp);
    i8   iDstStage(void);
    i8   iDstNode(void);
    PtnId  tDstPtn(void);
    PtnId  tDstPtn(i8 i_BktNo);
  
    Object* pGetItem(i8 i_Item);
    Operand* pGetOperand(void);
    
    void vLabel(i8 iPipelineJobId, eYF eParcelForward, eYX eCrossTopology);
  
   
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

    void vAppraiseSendCast(Op* pSendOp);
  
    void vDistributeBy_Forward_cast(Op* pSendOp);
  
    void vFwdTo_AllNodes(Op* pSendOp);
    void vFwdTo_AllPartitions(Op* pSendOp);
    void vFwdTo_AllPartitions_broadcast(Op* pSendOp);
    void vFwdTo_AllPartitions_hashcast(Op* pSendOp);
    void vFwdTo_AllPartitions_localrobin(Op* pSendOp);
    void vFwdTo_AllPartitions_robin(Op* pSendOp);
    void vFwdTo_AllPartitions_unicast(Op* pSendOp);
    void vFwdTo_AllPartitionsOnOrigin(Op* pSendOp);
    void vFwdTo_AllPartitionsOnOrigin_robin(Op* pSendOp);
    void vFwdTo_AllPartitionsOnOrigin_unicast(Op* pSendOp);
    void vFwdTo_NextNode(Op* pSendOp);
    void vFwdTo_Origin(Op* pSendOp);
    void vFwdTo_OtherNode_TBD(Op* pSendOp);
    void vFwdTo_SameNode(Op* pSendOp);

    void vFwdBufferTo_OnePtn(Op* pSendOp);
    void vFwdBufferTo_OnePtn_hashcast(Op* pSendOp);
    void vFwdBufferTo_OnePtn_robin(Op* pSendOp);
    void vFwdBufferTo_OnePtn_unicast(Op* pSendOp);

    void vDispatchParcel(Op *pSendOp, i8 iCodeLine,
                         i8 iDstNode, Stage* pDstStage, PtnId tDstPtn);
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
