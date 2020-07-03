/*============================================================================*/
/* ExprCompiler.hpp -- Expression Compiler                                    */
/*============================================================================*/
#pragma once

namespace Expressions
{
  class ExprCompiler : public Object
  {
  private:
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    ExprCompiler(void);
   ~ExprCompiler(void) noexcept(false);
  
  private:
    Footer* aFooter(void);
  };

  ExprCompiler* _pExprCompiler(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
