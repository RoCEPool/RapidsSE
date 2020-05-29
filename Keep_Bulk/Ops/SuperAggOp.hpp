/*============================================================================*/
/* SuperAggOp.hpp -- Super Count                                            */
/*============================================================================*/
#pragma once

namespace DataOperators
{
  
  class SuperAggOp : public Op
  {
  private:
  
    i8  m_nSum;
    
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    SuperAggOp(OpSpec* pOpSpec, Pipeline* pPipeline);
   ~SuperAggOp(void) noexcept(false);
  
    static Op* pNew(OpSpec* pOpSpec, Pipeline* pPipeline);
    
    static eOR eCall(void* aThis);
    
  private:
    eOR        eSuperAggLoop(void);
  
    Footer* aFooter(void);
  };

  SuperAggOp* _pSuperAggOp(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
