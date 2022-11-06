/*============================================================================*/
/* Card_Twist.hpp -- Twist                                                    */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_Twist : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Twist(void);
   ~Card_Twist(void) noexcept(false);
  };

  Card_Twist* _pCard_Twist(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
