/*============================================================================*/
/* Parcel.hpp -- Send Parcel                                                  */
/*============================================================================*/
#pragma once

#include "eXK.h"

namespace STORE
{
  class Parcel : public Operand
  {
  private:
    ePX  m_eCrossTopo;      // cross topology
  
    i8   m_iDstNode;        // destination node id
    tPN  m_tDstPtn;         // destination partition (in grid)
    i8   m_iDstStage;       // destination stage
    i8   m_iDstOp;          // destination op in the destination stage

    bool m_bOriginalParcel;
  
    i8   m_iPipelineJobId;  // pipeline job id

    eXK  m_eSendKind;       // broadcast, hashcast, localRobin, normal, robin

    i8 m_nItems;                        // number of objects in the parcel
    Object* m_pItems[C_MAX_PARCEL_ITEMS];  // let's for now, a fixed size array of
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Parcel(void);
  
   ~Parcel(void) noexcept(false);

  public:
    static void vPostRecv(void* aParcel);

    void vAddItem(Object* pItem);
  
    ePX  eCrossTopo(void);

    i8  iDstNode(void);
    i8  iDstOp(void);
    tPN tDstPtn(void);
    i8  iDstStage(void);

    Object* pGetItem(i8 i_Item);
    Operand* pGetOperand(void);
    void vLabel(ePX eCrossTopo,
                i8  iDstNode,
                tPN tDstPtn,
                i8  iPipelineJobId);
    i8   nItems(void);
    bool bOriginalParcel(void);
    void vOriginalParcel(bool bOriginalParcel);
    i8   iPipelineJobId(void);
    void vRecvContents(Socket* pSocket);
    void vReset(void);
    eXK  eSendKind(void);
    void vSendOut(Op* pSrcOp);

  private:
    void vAppraise(Op* pSrcOp, tPN& tDstPtn, ePX& eCrossTopo, Stage*& pNextStage);
    void vSendCross(Op* pSrcOp, tPN tDstPtn, ePX eCrossTopo, Stage* pNextStage);
    void vSendOverSocket(Op* pSrcOp);
    void vSendParcel(Op* pSrcOp, i8 iDstNode, tPN tDstPtn);
    void vSendToAllNodes(Op* pSrcOp);
    void vSendToAllOriginPartitions(Op* pSrcOp);
    void vSendToAllPartitions(Op* pSrcOp);
    void vSendToNode(Op* pSrcOp, i8 iDstNode);
    void vSendToOrigin(Op* pSrcOp);
    void vSendToPartition(Op* pSrcOp, tPN tDstPtn);
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
