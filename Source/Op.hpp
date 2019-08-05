/*============================================================================*/
/* Op.hpp -- Operator                                                         */
/*============================================================================*/
#pragma once

namespace PushDown
{
  class Op : public Object
  {
  friend class Digest;
  
  protected:
    s32            m_sLabel;         // label of this op
    s256           m_sArgs;          // args of this op   (initial dev only)
    s32            m_sNextLabel;     // label of this op
  
    OpDefn*        m_aOpDefn;        // op definition
    Digest*        m_pDigest;        // op digest
    Job*           m_pJob;           // job this operator belongs to
    Task*          m_pTask;          // task running this op
  
    bool           m_bAckJobEnd;     // ack job on end marker
    bool           m_bPulsed;        // is this a pulsed operand?
    i8             m_iWorkQuanta;    // quanta of work between pulses
    i8             m_iWorkDone;      // work done

    OperandQueue*  m_pOperandQueue;  // the input queue being processed
    OperandQueue*  m_pOperandQueue2; // the input queue being processed
    Op*            m_pOpOut;         // where results get piped (queued)
    Op*            m_pOpContinue;    // op which gets continued

    eOR  (*m_fpCall)(void* aThis);   // function which calls the op loop
    void (*m_fpDrain)(void* aThis);  // function which drains the operand queue

    //! need to add pulsing and cancelling
  
  public:
    void  operator delete(void*);
  
    Op(eCN eClass, Stz* szOpLabel, Digest* pDigest, OpDefn* aOpDefn);
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
    bool bPause(void);
    void vPipeOut(Operand* pOperand);
    void vPipeOut_endMarker(void);
    void vPipeSelf(Operand* pOperand);
    Operand* pPopOperand(void);
    bool bIsCancelling(void);
    void vStdDrain(void);
    Task* pTask(void);
    void vTask(Task* pTask);
    void vIncWorkDone(void);
    void vWorkQuanta(i8 iWorkQuanta);
  };

  Op* _pOp(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
