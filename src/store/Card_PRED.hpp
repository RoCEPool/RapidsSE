/*============================================================================*/
/* Card_PRED.hpp -- Abstract Predicate                                        */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card_SUB.hpp"

namespace STORE
{
  class Card_CmpPred;

  class Card_PRED : public Card_SUB
  {  
  public:
    Card_PRED(eCK eCard);
   ~Card_PRED(void) noexcept(false);
    
     void vReplaceOrdinals(StreamDef* pStreamDef,
                           Index*     pIndex=nullptr);
  
    void vCullByKids(void);
    Card_CmpPred* pFindBound_pred(Index* pIndex, eBK eBound);
    bool bHasBounds_pred (Index* pIndex);
    
    bool bReduciblePred(void);
    void vReducePred(Card_PRED* pCard_PRED_child);

  };

  //Card_PRED* _pCard_PRED(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
