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

    void vFlipTerms(Card_Projection* p_projection_reduced);
  };

  Card_Bounds* _pCard_Bounds(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
