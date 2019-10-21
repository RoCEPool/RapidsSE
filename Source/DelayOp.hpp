/*============================================================================*/
/* DelayOp.hpp -- Fetch                                                       */
/*============================================================================*/
#pragma once

namespace DataOperators
{
  class DelayOp : public Op
  {
  private:
  
    i8 m_nDelay;   // delay in seconds
    
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    DelayOp(OpSpec* pOpSpec, Pipeline* pPipeline);
   ~DelayOp(void) noexcept(false);

    static Op* pNew(OpSpec* pOpSpec, Pipeline* pPipeline);
    static eOR eCall(void* aThis);
  
    eOR eOpLoop(void);
  
    void vPreliminary(void);
  
  private:
    Footer* aFooter(void);
  };

  DelayOp* _pDelayOp(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
