/*============================================================================*/
/* Expr.hpp -- Expression Evaluator                                           */
/*============================================================================*/
#pragma once

namespace Expressions
{
  class Expr : public Object
  {
  private:
 
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Expr(void);
   ~Expr(void) noexcept(false);
  
  private:
    Footer* aFooter(void);
  };

  Expr* _pExpr(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
