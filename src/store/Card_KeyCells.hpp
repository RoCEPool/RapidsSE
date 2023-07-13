/*============================================================================*/
/* Card_KeyCells.hpp -- Key Cells                                             */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_KeyCells : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_KeyCells(void);
   ~Card_KeyCells(void) noexcept(false);
  };

  Card_KeyCells* _pCard_KeyCells(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
