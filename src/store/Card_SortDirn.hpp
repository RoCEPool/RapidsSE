/*============================================================================*/
/* Card_SortDirn.hpp -- Sort Direction                                        */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card.hpp"

namespace STORE
{
  class Card_SortDirn : public Card
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_SortDirn(Stz* szDirn);
   ~Card_SortDirn(void) noexcept(false);
  };

  Card_SortDirn* _pCard_SortDirn(void* aAddr);
  bool _bIsCard_SortDirn(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
