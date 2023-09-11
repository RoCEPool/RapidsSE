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

    static void vCullUsedBounds(CardIter* pCI);

    void vAcclimate_foldExpr(CardIter* pCI, Card_Expr* p_expr_fold);
    void vBump_Rightwards(i8 nBumpBy, bool bSkipFoldExpr=false);
    i8 nCullPred(Stz* szWithMark);
    void vConvert_stmColOdn_To_stmColNm(CardIter* pCI);
    void vDisambiguate(CardIter* pCI);

    void vEnsureTypes_For_stmColOdn(CardIter *pCI);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);

  };

  Card_Constraint* _pCard_Constraint(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
