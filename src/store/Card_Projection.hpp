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

    static void vProjection_Analysis_v2(CardIter* pCI);
    static Card_Projection* pResolvePriorProjection(CardIter* pCI);

    void vAs_Review(CardIter* pCI);

    void vDelete_key(CardIter* pCI);

    Card_Expr* pFind_expr_By_col(CardIter* pCI, Stz* szCol);
    Card_Expr* pFind_expr_By_stmColOdn(CardIter* pCI, Stz* szTblColOdn);
    Card_Expr* pFind_expr_By_tblColOdn(CardIter* pCI, Stz* szTblColOdn);

    void vKeyProjection(CardIter* pCI);

    Card_Projection* pReProjection(void);

    Card_Projection* pToCarry(Card_Key* p_key);
    Card_Projection* pToCarry_v2(CardIter* pCI, Card_Key* p_key);
  };

  Card_Projection* _pCard_Projection(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
