/*============================================================================*/
/* Card_BolPred.hpp -- Boolean Predicate                                      */
/*============================================================================*/
#pragma once

#include "Card_PRED.hpp"

namespace STORE
{
  class Card_BolPred : public Card_PRED
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_BolPred(void);
   ~Card_BolPred(void) noexcept(false);
  };

  Card_BolPred* _pCard_BolPred(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
