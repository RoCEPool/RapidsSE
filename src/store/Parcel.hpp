/*============================================================================*/
/* Parcel.hpp -- Send Parcel                                                  */
/*============================================================================*/
#pragma once

#include "eXK.h"
#include "eYO.h"

namespace STORE
{
  class Parcel : public Operand
  {
  private:
    eYO   m_eIntraTopology;  // op topology
    eYX   m_eCrossTopology;  // cross topology
    eXK   m_eSendKind;       // broadcast, hashcast, localRobin, normal, robin

    i8    m_iDstNode;        // destination node id
    i8    m_iDstStage;       // destination stage
    PtnId m_tDstPtn;         // destination partition (in grid)

    bool m_bOriginalParcel;
  
    i8   m_iPipelineJobId;  // pipeline job id


    i8 m_nItems;            // number of items in this parcel
    Object* m_pItems[C_MAX_PARCEL_ITEMS];
  
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
    i8   iDstNode(void);
    //i8   iDstOp(void);

    PtnId  tDstPtn(void);
    PtnId  tDstPtn(i8 i_BktNo);

    i8   iDstStage(void);
    Object* pGetItem(i8 i_Item);
    Operand* pGetOperand(void);
    void vLabel(eYX eCrossTopology, eYO eIntraTopology, i8  iPipelineJobId);
    i8   nItems(void);
    eYO  eIntraTopology(void);
    bool bOriginalParcel(void);
    void vOriginalParcel(bool bOriginalParcel);
    i8   iPipelineJobId(void);
    void vRecvContents(Socket* pSocket);
    void vReset(void);
    eXK  eSendKind(void);
    void vSendOut(Op* pSrcOp);

  private:

    void vAppraiseSendKind(Op* pSrcOp);
    void vSendCrossTopology(Op* pSrcOp);
  
    void vSendToAllNodes(Op* pSrcOp);
    void vSendToAllOriginPartitions(Op* pSrcOp);
    void vSendToAllPartitions(Op* pSrcOp);
    void vSendToNode(Op* pSrcOp);
    void vSendToOrigin(Op* pSrcOp);
    void vSendToPartition(Op* pSrcOp);
    void vSendToPartition_hashcast(Op* pSrcOp);
    void vSendToPartition_robin(Op* pSrcOp);
    void vSendToPartition_unicast(Op* pSrcOp);
 
    void vSetItem(i8 i_Item, Object* pItem);

    void vSendParcel(Op *pSrcOp, i8 iDstNode,
                     i8 iDstStage, Stage* pDstStage, PtnId tDstPtn);
    void vSocketSendOfParcel(Op* pSrcOp);

  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Parcel* _pParcel(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
