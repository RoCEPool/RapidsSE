/*============================================================================*/
/* Card_OrPred.hpp -- Or Predicate                                            */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card_PRED.hpp"

namespace STORE
{
  class Card_OrPred : public Card_PRED
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_OrPred(void);
   ~Card_OrPred(void) noexcept(false);
    
    Card_ComparePred* pFindBound_or(Index* pIndex, eBK eBound);
    bool bHasBounds_or(Index* pIndex);

  };

  Card_OrPred* _pCard_OrPred(void* aAddr);
  bool _bIsCard_OrPred(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
