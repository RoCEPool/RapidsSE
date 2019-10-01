/*============================================================================*/
/* ToCsvOp.hpp -- convert to csv operator                                     */
/*============================================================================*/
#pragma once

namespace DataOperators
{
  class ToCsvOp : public Op
  {
  private:
    
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    ToCsvOp(OpSpec* pOpSpec, Pipeline* pPipeline);
   ~ToCsvOp(void) noexcept(false);
  
    static Op* pNew(OpSpec* pOpSpec, Pipeline* pPipeline);
    static eOR eCall(void* aThis);
    static eOR eDrain(void* aThis);

    eOR eOpLoop(void);
    eOR eDrainLoop(void);

  private:
    Footer* aFooter(void);
  };

  ToCsvOp* _pToCsvOp(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
