/*============================================================================*/
/* Card_On.hpp -- On Predicate                                                */
/*============================================================================*/
#pragma once

#include "Card_PRED.hpp"

namespace STORE
{
  class Card_On : public Card_PRED
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_On(void);
   ~Card_On(void) noexcept(false);

  };

  Card_On* _pCard_On(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
