/*============================================================================*/
/* Card_Flat.hpp -- Flat (delineates the two halves after flattening)         */
/*============================================================================*/
#pragma once

#include "Card_OP.hpp"

namespace STORE
{
  class Card_Flat : public Card_OP
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Flat(void);
   ~Card_Flat(void) noexcept(false);
  };

  Card_Flat* _pCard_Flat(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
