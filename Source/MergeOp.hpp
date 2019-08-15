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
    
    MergeOp(Stz* szOpLabel, Digest* pDigest);
   ~MergeOp(void) noexcept(false);
  
    static eOR eCallLoop(void* aThis);
    
  private:
    void vMergeDangling(void);
    eOR  eMergeLoop(void);
    void vMergeOperands(Operand* pOperand1, Operand* pOperand2);
    
    BufferCursor* pPartitionBuffer(SrtBuffer* pSrtBuffer, BufferCursor* pLowCursor,
                                   BufferCursor* pHighCursor);
    void vQuickSortBuffer(SrtBuffer* pSrtBuffer, BufferCursor* pLowCursor, BufferCursor* pHighCursor);
    Footer* aFooter(void);
  };

  MergeOp* _pMergeOp(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
