/*============================================================================*/
/* Op.hpp -- Operator                                                         */
/*============================================================================*/
#pragma once

namespace PushDown
{
  class Op : public Object
  {
  private:
    Job* m_pJob;  // job this operator belongs to
  
    OperandQueue* m_pOperandQueue; // the queue being processed
  
    bool (*m_fpOpEntryPt)(Operand* pOperand);  // function which does the op
  
    Op* m_pOpOut;  // where results get queued
  
    Operand* m_pOperand; // current operand
  
  public:
    Op(eCN eClass);
   ~Op(void) noexcept(false);
  
    void vOpLoop(void);
  
    void vCallOpFun(void);
    bool bGetNextOperand(void);
  };

  Op* _pOp(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
