/*============================================================================*/
/* DynFun.hpp -- Dynamic Function                                             */
/*============================================================================*/
#pragma once

namespace Expressions
{
  class DynFun : public Object
  {
  private:
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    DynFun(void);
   ~DynFun(void) noexcept(false);
  
  private:
    Footer* aFooter(void);
  };

  DynFun* _pDynFun(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
