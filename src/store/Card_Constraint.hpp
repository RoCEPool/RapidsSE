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

    void vApplyCast(CardIter *pCI, Card_Emit* p_Dic_expr);
    i8   nCullMarked_rlnConj(Stz* szWithMark);
    void vResolve_colNm_From_colSym(CardIter* pCI);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);
  };

  Card_Constraint* _pCard_Constraint(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
