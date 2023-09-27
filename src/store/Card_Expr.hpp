/*============================================================================*/
/* Card_Expr.hpp -- Typed Expression                                          */
/*============================================================================*/
#pragma once

#include "Card_symbol.hpp"

namespace STORE
{
  class Card_Expr : public Card_symbol<Card_Expr>
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Expr(void);
    Card_Expr(Symbol* pSymbol);
    Card_Expr(Card_As*   p_as_arg,
              Card_Term* p_term_0_arg);
   ~Card_Expr(void) noexcept(false);

    //-----------------------------------------------------------
    static Card_Expr* pGby_expr_From_expr_corr(Card_Expr* pExpr_corr);
    static Card_Expr* pProj_expr_From_Gby_expr(Card_Expr* p_expr_gby, Stz* szAs=nullptr);
    static Card_Expr* pProj_Simple_expr_Of(Card_Expr* p_expr, i8 i_Col);
    static Card_Expr* pProj_Simple_expr_Of_key(Card_Expr* p_expr_key);

    void vBump_stmOdn(i8 nBumpBy, bool bSkipFoldExpr=false);
    eCT  eColumnType(void);
    Card_Expr* pClone(void);
    void vConvert_colOdn_To_stmOdn(CardIter* pCI);
    void vConvert_stmOdn_To_colOdn(CardIter* pCI);
    Card_Expr* pCraft_expr_early(CardIter* pCI, i8 i_Expr=0);
    void vDeconstruct(Card_As*&   rp_as,
                      Card_Term*& rp_term_0);
    void vEnsureTypes_For_stmOdn(CardIter* pCI);
    void vFactorAll_valOdn_IntoDistinct_expr(CardIter* pCI);
    Symbol* pFirstKeyCol(void);
    Symbol* pFirstStmCol(void);
    Symbol* pFirstTblCol(void);
    Symbol* pGetSymbol(i8 i_Term);
    Card_Term* pGetTerm(i8 i_Term);
    bool bIsAggCalc(void);
    bool bIsDescending(void);
    bool bExpressionOfSymbol(Symbol* pSymbol);
    bool bIsCompound(void);
    bool bIsScalarCol(void);
    bool bIsScalarKey(void);
    bool bIsExpressionOfKey(void);
    bool bHasTerm(Symbol* pSymbol);
    Card_Expr* pPropagate(void);
    void vReconstruct(Card_As*    p_as,
                      Card_Term*  p_term_0);
    Card_Expr *pReducedCopy(i8 i_Expr);

    void vRenumberForm_keyOdn(i8 i_Expr);
    void vRenumberForm_stmOdn(i8 i_Expr);

    bool bSameAs(Card_Expr* pExpr_arg);


    Stz* szP_Type(void);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);
  };

  Card_Expr* _pCard_Expr(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
