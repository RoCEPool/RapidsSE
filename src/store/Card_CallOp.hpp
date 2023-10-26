/*============================================================================*/
/* Card_CallOp.hpp -- Spare                                                    */
/*============================================================================*/
#pragma once

#include "Card_symbol.hpp"

namespace STORE
{
  class Card_CallOp : public Card_symbol<Card_CallOp>
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_CallOp(void);
   ~Card_CallOp(void) noexcept(false);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);
  };

  Card_CallOp* _pCard_CallOp(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
