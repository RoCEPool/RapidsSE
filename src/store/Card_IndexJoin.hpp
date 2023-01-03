/*============================================================================*/
/* Card_IndexJoin.hpp -- Index Join                                           */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_IndexJoin : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_IndexJoin(void);
   ~Card_IndexJoin(void) noexcept(false);

    void vApplyStream(Card_StreamModel* pStreamModel); // prototype
  };

  Card_IndexJoin* _pCard_IndexJoin(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
