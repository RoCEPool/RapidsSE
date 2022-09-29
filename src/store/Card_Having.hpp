/*============================================================================*/
/* Card_Having.hpp -- Having                                                  */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card.hpp"

namespace STORE
{
  class Card_Having : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Having(void);
   ~Card_Having(void) noexcept(false);
  };

  Card_Having* _pCard_Having(void* aAddr);
  bool _bIsCard_Having(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
