/*============================================================================*/
/* Card_Required.hpp -- Required Columns                                      */
/*============================================================================*/
#pragma once

#include "Card_symbol.hpp"

namespace STORE
{
  class Card_Required : public Card_symbol<Card_Required>
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Required(void);
   ~Card_Required(void) noexcept(false);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);
  };

  Card_Required* _pCard_Required(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
