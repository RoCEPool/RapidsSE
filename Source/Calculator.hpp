/*============================================================================*/
/* Calculator.hpp -- Calculator                                               */
/*============================================================================*/
#pragma once

namespace Operators
{
  static const i8 C_CALC_PROGRAM = C_4KB;
  static const i8 C_CALC_STACK   = C_4KB;

  class Calculator : public Object
  {
  private:
    byte m_baExpr[C_CALC_PROGRAM];
    i8   m_iaStack[C_CALC_STACK];

  private:
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Calculator(Job* pJob);
   ~Calculator(void) noexcept(false);
  
    bool bCompile(TokenVec* pTknVec);

    i8   iCompute(Row* pRow);

    Footer* aFooter(void);
  };
  
  Calculator* _pCalculator(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
