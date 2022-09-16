/*============================================================================*/
/* Card_Map.hpp -- Map                                                        */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card_SUB.hpp"

namespace STORE
{
  class Card_Map : public Card_SUB
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Map(void);
   ~Card_Map(void) noexcept(false);
  };

  Card_Map* _pCard_Map(void* aAddr);
  void _Card_Map(void* aAddr);
  bool _bIsCard_Map(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
