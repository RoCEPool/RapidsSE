/*============================================================================*/
/* Card_COLUMN.hpp -- Abstract Column                                         */
/*============================================================================*/
#pragma once

#include "Card_symbol.hpp"

namespace STORE
{
  class Card_COLUMN : public Card_symbol<Card_COLUMN>
  {  
  public:
    Card_COLUMN(eCK eCard, i8 iFixedDepth);
   ~Card_COLUMN(void) noexcept(false);

    static bool bIsReplicated (Symbol* aCol);
    static bool bMatchingTblSyn(Symbol* pArg_a, Symbol* pArg_b);

  };

  Card_COLUMN* _pCard_COLUMN(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
