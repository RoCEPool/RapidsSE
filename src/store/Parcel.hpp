/*============================================================================*/
/* Parcel.hpp -- Send Parcel                                                  */
/*============================================================================*/
#pragma once

#include "eXK.h"

namespace STORE
{
  static const i8 C_MAX_PARCEL_OBJ = 16;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Parcel : public Object
  {
    
  public:
    i8         m_iPipelineJobId;  // pipeline job id
    i8         m_iDstNode;        // destination node id
    tPN        m_tPtnNo;          // partition (in grid)
    ePX        m_eCrossTopo;      // cross topology
    i8         m_iNextStage;      // next stage
    i8         m_iNextOp;         // nextOp in the nextStage
    
  private:
    i8 m_nObjects;                        // number of objects in the parcel
    Object* m_pObjects[C_MAX_PARCEL_OBJ];  // let's for now, a fixed size array of
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Parcel(Operand* pOperand,
           i8 iPipelineJobId,
           i8 iDstNode,
           ePX eCrossTopo,
           i8 iNextStage,
           i8 iNextOp,
           tPN tPtnNo);
    ~Parcel(void) noexcept(false);
        
    static Parcel* pNewParcel(Operand* pOperand, Op*  pSrcOp);

    static void vSendOut(Parcel* pParcel, Operand* pOperand, Op* pOp);

  public:
    void vShow(void);


    void vAddObject(Object *pObj);
    i8 nObjects(void) { return m_nObjects;};
    Object* pGetObject(i8 iIdx);
    Operand* pGetOperand(void);
    void vSetOperand(Operand *pOperand);
  
  
  
  private:

    static void vSendAsParcel(Parcel* pParcel, Op* pSrcOp);
  
    static void vSendAsParcel(Parcel* pParcel, Operand* pOperand,
                              Op* pSrcOp, i8 iDstNode, tPN tPtnNo);


    static void vSendToAllNodes(Parcel* pParcel, Operand* pOperand,
                                Op* pSrcOp);
  
    static void vSendToAllOriginPartitions(Parcel* pParcel, Operand* pOperand,
                                           Op* pSrcOp, eXK eSendKind);
  
    static void vSendToAllPartitions(Parcel* pParcel, Operand* pOperand,
                                     Op* pSrcOp, eXK eSendKind);
  
    static void vSendToNode(Parcel* pParcel, Operand* pOperand,
                            Op* pSrcOp, i8 iDstNode);
  
    static void vSendToOrigin(Parcel* pParcel, Operand* pOperand,
                              Op* pSrcOp);
  
    static void vSendToPartition(Parcel* pParcel, Operand* pOperand,
                                 Op* pSrcOp, eXK eSendKind, tPN tPtnNo);

  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Parcel* _pParcel(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
