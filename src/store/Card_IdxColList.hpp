/*============================================================================*/
/* Card_IdxColList.hpp -- Index Column List                                   */
/*============================================================================*/
#pragma once

#include "Card_symbol.hpp"

namespace STORE
{
  class Card_IdxColList : public Card_symbol<Card_IdxColList>
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Card_IdxColList(void);
   ~Card_IdxColList(void) noexcept(false);

    void vAdd_streamKey_exprs(CardIter* pCI);
    void vAffix_tblColOdn(Card_Emitted* p_emitted);
    Card_IdxCol* pGetIdxCol_byIdxColOdn(Stz* szIdxColOdn);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);
  };

  Card_IdxColList* _pCard_IdxColList(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
