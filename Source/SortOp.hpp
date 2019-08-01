/*============================================================================*/
/* SortOp.hpp -- Sort                                                        */
/*============================================================================*/
#pragma once

namespace PushDown
{
  class SortOp : public Op
  {
  private:
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    SortOp(Stz* szOpLabel, Jobs::Job* pJob);
   ~SortOp(void) noexcept(false);
  
    static eOR eCallLoop(void* aThis);
    
  private:
    BufferCursor* pPartitionBuffer(IntBuffer* pIntBuffer, BufferCursor* pLowCursor,
                                           BufferCursor* pHighCursor);
    void vQuickSortBuffer(IntBuffer* pIntBuffer, BufferCursor* pLowCursor, BufferCursor* pHighCursor);
    eOR eSortLoop(void);
    Footer* aFooter(void);
  };

  SortOp* _pSortOp(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
