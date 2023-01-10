/*============================================================================*/
/* Card_Fold.hpp -- Fold (delineates the two halves of the flattening)        */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_Fold : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Fold(void);
   ~Card_Fold(void) noexcept(false);

    static void vBump_where_BaseOn_foldExpr(CardIter* pCI);
  };

  Card_Fold* _pCard_Fold(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
