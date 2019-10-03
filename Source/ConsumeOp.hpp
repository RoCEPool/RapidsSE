/*============================================================================*/
/* ConsumeOp.hpp -- Consume                                                   */
/*============================================================================*/
#pragma once

namespace DataOperators
{
  class ConsumeOp : public Op
  {
  private:
    
    i8   m_iOperandsExpected;  // number of operands expected
    i8   m_iOperandsReceived;  // number of operands received
    bool m_bAllEndMarkers;     // received all end markers
    
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
