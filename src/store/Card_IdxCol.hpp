/*============================================================================*/
/* Card_IdxCol.hpp -- Index Column                                            */
/*============================================================================*/
#pragma once

#include "Card_COL.hpp"

namespace STORE
{
  class Card_IdxCol : public Card_COL
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Card_IdxCol(void);
   ~Card_IdxCol(void) noexcept(false);

  };

  Card_IdxCol* _pCard_IdxCol(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
