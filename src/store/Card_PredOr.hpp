/*============================================================================*/
/* Card_PredOr.hpp -- Or Predicate                                            */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card_PRED.hpp"

namespace STORE
{
  class Card_PredOr : public Card_PRED
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_PredOr(void);
   ~Card_PredOr(void) noexcept(false);
    
    Card_PredCmp* pFindBound_or(Index* pIndex, eBK eBound);
    bool bHasBounds_or(Index* pIndex);
  };

  Card_PredOr* _pCard_PredOr(void* aAddr);
  bool _bIsCard_PredOr(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
