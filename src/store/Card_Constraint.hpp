/*============================================================================*/
/* Card_Constraint.hpp -- Filter                                              */
/*============================================================================*/
#pragma once

#include "Card_CONDITION.hpp"

namespace STORE
{
  class Card_Constraint : public Card_CONDITION
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Constraint(void);
   ~Card_Constraint(void) noexcept(false);

    void vAcclimate_foldExpr(CardIter* pCI, Card_Expr* p_expr_fold);
    void vApplyCast(CardIter *pCI, Card_Emit* p_Dic_expr);
    void vBump_expCol(i8 nBumpBy);
    i8   nCullMarked_rlnConj(Stz* szWithMark);
    void vResolve_colNm_From_colSym(CardIter* pCI);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);
  };

  Card_Constraint* _pCard_Constraint(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
