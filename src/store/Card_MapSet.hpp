/*============================================================================*/
/* Card_MapSet.hpp -- MapSet                                                  */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_MapSet : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_MapSet(void);
    Card_MapSet(Index* pIndex, Stz* szPrefix);
   ~Card_MapSet(void) noexcept(false);

    Stz* szMapBack(Stz* szMapFrom);
  };

  Card_MapSet* _pCard_MapSet(void* aAddr);
  bool _bIsCard_MapSet(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
