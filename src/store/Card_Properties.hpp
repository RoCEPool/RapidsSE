/*============================================================================*/
/* Card_Properties.hpp -- Set of Proprties                                    */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card_Property.hpp"

namespace STORE { class Card_Atom; }

namespace STORE
{
  class Card_Properties : public Card
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Properties(eCK eKind);
   ~Card_Properties(void) noexcept(false);
  };

  Card_Properties* _pCard_Properties(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
