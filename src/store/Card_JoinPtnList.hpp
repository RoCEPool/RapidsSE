/*============================================================================*/
/* Card_JoinPtnList.hpp -- Join Partition List                                */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_JoinPtnList : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_JoinPtnList(void);
   ~Card_JoinPtnList(void) noexcept(false);
  };

  Card_JoinPtnList* _pCard_JoinPtnList(void* aAddr);
  bool _bIsCard_JoinPtnList(void* aAddr);
  Card_JoinPtnList* _pGetSub_JoinPtnList(Card* pParent);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
