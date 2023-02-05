/*============================================================================*/
/* Card_Projection.hpp -- Stream Model                                       */
/*============================================================================*/
#pragma once

#include "Card_ELIST.hpp"

namespace STORE
{
  class Card_Projection : public Card_ELIST
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Projection(void);
   ~Card_Projection(void) noexcept(false);

    static void vProjection_Analysis(CardIter* pCI);
    static void vProjection_Analysis_postFold(CardIter* pCI);
    static void vRelabelExpressions(CardIter* pCI);
    static Card_Projection* pResolvePriorProjection(CardIter* pCI);

    void vBump_Rightwards(i8 nBumpBy);
    void vConvert_stmColOdn_terms_To_tblColOdn(CardIter* pCI);
    i8 nExpr(void);
    Card_Expr* pGetExpr(i8 i_Expr);
    Card_Expr* pGetExpr(Card_Term* p_term);

    void vNumberExpr(void);

    Card_Projection* pReProjection(void);
  };

  Card_Projection* _pCard_Projection(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
