/*============================================================================*/
/* Card_Hash.hpp -- Hash                                                      */
/*============================================================================*/
#pragma once

#include "Card_OP.hpp"

namespace STORE
{
  class Card_Hash : public Card_OP
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Hash(void);
   ~Card_Hash(void) noexcept(false);

    static Card_Hash* pDerive(Card_Expr* p_expr);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);

  };

  Card_Hash* _pCard_Hash(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
