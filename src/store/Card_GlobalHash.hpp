/*============================================================================*/
/* Card_GlobalHash.hpp -- Global Hash                                         */
/*============================================================================*/
#pragma once

#include "Card_OP.hpp"

namespace STORE
{
  class Card_GlobalHash : public Card_OP
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_GlobalHash(void);
   ~Card_GlobalHash(void) noexcept(false);

    Card_Expr* pNew_expr_rrf(void);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);
  };

  Card_GlobalHash* _pCard_GlobalHash(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
