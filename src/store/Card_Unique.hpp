/*============================================================================*/
/* Card_Unique.hpp -- Unique                                                  */
/*============================================================================*/
#pragma once

#include "Card_symbol.hpp"

namespace STORE
{
  class Card_Unique : public Card_symbol<Card_Unique>
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Unique(void);
   ~Card_Unique(void) noexcept(false);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);
  };

  Card_Unique* _pCard_Unique(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
