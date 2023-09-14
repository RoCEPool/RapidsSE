/*============================================================================*/
/* Card_LoadJoined.hpp -- Load of the actual Join                             */
/*============================================================================*/
#pragma once

#include "Card_symbol.hpp"

namespace STORE
{
  class Card_CostJoining : public Card_symbol<Card_CostJoining>
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_CostJoining(void);
   ~Card_CostJoining(void) noexcept(false);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);
  };

  Card_CostJoining* _pCard_CostJoining(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
