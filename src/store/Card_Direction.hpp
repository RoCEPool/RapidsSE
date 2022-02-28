/*============================================================================*/
/* Card_SortDirn.hpp -- Sort Direction                                        */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card_SUB.hpp"

namespace STORE
{
  class Card_Direction : public Card_SUB
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Direction(Stz* szDirn);
   ~Card_Direction(void) noexcept(false);
  };

  Card_Direction* _pCard_Direction(void* aAddr);
  bool _bIsCard_Direction(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
