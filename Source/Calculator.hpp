/*============================================================================*/
/* Calculator.hpp -- Calculator                                               */
/*============================================================================*/
#pragma once

namespace Operators
{
  class Calculator : public Object
  {
  private:
  
  private:
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Calculator(Job* pJob);
   ~Calculator(void) noexcept(false);
  
    Footer* aFooter(void);
  };
  
  Calculator* _pCalculator(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
