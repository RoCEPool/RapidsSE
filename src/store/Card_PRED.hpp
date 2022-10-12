/*============================================================================*/
/* Card_PRED.hpp -- Abstract Predicate                                        */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_CmpPred;

  class Card_PRED : public Card
  {  
  public:
    Card_PRED(eCK eCard);
    Card_PRED(eCK eCard, Stz* szValue);
   ~Card_PRED(void) noexcept(false);

    void vConcatTerms(Stz* szTerms);
    void vConcatTerms(Stz* szTerms, i8 i_From, i8 i_Till=C_UNKNOWN);
    void vCullByKids(void);
    Card_CmpPred* pFindBound_pred(Index* pIndex, ePB eBound);
    bool bHasBounds_pred (Index* pIndex);
    bool bIsPred(Card* pArgCard);

    bool bReduciblePred(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
