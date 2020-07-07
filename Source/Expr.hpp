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
    s2K     m_sSource;
    DynFun* m_pDynFun;
  
    Calculator* m_pCalculator;
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Expr(void);
   ~Expr(void) noexcept(false);
   
    i8   iAggregate(byte* ayPayLoad, Rsb* aSrcRsb, RowSlab* pSrcRowSlab);
    bool bCompile(void);
  
    i8   iFilter(Rsb* aRsb, RowSlab* pRowSlab);

    i8   iProject(RowSlab* pDstRowSlab, Rsb* aSrcRsb, RowSlab* pSrcRowSlab);

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
