/*============================================================================*/
/* Card_PtnCol.hpp -- Partition Column Name                                 */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_PtnCol : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_PtnCol(void);
   ~Card_PtnCol(void) noexcept(false);
  };

  Card_PtnCol* _pCard_PtnCol(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
