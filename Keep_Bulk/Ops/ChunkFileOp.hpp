/*============================================================================*/
/* ChunkFileOp.hpp -- chunkFile op                                            */
/*============================================================================*/
#pragma once

namespace DataOperators
{
  class ChunkFileOp : public Op
  {
  private:
    
    
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    ChunkFileOp(OpSpec* pOpSpec, Pipeline* pPipeline);
   ~ChunkFileOp(void) noexcept(false);
  
    static Op* pNew(OpSpec* pOpSpec, Pipeline* pPipeline);
    static eOR eCall(void* aThis);
    static eOR eDrain(void* aThis);

    eOR eOpLoop(void);
    eOR eDrainLoop(void);

  private:
    Footer* aFooter(void);
  };

  ChunkFileOp* _pChunkFileOp(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
