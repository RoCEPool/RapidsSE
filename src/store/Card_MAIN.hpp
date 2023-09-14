/*============================================================================*/
/* Card_MAIN.hpp -- Abstract Op                                                 */
/*============================================================================*/
#pragma once

#include "Card_symbol.hpp"

namespace STORE
{
  class Card_MAIN : public Card_symbol<Card_MAIN>
  {  
  public:
    Card_MAIN(eCK eCard, i8 iFixedDepth);
   ~Card_MAIN(void) noexcept(false);


    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);
  };

  Card_MAIN* _pCard_MAIN(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
