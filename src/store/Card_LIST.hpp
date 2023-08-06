/*============================================================================*/
/* Card_LIST.hpp -- Abstract List                                             */
/*============================================================================*/
#pragma once

#include "Card_t.hpp"

namespace STORE
{
  class Card_LIST : public Card
  {  
  public:
    Card_LIST(eCK eCard);
   ~Card_LIST(void) noexcept(false);

  };

  Card_LIST* _pCard_LIST(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
