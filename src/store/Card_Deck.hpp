/*============================================================================*/
/* Card_Deck.hpp -- Deck of Cards                                             */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_Deck : public Card
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Deck(void);
   ~Card_Deck(void) noexcept(false);

    Card_Expr* p_expr_fold(void);
    i8 i_Fold(void);
    Card_Fold* p_fold(void);

    void vAddStageCasts(CardIter* pCI);

    eXK eAddStageCast(CardIter*      pCI,
                      Card_OP*       p_main_prior,
                      Card_Combined* p_combined_prior,
                      Card_Next*     p_next_prior,
                      i8&            rnSkip);

    void vProjectionUpdate(CardIter* pCI);

    Card_Projection* p_projection_fold(void);

    void vProjectionAnalysis(CardIter* pCI);

  };

  Card_Deck* _pCard_Deck(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
