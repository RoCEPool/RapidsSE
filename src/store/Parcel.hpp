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
    i8  m_iPipelineJobId;  // pipeline job id
    i8  m_iDstNode;        // destination node id
    tPN m_tPtnNo;          // partition (in grid)
    ePX m_eCrossTopo;      // cross topology
    eXK m_eSendKind;       // hashed, localRobin, normal, robin
    i8  m_iNextStage;      // next stage
    i8  m_iNextOp;         // nextOp in the nextStage
    
  private:
    i8 m_nObjects;                        // number of objects in the parcel
    Object* m_pObjects[C_MAX_PARCEL_OBJ];  // let's for now, a fixed size array of
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Parcel(Operand* pOperand,
           i8       iPipelineJobId,
           i8       iDstNode,
           tPN      tPtnNo,
           ePX      eCrossTopo
           );
  
   ~Parcel(void) noexcept(false);
        

  public:
    void vShow(void);


    void vAddObject(Object *pObj);
    i8 nObjects(void) { return m_nObjects;};
    Object* pGetObject(i8 iIdx);
    Operand* pGetOperand(void);
    void vSendOut(Op* pSrcOp);
    void vSetOperand(Operand *pOperand);
  
  
  
  private:
  
    void vSendParcel(Op* pSrcOp, i8 iDstNode, tPN tDstPtn);


    void vSendToAllNodes(Op* pSrcOp);
  
    void vSendToAllOriginPartitions(Op* pSrcOp);
  
    void vSendToAllPartitions(Op* pSrcOp);
  
    void vSendToNode(Op* pSrcOp, i8 iDstNode);
  
    void vSendToOrigin(Op* pSrcOp);
  
    void vSendToPartition(Op* pSrcOp, tPN tDstPtn);

  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Parcel* _pParcel(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
