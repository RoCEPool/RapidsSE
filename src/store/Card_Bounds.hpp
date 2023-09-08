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

    void vConvert_colOdn_To_stmOdn(CardIter* pCI);

    i8 nCullBound(Stz* szWithMark);
    Card_Bound* pFindChosenBound(void);

    void vEnsureTypes_For_stmOdn(CardIter *pCI);

    bool bHasEquivalentBound(Card_CmpPred* p_cmpPred);
    bool bUsesSqyRsl(void);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);
  };

  Card_Bounds* _pCard_Bounds(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
