/*============================================================================*/
/* Card_NParmCols.hpp -- Number of Parameter Columns                          */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_NParmCols : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_NParmCols(void);
   ~Card_NParmCols(void) noexcept(false);
  };

  Card_NParmCols* _pCard_NParmCols(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
