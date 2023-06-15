/*============================================================================*/
/* Card_Projection.hpp -- Stream Model                                       */
/*============================================================================*/
#pragma once

#include "Card_ELIST.hpp"
#include "ColsReq.hpp"

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

    Card_Projection* pClone(void);
    Card_Needed* pCraftRoot_needed(CardIter* pCI);
    void vDeletePrjKey(CardIter* pCI);
    void vDerivePrjKey(CardIter* pCI);
    void vEnsureTypes_For_stmColOdn(CardIter* pCI);
    void vReviewAs(CardIter* pCI);

    StreamDef* pToStreamDef(ColsReq* pColMap=nullptr, Card_Key* p_key=nullptr);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);

  };

  Card_Projection* _pCard_Projection(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
