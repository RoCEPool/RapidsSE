/*============================================================================*/
/* Card_ByPred.hpp -- By Predicate                                            */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card_PRED.hpp"

namespace STORE
{
  class Card_ByPred : public Card_PRED
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_ByPred(void);
   ~Card_ByPred(void) noexcept(false);
  };

  Card_ByPred* _pCard_ByPred(void* aAddr);
  bool _bIsCard_ByPred(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
