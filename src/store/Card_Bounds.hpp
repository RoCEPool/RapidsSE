/*============================================================================*/
/* Card_Bounds.hpp -- Join Predicate                                        */
/*============================================================================*/
#pragma once

#include "Card_symbol.hpp"

namespace STORE
{
  class Card_Bounds : public Card_symbol<Card_Bounds>
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Bounds(void);
   ~Card_Bounds(void) noexcept(false);

    void vX_tblCol_To_expCol(CardIter* pCI);

    i8 nCullBound(Stz* szWithMark);
    Card_Bound* pFindChosenBound(void);

    void vApplyCast(CardIter *pCI, Card_Emit* p_Dic_expr);

    void vShortForm(String* pString_b);

    bool bHasEquivalentBound(Card_RlnConj* p_rlnConj);
    bool bUsesSqyRsl(void);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);
  };

  Card_Bounds* _pCard_Bounds(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
