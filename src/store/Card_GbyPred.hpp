/*============================================================================*/
/* Card_GbyPred.hpp -- GroupBy Predicate                                      */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card_PRED.hpp"

namespace STORE
{
  class Card_GbyPred : public Card_PRED
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_GbyPred(void);
   ~Card_GbyPred(void) noexcept(false);
  };

  Card_GbyPred* _pCard_GbyPred(void* aAddr);
  bool _bIsCard_GbyPred(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
