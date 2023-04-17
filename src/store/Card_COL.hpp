/*============================================================================*/
/* Card_COL.hpp -- Abstract Column                                            */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_COL : public Card
  {  
  public:
    Card_COL(eCK eCard);
   ~Card_COL(void) noexcept(false);

    static bool bIsReplicated (Symbol* aCol);
    static bool bMatchingTblSyn(Symbol* pArg_a, Symbol* pArg_b);

  };

  Card_COL* _pCard_COL(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
