/*============================================================================*/
/* Card_JoinOrder.hpp -- Join Order Sequence                               */
/*============================================================================*/
#pragma once

#include "Card_symbol.hpp"

namespace STORE
{
   class Card_JoinOrder : public Card_symbol<Card_JoinOrder>
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_JoinOrder(void);
   ~Card_JoinOrder(void) noexcept(false);

     void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);
   };

  Card_JoinOrder* _pCard_JoinOrder(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
