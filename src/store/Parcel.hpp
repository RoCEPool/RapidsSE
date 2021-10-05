/*============================================================================*/
/* Parcel.hpp -- Send Parcel                                                  */
/*============================================================================*/
#pragma once

#include "eXK.h"
#include "ePZ.h"

namespace STORE
{
  class Parcel : public Operand
  {
  private:
    eYX  m_eCrossTopo;      // cross topology
    ePZ  m_eOpTopology;     // op topology

    i8   m_iDstNode;        // destination node id

    PtnId  m_tDstPtn;       // destination partition (in grid)

    i8   m_iDstStage;       // destination stage
    i8   m_iDstOp;          // destination op in the destination stage

    bool m_bOriginalParcel;
  
    i8   m_iPipelineJobId;  // pipeline job id

    eXK  m_eSendKind;       // broadcast, hashcast, localRobin, normal, robin

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
    eYX  eCrossTopo(void);
    i8   iDstNode(void);
    i8   iDstOp(void);

    PtnId  tDstPtn(void);
    PtnId  tDstPtn(i8 i_BktNo);

    i8   iDstStage(void);
    Object* pGetItem(i8 i_Item);
    Operand* pGetOperand(void);
    void vLabel(eYX eCrossTopo, ePZ eOpTopology, i8  iPipelineJobId);
    i8   nItems(void);
    ePZ  eOpTopology(void);
    bool bOriginalParcel(void);
    void vOriginalParcel(bool bOriginalParcel);
    i8   iPipelineJobId(void);
    void vRecvContents(Socket* pSocket);
    void vReset(void);
    eXK  eSendKind(void);
    void vSendOut(Op* pSrcOp);

  private:

    void vSendOverSocket(Op* pSrcOp);

    void vAppraise(Op* pSrcOp, PtnId& tDstPtn, eYX& eCrossTopo, Stage*& pNextStage);
    void vSendCross(Op* pSrcOp, PtnId tDstPtn, eYX eCrossTopo, Stage* pNextStage);
    void vSendParcel(Op *pSrcOp, i8 iDstNode, PtnId tDstPtn);
    void vSendToPartition(Op* pSrcOp, PtnId tDstPtn);

    void vSendToAllNodes(Op* pSrcOp);
    void vSendToAllOriginPartitions(Op* pSrcOp);
    void vSendToAllPartitions(Op* pSrcOp);
    void vSendToNode(Op* pSrcOp, i8 iDstNode);
    void vSendToOrigin(Op* pSrcOp);
    void vSetItem(i8 i_Item, Object* pItem);

  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Parcel* _pParcel(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
