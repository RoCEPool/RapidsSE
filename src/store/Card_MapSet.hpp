/*============================================================================*/
/* Card_MapSet.hpp -- MapSet                                                  */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card_SUB.hpp"

namespace STORE
{
  class Card_MapSet : public Card_SUB
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_MapSet(void);
    Card_MapSet(Index* pIndex, Stz* szPrefix);
   ~Card_MapSet(void) noexcept(false);
  };

  Card_MapSet* _pCard_MapSet(void* aAddr);
  bool _bIsCard_MapSet(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
