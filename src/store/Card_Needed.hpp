/*============================================================================*/
/* Card_Projection.hpp -- Needed (for projection)                             */
/*============================================================================*/
#pragma once

#include "Card_Projection.hpp"

namespace STORE
{
  class Card_Needed : public Card_Projection
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Card_Needed(void);
   ~Card_Needed(void) noexcept(false);

    Card_Needed* pClone(void);
  };

  Card_Needed* _pCard_Needed(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
