/*============================================================================*/
/* Expr.hpp -- Expression Evaluator                                           */
/*============================================================================*/
#pragma once

namespace Expressions
{
  class Expr : public Object
  {
  private:
    eUK     m_eKind;
    DynFun* m_pDynFun;
  
    Calculator* m_pCalculator;
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Expr(void);
   ~Expr(void) noexcept(false);
   
    //! TEMP: TO GO
    void vGenerateAggGby(void);
      

    eUK  eKind(void);
    void vKind(eUK eKind);
  
    Stz* szSource(void);
    void vSource(Stz* szSource);

  private:
    Footer* aFooter(void);
  };

  Expr* _pExpr(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
