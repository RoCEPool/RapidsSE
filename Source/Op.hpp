/*============================================================================*/
/* Op.hpp -- Operator                                                         */
/*============================================================================*/
#pragma once

namespace PushDown
{
  class Op : public Object
  {
  protected:
    s32            m_sLabel;         // label of this op
    s256           m_sArgs;          // args of this op   (initial dev only)
    s32            m_sNextLabel;     // label of this op
  
    Job*           m_pJob;           // job this operator belongs to
    Task*          m_pTask;          // task running this op
  
    bool           m_bAckJobEnd;     // ack job on end marker
    bool           m_bPulsed;        // is this a pulsed operand?
    i8             m_iWorkQuanta;    // quanta of work between pulses

    OperandQueue*  m_pOperandQueue;  // the input queue being processed
    OperandQueue*  m_pOperandQueue2; // the input queue being processed
    Op*            m_pOpOut;         // where results get piped (queued)
    Op*            m_pOpContinue;    // op which gets continued

    eOR  (*m_fpOpFun)(void* aThis);  // function which does the op
    void (*m_fpDrain)(void* aThis);  // function which drains the operand queue

    //! need to add pulsing and cancelling
  
  public:
    void  operator delete(void*);
  
    Op(eCN eClass, OpDesc* aOpDesc, Stz* szOpLabel, Jobs::Job* pJob);
   ~Op(void) noexcept(false);
  
    static Op* pThisTLSOp(void);

    // - - - - - - - - - - - - - - - -
    void vOpLoop(void);
  
    // - - - - - - - - - - - - - - - -
    void vAckJob(void);
    void vAckJobEnd(bool m_bAckJobEnd);
    void vAppendOperand(Operand* pOperand);
    Stz* szArgs(void);
    void vDropTaskLock(void);
    void vGrabTaskLock(void);
    bool bIsLocked(void);
    Stz* szLabel(void);
    void vLabel(Stz* szLabel);
    Stz* szNextLabel(void);
    void vNextLabel(Stz* szNextLabel);
    void vOpOut(Op* pOpOut);
    void vPipeOut(Operand* pOperand);
    void vPipeOut_endMarker(void);
    void vPipeSelf(Operand* pOperand);
    Operand* pPopOperand(void);
    void vStdDrain(void);
    Task* pTask(void);
    void vTask(Task* pTask);
    void vWorkQuanta(i8 iWorkQuanta);
  };

  Op* _pOp(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
