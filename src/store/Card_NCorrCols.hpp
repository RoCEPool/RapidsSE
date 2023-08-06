/*============================================================================*/
/* Card_NCorrCols.hpp -- Number of Correlated Columns                         */
/*============================================================================*/
#pragma once

#include "Card_symbol.hpp"

namespace STORE
{
  class Card_NCorrCols : public Card_symbol<Card_NCorrCols>
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
