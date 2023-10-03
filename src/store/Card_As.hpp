/*============================================================================*/
/* Card_As.hpp -- As                                                          */
/*============================================================================*/
#pragma once

#include "Card_symbol.hpp"

namespace STORE
{
  class Card_As : public Card_symbol<Card_As>
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Card_As(void);
   ~Card_As(void) noexcept(false);

    static void vItemise_colNm(CardIter* pCI);
    static void vItemStrip_colNm(CardIter* pCI);

    Card_As* pClone(void);

    bool bIs_SQCRLKEY(void);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);
  };

  Card_As* _pCard_As(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
