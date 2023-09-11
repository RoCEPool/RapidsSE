/*============================================================================*/
/* Card_TERMLIST.hpp -- Abstract Term List                                    */
/*============================================================================*/
#pragma once

#include "Card_LIST.hpp"

namespace STORE
{
  class Card_TERMLIST : public Card_LIST
  {  
  public:
    Card_TERMLIST(eCK eCard, i8 iFixedDepth);
   ~Card_TERMLIST(void) noexcept(false);

  };

  Card_TERMLIST* _pCard_TERMLIST(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
