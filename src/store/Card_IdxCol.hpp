/*============================================================================*/
/* Card_IdxCol.hpp -- Index Column                                            */
/*============================================================================*/
#pragma once

#include "Card_COLUMN.hpp"

namespace STORE
{
  class Card_IdxCol : public Card_COLUMN
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Card_IdxCol(void);
   ~Card_IdxCol(void) noexcept(false);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);
  };

  Card_IdxCol* _pCard_IdxCol(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
