/*============================================================================*/
/* Card_ColumnRef.hpp -- Column Reference                                     */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_ColumnRef : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_ColumnRef(Stz* szColumnRef);
   ~Card_ColumnRef(void) noexcept(false);
  };

  Card_ColumnRef* _pCard_ColumnRef(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
