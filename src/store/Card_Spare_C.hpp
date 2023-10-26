/*============================================================================*/
/* Card_Spare_C.hpp -- Spare                                                    */
/*============================================================================*/
#pragma once

#include "Card_symbol.hpp"

namespace STORE
{
  class Card_Spare_C : public Card_symbol<Card_Spare_C>
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Spare_C(void);
   ~Card_Spare_C(void) noexcept(false);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);
  };

  Card_Spare_C* _pCard_Spare_C(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
