/*============================================================================*/
/* Card_Fold.hpp -- XF_Fold (delineates the two halves during flattening)        */
/*============================================================================*/
#pragma once

#include "Card_CONSTRUCTION.hpp"

namespace STORE
{
  class Card_Fold : public Card_CONSTRUCTION
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Fold(void);
   ~Card_Fold(void) noexcept(false);

    static void vMorph_to_flat(CardIter* pCI);

    static void vPush_foldExpr_Into_gbyAgg_key(CardIter* pCI, Card_Expr* p_expr_fold);
    static void vPush_foldExpr_Into_gbyAgg_projection(CardIter* pCI, Card_Expr* p_expr_fold);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);

  };

  Card_Fold* _pCard_Fold(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
