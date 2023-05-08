/*============================================================================*/
/* Card_Bounds.hpp -- Join Predicate                                        */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_Bounds : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Bounds(void);
   ~Card_Bounds(void) noexcept(false);

    void vCullBound(char cWithMark);

    Card_Bound* pFindChosenBound(void);

    bool bHasBoundOf(Card_CmpPred* p_cmpPred);
  };

  Card_Bounds* _pCard_Bounds(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
