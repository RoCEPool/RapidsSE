/*============================================================================*/
/* SuperMergeOp.hpp -- Super Merge                                            */
/*============================================================================*/
#pragma once

namespace DataOperators
{
  
  class SuperMergeOp : public Op
  {
  private:
  
    i8       m_nLimit;     
    Operand* m_pOperand[C_MAX_SLOTS];
    i8       m_iOperands;
    
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    SuperMergeOp(OpSpec* pOpSpec, Pipeline* pPipeline);
   ~SuperMergeOp(void) noexcept(false);
  
    static Op* pNew(OpSpec* pOpSpec, Pipeline* pPipeline);
    
    static eOR eCall(void* aThis);
    
  private:
    SrtBuffer* pMergeAll(void);
    eOR        eMergeLoop(void);
    SrtBuffer* pSuperMergeOperands(Operand* pOperand1, Operand* pOperand2);
  
    Footer* aFooter(void);
  };

  SuperMergeOp* _pSuperMergeOp(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
