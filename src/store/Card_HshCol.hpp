/*============================================================================*/
/* Card_HshCol.hpp -- Hash Column                                             */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_HshCol : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_HshCol(void);
   ~Card_HshCol(void) noexcept(false);

  };

  Card_HshCol* _pCard_HshCol(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
