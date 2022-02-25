/*============================================================================*/
/* Card_Scan.hpp -- Scan                                                      */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card_MAIN.hpp"

namespace STORE
{
  class Card_Scan : public Card_MAIN
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Scan(void);
   ~Card_Scan(void) noexcept(false);
  };

  Card_Scan* _pCard_Scan(void* aAddr);
  bool _bIsCard_Scan(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
