/*============================================================================*/
/* Card_TIER2.hpp -- Tier 2 (second tier)                                     */
/*============================================================================*/
#pragma once

#include "Card_symbol.hpp"
#include "IndexDef.hpp"
#include "ColsReq.hpp"
#include "ColMap.hpp"

namespace STORE
{
  class Card_TIER2 : public Card_symbol<Card_TIER2>
  {  
  public:
    Card_TIER2(eCK eCard);
   ~Card_TIER2(void) noexcept(false);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);
  };

  Card_TIER2* _pCard_TIER2(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
