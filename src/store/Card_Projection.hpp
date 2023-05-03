/*============================================================================*/
/* Card_Projection.hpp -- Stream Model                                       */
/*============================================================================*/
#pragma once

#include "Card_ELIST.hpp"

namespace STORE
{
  class Card_Projection : public Card_ELIST
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Projection(void);
    Card_Projection(eCK eCard);
   ~Card_Projection(void) noexcept(false);


    static Card_Projection* pResolvePriorProjection(CardIter* pCI);

    void vAs_Review(CardIter* pCI);
    Card_Projection* pClone(void);
    Card_Needed* pCraftRoot_needed(CardIter* pCI);
    void vDelete_key(CardIter* pCI);
    void vKeyProjection(CardIter* pCI);
    void vTypes_For_stmColOdn(CardIter* pCI);

  };

  Card_Projection* _pCard_Projection(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
