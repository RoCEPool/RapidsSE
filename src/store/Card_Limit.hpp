/*============================================================================*/
/* Card_Limit.hpp -- Limit                                                    */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_Limit : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Limit(Stz* szRows);
   ~Card_Limit(void) noexcept(false);

    void vComputeProjection(CardIter* pCI); // prototype

  };

  Card_Limit* _pCard_Limit(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
