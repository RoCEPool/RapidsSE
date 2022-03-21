/*============================================================================*/
/* Card_OBJECT.hpp -- Abstract Object Card                                    */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card_SUB.hpp"

namespace STORE
{
  class Card_OBJECT : public Card_SUB
  {  
  public:
    Card_OBJECT(eCK eCard);
   ~Card_OBJECT(void) noexcept(false);
  };

  Card_OBJECT* _pCard_OBJECT(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
