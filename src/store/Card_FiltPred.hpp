/*============================================================================*/
/* Card_FiltPred.hpp -- Filter Predicate                                      */
/*============================================================================*/
#pragma once

#include "Card_symbol.hpp"

namespace STORE
{
  class Card_FiltPred : public Card_symbol<Card_FiltPred>
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_FiltPred(void);
   ~Card_FiltPred(void) noexcept(false);
  };

  Card_FiltPred* _pCard_FiltPred(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
