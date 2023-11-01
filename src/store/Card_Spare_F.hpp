/*============================================================================*/
/* Card_Spare_F.hpp -- Spare                                                    */
/*============================================================================*/
#pragma once

#include "Card_symbol.hpp"

namespace STORE
{
  class Card_Spare_F : public Card_symbol<Card_Spare_F>
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Spare_F(void);
   ~Card_Spare_F(void) noexcept(false);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);
  };

  Card_Spare_F* _pCard_Spare_F(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
