/*============================================================================*/
/* Card_Expr.hpp -- Typed Expression                                          */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_Expr : public Card
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Expr(void);
   ~Card_Expr(void) noexcept(false);

    //-----------------------------------------------------------
    static Card_Expr* pGby_expr_From_expr_corr(Card_Expr* pExpr_corr);
    static Card_Expr* pProj_expr_From_Gby_expr(Card_Expr* p_expr_gby, Stz* szAs=nullptr);
    static Card_Expr* pProj_Simple_expr_Of(Card_Expr* p_expr, i8 i_Col);

    void vBump_Rightwards(i8 nBumpBy, bool bSkipFoldExpr=false);
    void vConvert_stmColOdn_terms_To_tblColOdn(CardIter* pCI);
    Stz* szDollarOdn(void);
    bool bIsCalcSequence(void);
    bool bIsColOrdinal(void);
    bool bIsProjectedCol(Stz* szProjectedCol);
    bool bIsSame(Card_Expr* pExpr_arg);
    Stz* szProjectedCol(void);
    Card_Term* pProjectedTerm(void);
    void vRenumber(i8 i_Expr, bool bIsKeyExpr=false);
    Card_Expr* pProjectRight(void);

    i8 iRpnCalc_WhichGbyKey(void);
    Card_Expr* pToCarry_key(void);
  };

  Card_Expr* _pCard_Expr(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
