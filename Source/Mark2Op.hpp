/*============================================================================*/
/* Mark2Op.hpp -- Mark II Operator                                            */
/*============================================================================*/
#pragma once

namespace DataOperators
{
  class Mark2Op : public Op
  {
  private:
    
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Mark2Op(OpSpec* pOpSpec, Pipeline* pPipeline);
   ~Mark2Op(void) noexcept(false);
  
    static Op* pNew(OpSpec* pOpSpec, Pipeline* pPipeline);
    static eOR eCall(void* aThis);
    static eOR eDrain(void* aThis);

    eOR eOpLoop(void);
    eOR eDrainLoop(void);

  private:
    Footer* aFooter(void);
  };

  Mark2Op* _pMark2Op(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
