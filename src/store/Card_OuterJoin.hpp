/*============================================================================*/
/* Card_OuterJoin.hpp -- Outer Join indicator                                 */
/*============================================================================*/
#pragma once

#include "Card_PRED.hpp"

namespace STORE
{
  class Card_OuterJoin : public Card
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_OuterJoin(void);
   ~Card_OuterJoin(void) noexcept(false);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);
  };

  Card_OuterJoin* _pCard_OuterJoin(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
