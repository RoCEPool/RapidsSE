/*============================================================================*/
/* Card_Fold.hpp -- XF_Fold (delineates the two halves during flattening)        */
/*============================================================================*/
#pragma once

#include "Card_CONSTRUCTION.hpp"

namespace STORE
{
  class SubQuery;

  class Card_Fold : public Card_CONSTRUCTION
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Fold(void);
   ~Card_Fold(void) noexcept(false);

    static Card_Fold* pCraft_fold(Stz* szFoldNm, i8 i_SqyNo);

    void vDerive_emit_fold(CardIter *pCI, SubQuery *pSubQuery_leftmost);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);

  };

  Card_Fold* _pCard_Fold(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
