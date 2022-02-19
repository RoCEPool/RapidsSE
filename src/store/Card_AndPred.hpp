/*============================================================================*/
/* Card_AndPred.hpp -- And Predicate                                          */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card_PRED.hpp"

namespace STORE
{
  class Card_AndPred : public Card_PRED
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_AndPred(void);
   ~Card_AndPred(void) noexcept(false);
    
    Card_ComparePred* pFindBound_and(Index* pIndex, eBK eBound);
    bool bHasBounds_and(Index* pIndex);

  };

  Card_AndPred* _pCard_AndPred(void* aAddr);
  bool _bIsCard_AndPred(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
