/*============================================================================*/
/* LimitOp.hpp -- Limit                                                       */
/*============================================================================*/
#pragma once

namespace DataOperators
{
  class LimitOp : public Op
  {
  private:
  
    i8         m_nLimit;     // what is the limit?
    SrtBuffer* m_pSrtBuffer; // sort buffer
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    LimitOp(OpSpec* pOpSpec, Pipeline* pPipeline);
   ~LimitOp(void) noexcept(false);
  
    static Op* pNew(OpSpec* pOpSpec, Pipeline* pPipeline);
    static eOR eCall(void* aThis);
    static eOR eDrain(void* aThis);

    eOR  eOpLoop(void);
    bool bLimitDone(void);
    eOR  eDrainLoop(void);

  private:
    Footer* aFooter(void);
  };

  LimitOp* _pLimitOp(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
