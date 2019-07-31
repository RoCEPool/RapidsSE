/*============================================================================*/
/* Op.hpp -- Operator                                                         */
/*============================================================================*/
#pragma once

namespace PushDown
{
  class Op : public Object
  {
  protected:
    s32            m_sOpLabel;       // label of this op
    Jobs::Job*     m_pJob;           // job this operator belongs to
    Tasks::Task*   m_pTask;          // task running this op
  
    bool           m_bAckJobEnd;     // ack job on end marker
    OperandQueue*  m_pOperandQueue;  // the input queue being processed
    i8             m_iWorkQuanta;    // quanta of work between pulses
    Op*            m_pOpOut;         // where results get piped (queued)
    Op*            m_pOpContinue;    // op which gets continued

    eOR  (*m_fpOpFun)(void* aThis);  // function which does the op
    void (*m_fpDrain)(void* aThis);  // function which drains the operand queue

    //! need to add pulsing and cancelling
  
  public:
    void  operator delete(void*);
  
    Op(eCN eClass, Stz* szOpLabel, Jobs::Job* pJob);
   ~Op(void) noexcept(false);
  
    static Op* pThisTLSOp(void);

    // - - - - - - - - - - - - - - - -
    void vOpLoop(void);
  
    // - - - - - - - - - - - - - - - -
    void vAckJob(void);
    void vAckJobEnd(bool m_bAckJobEnd);
    void vAppendOperand(Operand* pOperand);
    void vDropTaskLock(void);
    void vGrabTaskLock(void);
    bool bIsLocked(void);
    void vOpOut(Op* pOpOut);
    void vPipeOut(Operand* pOperand);
    void vPipeOut_endMarker(void);
    Operand* pPopOperand(void);
    void vStdDrain(void);
    Tasks::Task* pTask(void);
    void vTask(Tasks::Task* pTask);
    void vWorkQuanta(i8 iWorkQuanta);
  };

  Op* _pOp(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
