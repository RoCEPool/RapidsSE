/*============================================================================*/
/* Card_Call.hpp -- Exists                                                  */
/*============================================================================*/
#pragma once

#include "Card_symbol.hpp"

namespace STORE
{
  class Card_Call : public Card_symbol<Card_Call>
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Call(void);
   ~Card_Call(void) noexcept(false);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);
  };

  Card_Call* _pCard_Call(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
