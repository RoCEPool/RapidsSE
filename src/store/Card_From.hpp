/*============================================================================*/
/* Card_From.hpp -- From                                                      */
/*============================================================================*/
#pragma once

#include "Card_symbol.hpp"

namespace STORE
{
  class Card_From : public Card_symbol<Card_From>
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_From(void);
   ~Card_From(void) noexcept(false);

    Stz* szColNm(i8 i_Col_base0);
    static void vProduceTablePtns(CardIter* pCI);
    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);
  };

  Card_From* _pCard_From(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
