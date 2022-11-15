/*============================================================================*/
/* Card_Clause.hpp -- Clause (e.g. from, having or where)                     */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_Clause : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Clause(void);
   ~Card_Clause(void) noexcept(false);
  };

  Card_Clause* _pCard_Clause(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
