/*============================================================================*/
/* OperandQueue.hpp -- queue of operand buffers to be processed by an op      */
/*============================================================================*/
#pragma once

namespace BASE
{
  class OperandQueue : public Queue
  {
  private:

    bool m_bLatch;

    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    OperandQueue(void);
    ~OperandQueue(void) noexcept(false);
    
    i8        iAppendOperand(Operand* pOperand);
    void      vLatch(bool bLatch);
    bool      bPeekOoO(void);
    bool      bPeekQueue(void);
    Operand*  pPeekOoO(void);
    Operand*  pPeekQueue(void);
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
