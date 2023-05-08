/*============================================================================*/
/* Card_SortDirn.hpp -- Sort Direction                                        */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_Direction : public Card
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Direction(Stz* szDirn);
   ~Card_Direction(void) noexcept(false);

    bool bIsEqual(Card_Direction* p_direction_arg);

  };

  Card_Direction* _pCard_Direction(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
