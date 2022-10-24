/*============================================================================*/
/* Card_Type.hpp -- Type                                                      */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_Type : public Card
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Type(Stz* szType);
    Card_Type(Column* pColumn);
   ~Card_Type(void) noexcept(false);
  };

  Card_Type* _pCard_Type(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
