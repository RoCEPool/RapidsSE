/*============================================================================*/
/* Card_FiltPredList.hpp -- Filter Predicate List                             */
/*============================================================================*/
#pragma once

#include "Card_symbol.hpp"

namespace STORE
{
  class Card_FiltPred;

  class Card_FiltPredList : public Card_symbol<Card_FiltPredList>
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_FiltPredList(void);
   ~Card_FiltPredList(void) noexcept(false);

    static Card_FiltPredList* pExtractList(Card_Deck* p_deck);

    i8 nFiltersFor(char cTblSyn);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);
  };

  Card_FiltPredList* _pCard_FiltPredList(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
