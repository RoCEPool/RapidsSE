/*============================================================================*/
/* Card_CoPtn.hpp -- Co-Partition                                             */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_CoPtn : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_CoPtn(void);
   ~Card_CoPtn(void) noexcept(false);
  };

  Card_CoPtn* _pCard_CoPtn(void* aAddr);
  bool _bIsCard_CoPtn(void* aAddr);
  Card_CoPtn* _pGetSub_Card_CoPtn(Card* pParent);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
