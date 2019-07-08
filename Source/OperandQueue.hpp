/*============================================================================*/
/* OperandQueue.hpp -- queue of operand buffers to be processed by an op      */
/*============================================================================*/
#pragma once

namespace PushDown
{
  class OperandQueue : public ListNL_t<OperandBuffer>
  {
  private:
    Lock m_okLock;
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    OperandQueue(void);
    ~OperandQueue(void) noexcept(false);
    
    void vAppendOperand(OperandBuffer* pOperand);
    void vDropLock(void);
    void vGrabLock(void);
    bool bGrabLock(void);
    bool bIsLocked(void);
    OperandBuffer*  pPeekOperand(void);
    OperandBuffer*  pPopOperand(void);
    i8   nQueuedOperands(void);
  
    void vContentDump(void);
    
    Footer* aFooter(void);
  };
  
  OperandQueue* _pOperandQueue(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
