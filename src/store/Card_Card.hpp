/*============================================================================*/
/* Card_Card.hpp -- Card                                                     */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_Card : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Card(Card* pCard);
   ~Card_Card(void) noexcept(false);
  };

  Card_Card* _pCard_Card(void* aAddr);
  bool _bIsCard_Card(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
