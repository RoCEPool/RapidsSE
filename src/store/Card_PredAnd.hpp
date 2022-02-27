/*============================================================================*/
/* Card_PredAnd.hpp -- And Predicate                                          */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card_PRED.hpp"

namespace STORE
{
  class Card_PredAnd : public Card_PRED
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_PredAnd(void);
   ~Card_PredAnd(void) noexcept(false);
    
    Card_PredCmp* pFindBound_and(Index* pIndex, eBK eBound);
    bool bHasBounds_and(Index* pIndex);
  };

  Card_PredAnd* _pCard_PredAnd(void* aAddr);
  bool _bIsCard_PredAnd(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
