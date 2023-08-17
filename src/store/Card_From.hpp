/*============================================================================*/
/* Card_From.hpp -- From                                                      */
/*============================================================================*/
#pragma once

#include "Card_OP.hpp"

namespace STORE
{
  class CarryForward;

  class Card_From : public Card_OP
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_From(void);
   ~Card_From(void) noexcept(false);

    Stz* szColNm(i8 i_Col_base0);
    static void vProduceTablePtns(CardIter* pCI);
    void vSmooth(CardIter* pCI);
    void vSmooth(CardIter* pCI, CarryForward* pCarryForward);
    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);
  };

  Card_From* _pCard_From(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
