/*============================================================================*/
/* Card_PRED.hpp -- Abstract Predicate                                        */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card.hpp"

namespace STORE
{
  class Card_ComparePred;

  class Card_PRED : public Card
  {  
  public:
    Card_PRED(eCK eCard);
   ~Card_PRED(void) noexcept(false);
    
     void vReplaceOrdinals(StreamDef* pStreamDef,
                           Index*     pIndex=nullptr);
  
    void vCullByZeroKids(void);
    Card_ComparePred* pFindBound_pred(Index* pIndex, eBK eBound);
    bool bHasBounds_pred (Index* pIndex);
    
    bool bReduciblePred(void);
    void vReducePred(Card_PRED* pCard_PRED_child);

    bool bRemoveBound_pred(Card_ComparePred* pCard_ComparePred);
  };

  //Card_PRED* _pCard_PRED(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
