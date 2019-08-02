/*============================================================================*/
/* MergeOp.hpp -- Merge                                                       */
/*============================================================================*/
#pragma once

namespace PushDown
{
  const i8 C_OPERANDS = 32;
  
  class MergeOp : public Op
  {
  private:
  
    Operand* m_pOperand[C_OPERANDS];
    
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    MergeOp(Stz* szOpLabel, Jobs::Job* pJob);
   ~MergeOp(void) noexcept(false);
  
    static eOR eCallLoop(void* aThis);
    
  private:
    i8 iCalculateCardinality(i8 iCount);
    eOR eMergeLoop(void);
    BufferCursor* pPartitionBuffer(IntBuffer* pIntBuffer, BufferCursor* pLowCursor,
                                   BufferCursor* pHighCursor);
    void vQuickSortBuffer(IntBuffer* pIntBuffer, BufferCursor* pLowCursor, BufferCursor* pHighCursor);
    Footer* aFooter(void);
  };

  MergeOp* _pMergeOp(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
