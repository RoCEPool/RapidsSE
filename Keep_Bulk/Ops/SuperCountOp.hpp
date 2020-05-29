/*============================================================================*/
/* SuperCountOp.hpp -- Super Count                                            */
/*============================================================================*/
#pragma once

namespace DataOperators
{
  
  class SuperCountOp : public Op
  {
  private:
  
    i8  m_nCount;     
    
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    SuperCountOp(OpSpec* pOpSpec, Pipeline* pPipeline);
   ~SuperCountOp(void) noexcept(false);
  
    static Op* pNew(OpSpec* pOpSpec, Pipeline* pPipeline);
    
    static eOR eCall(void* aThis);
    
  private:
    eOR        eSuperCountLoop(void);
  
    Footer* aFooter(void);
  };

  SuperCountOp* _pSuperCountOp(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
