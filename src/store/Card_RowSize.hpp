/*============================================================================*/
/* Card_RowSize.hpp -- Row Size                                               */
/*============================================================================*/
#pragma once

#include "Card_t.hpp"

namespace STORE
{
  class Card_RowSize : public Card
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_RowSize(void);
   ~Card_RowSize(void) noexcept(false);
  };

  Card_RowSize* _pCard_RowSize(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
