/*============================================================================*/
/* Card_NCorrCols.hpp -- Number of Correlated Columns                         */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_NCorrCols : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_NCorrCols(void);
   ~Card_NCorrCols(void) noexcept(false);
  };

  Card_NCorrCols* _pCard_NCorrCols(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
