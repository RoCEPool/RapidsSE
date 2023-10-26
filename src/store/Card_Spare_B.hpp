/*============================================================================*/
/* Card_Spare_B.hpp -- Spare                                                    */
/*============================================================================*/
#pragma once

#include "Card_symbol.hpp"

namespace STORE
{
  class Card_Spare_B : public Card_symbol<Card_Spare_B>
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Spare_B(void);
   ~Card_Spare_B(void) noexcept(false);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);
  };

  Card_Spare_B* _pCard_Spare_B(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
