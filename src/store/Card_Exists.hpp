/*============================================================================*/
/* Card_Exists.hpp -- Exists                                                  */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card_MAIN.hpp"

namespace STORE
{
  class Card_Exists : public Card_MAIN
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Exists(void);
   ~Card_Exists(void) noexcept(false);
  };

  Card_Exists* _pCard_Exists(void* aAddr);
  bool _bIsCard_Exists(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
