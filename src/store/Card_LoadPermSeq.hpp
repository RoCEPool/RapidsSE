/*============================================================================*/
/* Card_LoadPermSeq.hpp -- Load of join Permutation Sequence                  */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_LoadPermSeq : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_LoadPermSeq(void);
   ~Card_LoadPermSeq(void) noexcept(false);

  };

  Card_LoadPermSeq* _pCard_LoadPermSeq(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
