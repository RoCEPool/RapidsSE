/*============================================================================*/
/* Card_Cell.hpp -- Cell                                                      */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_Cell : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Cell(void);
   ~Card_Cell(void) noexcept(false);
  };

  Card_Cell* _pCard_Cell(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
