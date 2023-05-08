/*============================================================================*/
/* Card_Fold.hpp -- Fold (delineates the two halves during flattening)        */
/*============================================================================*/
#pragma once

#include "Card_OP.hpp"

namespace STORE
{
  class Card_Fold : public Card_OP
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Fold(void);
   ~Card_Fold(void) noexcept(false);

    static void vMorph_to_flat(CardIter* pCI);

  };

  Card_Fold* _pCard_Fold(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
