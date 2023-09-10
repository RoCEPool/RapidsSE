/*============================================================================*/
/* Card_Count.hpp -- Count                                                    */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_Count : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Count(void);
   ~Card_Count(void) noexcept(false);
  };

  Card_Count* _pCard_Count(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
