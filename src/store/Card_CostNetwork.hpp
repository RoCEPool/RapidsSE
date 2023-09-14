/*============================================================================*/
/* Card_CostNetwork.hpp -- Load of Network (in load of join)                  */
/*============================================================================*/
#pragma once

#include "Card_symbol.hpp"

namespace STORE
{
  class Card_CostNetwork : public Card_symbol<Card_CostNetwork>
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_CostNetwork(void);
   ~Card_CostNetwork(void) noexcept(false);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);
  };

  Card_CostNetwork* _pCard_CostNetwork(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
