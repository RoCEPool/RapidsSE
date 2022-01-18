/*============================================================================*/
/* Card_By.hpp -- By                                                          */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card.hpp"

namespace STORE
{
  class Card_By : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_By(void);
   ~Card_By(void) noexcept(false);
  };

  Card_By* _pCard_By(void* aAddr);
  bool _bIsCard_By(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
