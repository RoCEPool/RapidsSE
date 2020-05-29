/*============================================================================*/
/* FilterOp.hpp -- Where                                                       */
/*============================================================================*/
#pragma once

namespace DataOperators
{
  class FilterOp : public Op
  {
  private:
  
    TokenVec*   m_pTokenVec;
    i8          m_iStartTkn;
    i8          m_iEndTkn;
    Calculator* m_pCalculator;
    IrwBuffer*  m_pIrwBuffer;
    
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    FilterOp(OpSpec* pOpSpec, Pipeline* pPipeline);
    ~FilterOp(void) noexcept(false);
    
    static Op* pNew(OpSpec* pOpSpec, Pipeline* pPipeline);
    static eOR eCall(void* aThis);
    static eOR eDrain(void* aThis);
  
    eOR eOpLoop(void);
    eOR eDrainLoop(void);
    
  private:
    Footer* aFooter(void);
  };

  FilterOp* _pFilterOp(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
