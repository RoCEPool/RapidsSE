/*============================================================================*/
/* Card_JoinOrderSeq.hpp -- Join Order Sequence                               */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_Stream;

  class Card_JoinOrderSeq : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_JoinOrderSeq(void);
   ~Card_JoinOrderSeq(void) noexcept(false);
  };

  Card_JoinOrderSeq* _pCard_JoinOrderSeq(void* aAddr);
  bool _bIsCard_JoinOrderSeq(void* aAddr);
  Card_JoinOrderSeq* _pGetSub_JoinOrderSeq(Card* pParent);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
