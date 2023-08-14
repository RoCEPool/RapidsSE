/*============================================================================*/
/* Card_OuterColNm.hpp -- Parameter                                                */
/*============================================================================*/
#pragma once

#include "Card_symbol.hpp"

namespace STORE
{
  class Card_OuterColNm : public Card_symbol<Card_OuterColNm>
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_OuterColNm(void);
   ~Card_OuterColNm(void) noexcept(false);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);
  };

  Card_OuterColNm* _pCard_OuterColNm(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
