/*============================================================================*/
/* Card_CELLS.hpp -- Abstract List                                            */
/*============================================================================*/
#pragma once

#include "Card_LIST.hpp"

namespace STORE
{
  class Card_CELLS : public Card_LIST
  {  
  public:
    Card_CELLS(eCK eCard);
   ~Card_CELLS(void) noexcept(false);

  };

  Card_CELLS* _pCard_CELLS(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
