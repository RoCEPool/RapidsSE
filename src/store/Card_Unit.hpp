/*============================================================================*/
/* Card_Unit.hpp -- Abstract Unit                                             */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card.hpp"

namespace STORE
{
  class Card_Unit : public Card
  {  
  protected:
    
  private:
    
  public:
  
    Card_Unit(eCK eKind, eCN eClass);
   ~Card_Unit(void) noexcept(false);
    
  private:
    static void vDeleteThis_(Card* pCard);
  
    Footer* aFooter(void);
  };

  Card_Unit* _pCard_Unit(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
