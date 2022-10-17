/*============================================================================*/
/* Card_Param.hpp -- Parameter                                                */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_Param : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Param(void);
   ~Card_Param(void) noexcept(false);
  };

  Card_Param* _pCard_Param(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
