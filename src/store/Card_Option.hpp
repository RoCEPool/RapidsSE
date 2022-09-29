/*============================================================================*/
/* Card_Option.hpp -- Option                                                  */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_Option : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Option(i8 iOption);
   ~Card_Option(void) noexcept(false);
  };

  Card_Option* _pCard_Option(void* aAddr);
  bool _bIsCard_Option(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
