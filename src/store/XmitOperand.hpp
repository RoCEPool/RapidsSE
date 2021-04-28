/*============================================================================*/
/* XmitInfo.hpp -- XmitInfo                                                   */
/*============================================================================*/
#pragma once

namespace STORE
{
  static const i8 C_MAX_XMIT_OBJ = 16;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class XmitOperand : public Object
  {
    
  public:
    i8         m_iPipelineJobId;  // pipeline job id
    i8         m_i_Node;          // node id
    tPN        m_tPtnNo;          // partition (in grid)
    ePX        m_eCrossTopo;      // cross topology
    i8         m_iNextStage;      // next stage
    i8         m_iNextOp;         // nextOp in the nextStage
    
  private:
    i8 m_nObjects;                        // number of objects in the parcel
    Object* m_pObjects[C_MAX_XMIT_OBJ];  // let's for now, a fixed size array of
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    XmitOperand(void);
    ~XmitOperand(void) noexcept(false);
    
    static XmitOperand* pNewXmitOperand(Operand* pOperand,
                                        i8 iParentJobId,
                                        ePX eCrossTopo,
                                        i8 iNextStage,
                                        i8 iNextOp,
                                        i8 i_Node,
                                        tPN tPtnNo);
    
  public:
    void vShow(void);


    void vAddObject(Object *pObj);
    i8 nObjects(void) { return m_nObjects;};
    Object* pGetObject(i8 iIdx);
    Operand* pGetOperand(void);
    void vSetOperand(Operand *pOperand);

  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  XmitOperand* _pXmitOperand(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
