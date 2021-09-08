/*============================================================================*/
/* OperandQueue.hpp -- queue of operand buffers to be processed by an op      */
/*============================================================================*/
#pragma once

namespace BASE
{
  class OperandQueue : public Queue
  {
  private:
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    OperandQueue(void);
    ~OperandQueue(void) noexcept(false);
    
    void      vAppendOperand(Operand* pOperand);
    bool      bPeekOoO(void);
    bool      bPeekQueue(void);
    Operand*  pPopOoO(void);
    Operand*  pPopQueue(void);
    i8        nQueuedOperands(void);
    i8        nMaxOperands(void);
      
    Footer* aFooter(void);
  };
  
  OperandQueue* _pOperandQueue(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
