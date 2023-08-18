/*============================================================================*/
/* Card_COL.hpp -- Abstract Column                                            */
/*============================================================================*/
#pragma once

#include "Card_symbol.hpp"

namespace STORE
{
  class Card_COL : public Card_symbol<Card_COL>
  {  
  public:
    Card_COL(eCK eCard, i8 iFixedDepth);
   ~Card_COL(void) noexcept(false);

    static bool bIsReplicated (Symbol* aCol);
    static bool bMatchingTblSyn(Symbol* pArg_a, Symbol* pArg_b);

  };

  Card_COL* _pCard_COL(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
