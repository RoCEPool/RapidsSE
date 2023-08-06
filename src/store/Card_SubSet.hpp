/*============================================================================*/
/* Card_SubSet.hpp -- SubSet                                                  */
/*============================================================================*/
#pragma once

#include "Card_t.hpp"

namespace STORE
{
  class Card_SubSet : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_SubSet(void);
   ~Card_SubSet(void) noexcept(false);
  };

  Card_SubSet* _pCard_SubSet(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
