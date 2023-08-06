/*============================================================================*/
/* Card_Send.hpp -- Send                                                      */
/*============================================================================*/
#pragma once

#include "Card_t.hpp"

namespace STORE
{
  class Card_Send : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    static void vDeleteAllSend(CardIter* pCI);

    Card_Send(void);
   ~Card_Send(void) noexcept(false);
  };

  Card_Send* _pCard_Send(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
