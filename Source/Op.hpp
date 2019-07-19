/*============================================================================*/
/* Op.hpp -- Operator                                                         */
/*============================================================================*/
#pragma once

namespace PushDown
{
  class Op : public Object
  {
  friend Tasks::Task;
  
  protected:
    Jobs::Job* m_pJob;  // job this operator belongs to
    s32  m_sOpLabel;    // label of this op
    bool m_bAckJobEnd;  // ack job on end marker
  
    OperandQueue* m_pOperandQueue;   // the input queue being processed
    bool (*m_fpOpFun)(void* aThis);  // function which does the op
    Op*  m_pOpOut;                    // where results get queued (piping)
  
  public:
    void  operator delete(void*);
  
    Op(eCN eClass, Jobs::Job* pJob, Stz* szOpLabel);
   ~Op(void) noexcept(false);
  
    static Op* pThisTLSOp(void);

    // - - - - - - - - - - - - - - - -
    void vOpLoop(void);
  
    // - - - - - - - - - - - - - - - -
    void vAckJobEnd(bool m_bAckJobEnd);
    void vAppendIn(Operand* pOperand);
    void vAppendOut(Operand* pOperand);
    void vAppendOut_endMarker(void);
    void vOpOut(Op* pOpOut);
  };

  Op* _pOp(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
