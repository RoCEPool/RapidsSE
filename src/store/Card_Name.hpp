/*============================================================================*/
/* Card_Name.hpp -- Name                                                      */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_Name : public Card
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Name(Stz* szName);
   ~Card_Name(void) noexcept(false);
  };

  Card_Name* _pCard_Name(void* aAddr);
  bool _bIsCard_Name(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
