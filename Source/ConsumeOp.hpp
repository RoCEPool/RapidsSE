/*============================================================================*/
/* ConsumeOp.hpp -- Consume                                                   */
/*============================================================================*/
#pragma once

namespace DataOperators
{
  class ConsumeOp : public Op
  {
  private:
    
    Operand*  m_pFirstOperand;
    
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    ConsumeOp(OpSpec* pOpSpec, Pipeline* pPipeline);
   ~ConsumeOp(void) noexcept(false);
  
    static Op* pNew(OpSpec* pOpSpec, Pipeline* pPipeline);
    static eOR eCall(void* aThis);
    static eOR eDrain(void* aThis);

    eOR eOpLoop(void);
    eOR eDrainLoop(void);

  private:
    Footer* aFooter(void);
  };

  ConsumeOp* _pConsumeOp(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
