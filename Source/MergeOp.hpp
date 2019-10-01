/*============================================================================*/
/* MergeOp.hpp -- Merge                                                       */
/*============================================================================*/
#pragma once

namespace DataOperators
{
  const i8 C_OPERANDS = 32;
  
  class MergeOp : public Op
  {
  private:
  
    Operand* m_pOperand[C_OPERANDS];
    i8       m_iOperands;
    
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    MergeOp(OpSpec* pOpSpec, Pipeline* pPipeline);
   ~MergeOp(void) noexcept(false);
  
    static Op* pNew(OpSpec* pOpSpec, Pipeline* pPipeline);
    
    static eOR eCall(void* aThis);
    
  private:
    void vMergeDangling(void);
    eOR  eMergeLoop(void);
    void vMergeOperands(Operand* pOperand1, Operand* pOperand2);
  
    Footer* aFooter(void);
  };

  MergeOp* _pMergeOp(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
