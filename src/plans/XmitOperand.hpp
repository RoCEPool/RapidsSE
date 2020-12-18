/*============================================================================*/
/* XmitInfo.hpp -- XmitInfo                                                   */
/*============================================================================*/
#pragma once

namespace Plans
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class XmitOperand : public Object
  {
  public:
    Operand*   m_pOperand;        // operand
    
    i8         m_iPipelineJobId;  // pipeline job id
    i8         m_i_Node;          // node id
    tPN        m_tPtnNo;          // partition (in grid)
    ePX        m_eCrossTopo;      // cross topology
    i8         m_iNextStage;      // next stage
    i8         m_iNextOp;         // nextOp in the nextStage
    
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    XmitOperand(void);
    ~XmitOperand(void) noexcept(false);
    
    static XmitOperand* pNewXmitOperand(Operand* pOperand, i8 iParentJobId,
                                        Op* pOp, i8 i_Node, tPN tPtnNo);
    
  public:
    void vShow(void);
    
  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  XmitOperand* _pXmitOperand(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
