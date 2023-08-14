/*============================================================================*/
/* Card_JoinCast.hpp -- How buffers were cast to a join locus                */
/*============================================================================*/
#pragma once

#include "Card_symbol.hpp"

namespace STORE
{
  class Card_JoinCast : public Card_symbol<Card_JoinCast>
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_JoinCast(void);
   ~Card_JoinCast(void) noexcept(false);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);
  };

  Card_JoinCast* _pCard_JoinCast(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
