/*============================================================================*/
/* Card_TLIST.hpp -- Abstract Term List                                       */
/*============================================================================*/
#pragma once

#include "Card_LIST.hpp"

namespace STORE
{
  class Card_TLIST : public Card_LIST
  {  
  public:
    Card_TLIST(eCK eCard, i8 iFixedDepth);
   ~Card_TLIST(void) noexcept(false);

  };

  Card_TLIST* _pCard_TLIST(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
