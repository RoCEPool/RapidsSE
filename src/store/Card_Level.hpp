/*============================================================================*/
/* Card_Level.hpp -- Aggregate Level                                       */
/*============================================================================*/
#pragma once

#include "Card_TIER2.hpp"

namespace STORE
{
  class Card_Level : public Card_TIER2
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Level(eAL eLevel);
   ~Card_Level(void) noexcept(false);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);
  };

  Card_Level* _pCard_Level(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
