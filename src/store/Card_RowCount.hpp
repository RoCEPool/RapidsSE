/*============================================================================*/
/* Card_RowCount.hpp -- Row Count                                             */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_RowCount : public Card
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_RowCount(void);
   ~Card_RowCount(void) noexcept(false);
  };

  Card_RowCount* _pCard_RowCount(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
