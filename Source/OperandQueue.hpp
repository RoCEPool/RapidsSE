/*============================================================================*/
/* OperandQueue.hpp -- queue of operand buffers to be processed by an op      */
/*============================================================================*/
#pragma once

namespace PushDown
{
  class OperandQueue : public ListNL_t<Operand>
  {
  private:
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    OperandQueue(void);
    ~OperandQueue(void) noexcept(false);
    
    void vAppendOperand(Operand* pOperand);
    Operand*  pPeekOperand(void);
    Operand*  pPopOperand(void);
    i8   nQueuedOperands(void);
      
    Footer* aFooter(void);
  };
  
  OperandQueue* _pOperandQueue(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
