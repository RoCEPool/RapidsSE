/*============================================================================*/
/* Card_PRED.hpp -- Abstract Predicate                                        */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card_SUB.hpp"

namespace STORE
{
  class Card_PredCmp;

  class Card_PRED : public Card_SUB
  {  
  public:
    Card_PRED(eCK eCard);
   ~Card_PRED(void) noexcept(false);
      
    void vCullByKids(void);
    Card_PredCmp* pFindBound_pred(Index* pIndex, eBK eBound);
    bool bHasBounds_pred (Index* pIndex);
    
    bool bReduciblePred(void);

  };

  //Card_PRED* _pCard_PRED(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
