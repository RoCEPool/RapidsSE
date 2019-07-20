/*============================================================================*/
/* Op.hpp -- Operator                                                         */
/*============================================================================*/
#pragma once

namespace PushDown
{
  static const bool B_CALL_AGAIN      = true;
  static const bool B_DONT_CALL_AGAIN = false;

  class Op : public Object
  {
  protected:
    s32            m_sOpLabel;       // label of this op
    Jobs::Job*     m_pJob;           // job this operator belongs to
    Tasks::Task*   m_pTask;          // task running this op
    bool           m_bAckJobEnd;     // ack job on end marker
    OperandQueue*  m_pOperandQueue;  // the input queue being processed
    Op*            m_pOpOut;         // where results get piped (queued)

    bool (*m_fpOpFun)(void* aThis);  // function which does the op
  
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
    void vAppendIn(Operand* pOperand);
    void vAppendOut(Operand* pOperand);
    void vAppendOut_endMarker(void);
    void vBideOperand(void);
    void vDropLock(void);
    void vGrabLock(void);
    bool bIsLocked(void);
    void vOpOut(Op* pOpOut);
    Operand* pPopOperand(void);
    Tasks::Task* pTask(void);
    void vTask(Tasks::Task* pTask);
  };

  Op* _pOp(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
