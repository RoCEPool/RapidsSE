/*============================================================================*/
/* Parcel.hpp -- Send Parcel                                                  */
/*============================================================================*/
#pragma once

#include "eXK.h"

namespace STORE
{
  static const i8 C_MAX_PARCEL_OBJ = 16;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Parcel : public Operand
  {
    
  public:
    i8  m_iPipelineJobId;  // pipeline job id
    ePX m_eCrossTopo;      // cross topology
    eXK m_eSendKind;       // hashed, localRobin, normal, robin
  
    i8  m_iDstNode;        // destination node id
    tPN m_tDstPtn;         // destination partition (in grid)
    i8  m_iDstStage;       // destination stage
    i8  m_iDstOp;          // destination op in the destination stage
    
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
        
  static void vPostRecv(void* aParcel);

  public:
    void vAddObject(Object* pObject);
    Object* pGetObject(i8 i_Object);
    i8 nObjects(void);
    Operand* pGetOperand(void);
    void vRecvContents(Socket* pSocket);
    void vSendOut(Op* pSrcOp);
    void vSetObject(i8 i_Object, Object* pObject);

  private:
    void vSendOverSocket(Op* pSrcOp);
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
