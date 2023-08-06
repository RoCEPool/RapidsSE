/*============================================================================*/
/* Card_Deck.hpp -- Deck of Cards                                             */
/*============================================================================*/
#pragma once

#include "Card_t.hpp"

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

    eXK eAddStageCast_toPrev_cast(CardIter*        pCI,
                                  Card_StreamPtn*  p_streamPtn,
                                  Card_Project*    p_project_prev,
                                  Card_Combined*   p_combined_prev,
                                  Card_Cast*       p_cast_prev,
                                  Card_Projection* p_projection_prev,
                                  Card_OP*         p_OP_curr);

    void vProjectionUpdate(CardIter* pCI);

    Card_Projection* p_projection_fold(void);

    void vProjectionAnalysis(CardIter* pCI);
    void vProjectionAnalysis_passThru(CardIter* pCI);

  };

  Card_Deck* _pCard_Deck(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
