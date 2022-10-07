/*============================================================================*/
/* Card_PRED.hpp -- Abstract Predicate                                        */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_PredCmp;

  class Card_PRED : public Card
  {  
  public:
    Card_PRED(eCK eCard);
    Card_PRED(eCK eCard, Stz* szValue);
   ~Card_PRED(void) noexcept(false);

    void vConcatTerms(Stz* szTerms);
    void vConcatTerms(Stz* szTerms, i8 i_From, i8 i_Till=C_UNKNOWN);
    void vCullByKids(void);
    Card_PredCmp* pFindBound_pred(Index* pIndex, ePB eBound);
    bool bHasBounds_pred (Index* pIndex);
    
    bool bReduciblePred(void);

  };

  //Card_PRED* _pCard_PRED(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
