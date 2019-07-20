/*============================================================================*/
/* Op.hpp -- Operator                                                         */
/*============================================================================*/
#pragma once

namespace PushDown
{
  enum eOZ // Op State
  {
  /* 0 */ OZ_NONE=0,
  /* 1 */ OZ_BIDING,     // op is dormant, waiting for operand
  /* 2 */ OZ_PROCESSING, // op is processing an operand
  /* 3 */ OZ_ENUMS
  };
  
  static Stz* aOZ[]
  {
  /* 0 */ "NONE"_Stz,
  /* 1 */ "BIDING"_Stz,
  /* 2 */ "PROCESSING"_Stz,
  /* 3 */ "ENUMS"_Stz,
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  class Op : public Object
  {
  protected:
    s32  m_sOpLabel;    // label of this op
    Jobs::Job* m_pJob;  // job this operator belongs to
  
    bool m_bAckJobEnd;  // ack job on end marker
    bool (*m_fpOpFun)(void* aThis);  // function which does the op
    Op*  m_pOpOut;                   // where results get queued (piping)

    Lock m_okLock;      // op lock
    eOZ  m_eOpState;    // state of this op
    OperandQueue* m_pOperandQueue;   // the input queue being processed
  
    Tasks::Task*  m_pTask;           // task running this op

  public:
    void  operator delete(void*);
  
    Op(eCN eClass, Stz* szOpLabel, Jobs::Job* pJob);
   ~Op(void) noexcept(false);
  
    static Op* pThisTLSOp(void);

    // - - - - - - - - - - - - - - - -
    void vOpLoop(void);
  
    // - - - - - - - - - - - - - - - -
    void vAckJobEnd(bool m_bAckJobEnd);
    void vAppendIn(Operand* pOperand);
    void vAppendOut(Operand* pOperand);
    void vAppendOut_endMarker(void);
    void vBideOperand(void);
    void vDropLock(void);
    void vGrabLock(void);
    bool bIsLocked(void);
    void vOpOut(Op* pOpOut);
    eOZ  eOpState(void);
    void vOpState(eOZ eOpState);
    Operand* pPopOperand(void);
    Tasks::Task* pTask(void);
    void vTask(Tasks::Task* pTask);
  };

  Op* _pOp(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
