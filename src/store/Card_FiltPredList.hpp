/*============================================================================*/
/* Card_FiltPredList.hpp -- Filter Predicate List                             */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_FiltPred;

  class Card_FiltPredList : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_FiltPredList(void);
   ~Card_FiltPredList(void) noexcept(false);

    static Card_FiltPredList* pExtract(Card_Deck* p_deck);

    Card_FiltPred* pFindFiltPred(char cTblSyn);

    i8 nFiltersFor(char cTblSyn);

  };

  Card_FiltPredList* _pCard_FiltPredList(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
