/*============================================================================*/
/* Card_Spare_E.hpp -- Spare                                                    */
/*============================================================================*/
#pragma once

#include "Card_symbol.hpp"

namespace STORE
{
  class Card_Spare_E : public Card_symbol<Card_Spare_E>
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Spare_E(void);
   ~Card_Spare_E(void) noexcept(false);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);
  };

  Card_Spare_E* _pCard_Spare_E(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
