/*============================================================================*/
/* Card_Neededv2.hpp -- Neededv2 (for emit)                                       */
/*============================================================================*/
#pragma once

#include "Card_EXPRLIST.hpp"

namespace STORE
{
  class Card_Neededv2 : public Card_EXPRLIST
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Card_Neededv2(void);
   ~Card_Neededv2(void) noexcept(false);

    Card_Neededv2* pClone(void);

    bool bNeededv2_expr(Card_Expr* p_expr_arg);

    Card_Project* pNew_project_early(CardIter* pCI);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);
  };

  Card_Neededv2* _pCard_Neededv2(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
