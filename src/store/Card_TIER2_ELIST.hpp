/*============================================================================*/
/* Card_TIER2_ELIST.hpp -- Tier 2 (second tier)                                     */
/*============================================================================*/
#pragma once

#include "Card_ELIST.hpp"

namespace STORE
{
  class Card_TIER2_ELIST : public Card_ELIST
  {  
  public:
    Card_TIER2_ELIST(eCK eCard);
   ~Card_TIER2_ELIST(void) noexcept(false);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);
  };

  Card_TIER2_ELIST* _pCard_TIER2_ELIST(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
