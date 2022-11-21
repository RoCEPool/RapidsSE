/*============================================================================*/
/* Card_FoldExpr.hpp -- Fold Typed Expression                                 */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_FoldExpr : public Card
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_FoldExpr(void);
   ~Card_FoldExpr(void) noexcept(false);
  };

  Card_FoldExpr* _pCard_FoldExpr(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
