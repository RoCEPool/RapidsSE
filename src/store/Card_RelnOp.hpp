/*============================================================================*/
/* Card_RelnOp.hpp -- Spare                                                    */
/*============================================================================*/
#pragma once

#include "Card_symbol.hpp"

namespace STORE
{
  class Card_RelnOp : public Card_symbol<Card_RelnOp>
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_RelnOp(void);
   ~Card_RelnOp(void) noexcept(false);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);
  };

  Card_RelnOp* _pCard_RelnOp(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
