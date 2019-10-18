/*============================================================================*/
/* StartOp.hpp -- Output                                                     */
/*============================================================================*/
#pragma once

namespace DataOperators
{
  class StartOp : public Op
  {
  private:
    
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    StartOp(OpSpec* pOpSpec, Pipeline* pPipeline);
   ~StartOp(void) noexcept(false);
  
    static Op* pNew(OpSpec* pOpSpec, Pipeline* pPipeline);
    static eOR eCall(void* aThis);

    eOR eStartLoop(void);

  private:
    Footer* aFooter(void);
  };

  StartOp* _pStartOp(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
