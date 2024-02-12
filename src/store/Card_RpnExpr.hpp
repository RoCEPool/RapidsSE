/*============================================================================*/
/* Card_RpnExpr.hpp -- RPN Sequence of Terms                                 */
/*============================================================================*/
#pragma once

#include "Card_symbol.hpp"

namespace STORE
{
  class Card_RpnExpr : public Card_symbol<Card_RpnExpr>
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_RpnExpr(void);
   ~Card_RpnExpr(void) noexcept(false);

  };

  Card_RpnExpr* _pCard_RpnExpr(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
