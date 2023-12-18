/*============================================================================*/
/* Card_Needed.hpp -- Needed (for projection)                                 */
/*============================================================================*/
#pragma once

#include "Card_EXPRLIST.hpp"

namespace STORE
{
  class Card_Needed : public Card_EXPRLIST
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Card_Needed(void);
   ~Card_Needed(void) noexcept(false);

    Card_Needed* pClone(void);

    bool bNeeded_expr(Card_Expr* p_expr_arg);

    Card_Project* pNew_project_early(CardIter* pCI);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);
  };

  Card_Needed* _pCard_Needed(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
