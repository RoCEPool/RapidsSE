/*============================================================================*/
/* Card_PmzWhere.hpp -- Parameterised Where                                   */
/*============================================================================*/
#pragma once

#include "Card.hpp"
#include "Card_PRED.hpp"

namespace STORE
{
  class Card_PmzWhere : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_PmzWhere(void);
   ~Card_PmzWhere(void) noexcept(false);
  };

  Card_PmzWhere* _pCard_PmzWhere(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
