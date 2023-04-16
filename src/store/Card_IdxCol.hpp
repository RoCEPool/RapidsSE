/*============================================================================*/
/* Card_IdxCol.hpp -- Index Column                                            */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_IdxCol : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Card_IdxCol(void);
   ~Card_IdxCol(void) noexcept(false);

    static bool bMatchingTblSyn(Symbol* pArg_a, Symbol* pArg_b);
  };

  Card_IdxCol* _pCard_IdxCol(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
