/*============================================================================*/
/* Card_Spare_A.hpp -- Spare                                                    */
/*============================================================================*/
#pragma once

#include "Card_symbol.hpp"

namespace STORE
{
  class Card_Spare_A : public Card_symbol<Card_Spare_A>
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Spare_A(void);
   ~Card_Spare_A(void) noexcept(false);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);
  };

  Card_Spare_A* _pCard_Spare_A(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
