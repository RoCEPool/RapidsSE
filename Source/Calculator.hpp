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
    eEO  m_eaCode[C_CALC_PROGRAM];
    i8   m_iaData[C_CALC_STACK];

  private:
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Calculator(void);
   ~Calculator(void) noexcept(false);
  
    bool bCompile(TokenVec* pTknVec);
    bool bCompileTest(TokenVec* pTknVec);

    i8   iEvaluate(i8* paColValues);

    Footer* aFooter(void);
  };
  
  Calculator* _pCalculator(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
