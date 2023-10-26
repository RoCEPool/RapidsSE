/*============================================================================*/
/* Card_SqyNo.hpp -- Spare                                                    */
/*============================================================================*/
#pragma once

#include "Card_symbol.hpp"

namespace STORE
{
  class Card_SqyNo : public Card_symbol<Card_SqyNo>
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_SqyNo(void);
   ~Card_SqyNo(void) noexcept(false);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);
  };

  Card_SqyNo* _pCard_SqyNo(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
