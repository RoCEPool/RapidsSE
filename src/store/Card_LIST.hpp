/*============================================================================*/
/* Card_LIST.hpp -- Abstract List                                             */
/*============================================================================*/
#pragma once

#include "Card_symbol.hpp"

namespace STORE
{
  class Card_LIST : public Card_symbol<Card_LIST>
  {  
  public:
    Card_LIST(eCK eCard, i8 iFixedDepth=C_UNKNOWN);
   ~Card_LIST(void) noexcept(false);

  };

  Card_LIST* _pCard_LIST(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
