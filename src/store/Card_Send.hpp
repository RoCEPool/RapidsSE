/*============================================================================*/
/* Card_Send.hpp -- Send                                                      */
/*============================================================================*/
#pragma once

#include "Card_CONSTRUCTION.hpp"

namespace STORE
{
  class Card_Send : public Card_CONSTRUCTION
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Card_Send(void);
   ~Card_Send(void) noexcept(false);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);
  };

  Card_Send* _pCard_Send(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
