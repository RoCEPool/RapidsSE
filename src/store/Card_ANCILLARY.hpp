/*============================================================================*/
/* Card_ANCILLARY.hpp -- Ancilliary                                          */
/*============================================================================*/
#pragma once

#include "Card_MAIN.hpp"

namespace STORE
{
  class Card_ANCILLARY : public Card_MAIN
  {  
  public:
    Card_ANCILLARY(eCK eCard, i8 iFixedDepth);
   ~Card_ANCILLARY(void) noexcept(false);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);
  };

  Card_ANCILLARY* _pCard_ANCILLARY(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
