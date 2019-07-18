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
  
    OperandQueue* m_pOperandQueue; // the queue being processed
  
    bool (*m_fpOpFun)(void* aThis);  // function which does the op
  
    Op* m_pOpOut;  // where results get queued (piping)
  
    Operand* m_pOperand; // current operand
  
    Tasks::Task* m_pExecutingTask;   // task executing this op
  
  public:
    Op(eCN eClass, Jobs::Job* pJob);
   ~Op(void) noexcept(false);
  
    static Op* pThisTLSOp(void);

    // - - - - - - - - - - - - - - - -
    void vOpLoop(void);
  
    // - - - - - - - - - - - - - - - -
    void vAppendIn(Operand* pOperand);
    void vAppendOut(Operand* pOperand);
    void vCallOpFun(void);
    bool bGetNextOperand(void);
    void vOpOut(Op* pOpOut);
  
    void vJob(Jobs::Job* pJob);
  };

  Op* _pOp(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
