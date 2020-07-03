/*============================================================================*/
/* ExprVec.hpp -- Expression Evaluation Vector                                */
/*============================================================================*/
#pragma once

namespace Expressions
{
  class ExprVec : public VectorNL_t<CN_EXPRVEC,Expr,C_MAX_EXPR>
  {
  private:
 
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    ExprVec(void);
   ~ExprVec(void) noexcept(false);
  
  private:
    Footer* aFooter(void);
  };

  ExprVec* _pExprVec(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
