/*============================================================================*/
/* Card_CostQuery.hpp -- Estimate Cost for Query                              */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_CostQuery : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_CostQuery(void);
   ~Card_CostQuery(void) noexcept(false);

    bool bIsLessCost(Card_CostQuery* p_costQuery_arg);
  };

  Card_CostQuery* _pCard_CostQuery(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
