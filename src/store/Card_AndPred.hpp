/*============================================================================*/
/* Card_AndPred.hpp -- And Predicate                                     */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card_Pred.hpp"

namespace STORE
{
  class Card_AndPred : public Card_Pred
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_AndPred(eCK eKind);
   ~Card_AndPred(void) noexcept(false);
  };

  Card_AndPred* _pCard_AndPred(void* aAddr);
  bool _bIsCard_AndPred(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
