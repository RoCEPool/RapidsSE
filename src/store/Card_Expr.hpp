/*============================================================================*/
/* Card_Expr.hpp -- Typed Expression                                     */
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
    static Card_Expr* pGby_expr_From_stmCol(Card_StmCol* p_stmCol);
    static Card_Expr* pProj_expr_From_Gby_expr(Card_Expr* p_expr_gby, Stz* szAs=nullptr);
    static Card_Expr* pProj_expr_From_stmCol(Card_StmCol* p_stmCol);
    static Card_Expr* pProj_Simple_expr_Of(Card_Expr* p_expr, i8 i_Col);

    i8 iRpnCalc_WhichGbyKey(void);

    void vNumberValue(i8 i_Expr);
  };

  Card_Expr* _pCard_Expr(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
