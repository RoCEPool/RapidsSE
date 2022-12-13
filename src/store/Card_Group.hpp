/*============================================================================*/
/* Card_Group.hpp -- Group information                                        */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_Group : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Group(void);
   ~Card_Group(void) noexcept(false);
  };

  Card_Group* _pCard_Group(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
