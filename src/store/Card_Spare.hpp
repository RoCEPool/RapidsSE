/*============================================================================*/
/* Card_Spare.hpp -- Spare                                                    */
/*============================================================================*/
#pragma once

#include "Card_symbol.hpp"

namespace STORE
{
  class Card_Spare : public Card_symbol<Card_Spare>
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Spare(void);
   ~Card_Spare(void) noexcept(false);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);
  };

  Card_Spare* _pCard_Spare(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
