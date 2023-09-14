/*============================================================================*/
/* Card_Address.hpp -- Address                                                */
/*============================================================================*/
#pragma once

#include "Card_symbol.hpp"

namespace STORE
{
  class Card_Address : public Card_symbol<Card_Address>
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Address(void);
   ~Card_Address(void) noexcept(false);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);
  };

  Card_Address* _pCard_Address(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
