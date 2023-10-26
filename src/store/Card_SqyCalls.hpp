/*============================================================================*/
/* Card_SqyCalls.hpp -- Spare                                                    */
/*============================================================================*/
#pragma once

#include "Card_symbol.hpp"

namespace STORE
{
  class Card_SqyCalls : public Card_symbol<Card_SqyCalls>
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_SqyCalls(void);
   ~Card_SqyCalls(void) noexcept(false);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);
  };

  Card_SqyCalls* _pCard_SqyCalls(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
