/*============================================================================*/
/* Card_Scan.hpp -- Scan Card                                                 */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card_UNIT.hpp"

namespace STORE
{
  class Card_Scan : public Card_UNIT
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Scan(eCK eKind);
   ~Card_Scan(void) noexcept(false);
  };

  Card_Scan* _pCard_Scan(void* aAddr);
  bool _bIsCard_Scan(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
