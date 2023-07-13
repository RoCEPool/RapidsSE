/*============================================================================*/
/* Card_PayloadCells.hpp -- Payload Cells                                     */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_PayloadCells : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_PayloadCells(void);
   ~Card_PayloadCells(void) noexcept(false);
  };

  Card_PayloadCells* _pCard_PayloadCells(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
