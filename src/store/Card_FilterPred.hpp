/*============================================================================*/
/* Card_FilterPred.hpp -- Filter Predicate                                    */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card_PRED.hpp"

namespace STORE
{
  class Card_FilterPred : public Card_PRED
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_FilterPred(void);
   ~Card_FilterPred(void) noexcept(false);
  };

  Card_FilterPred* _pCard_FilterPred(void* aAddr);
  bool _bIsCard_FilterPred(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
