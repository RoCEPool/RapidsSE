/*============================================================================*/
/* Card_StmColOrd.hpp -- Stream Column Ordinal                                */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_StmColOrd : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_StmColOrd(void);
   ~Card_StmColOrd(void) noexcept(false);
  };

  Card_StmColOrd* _pCard_StmColOrd(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
