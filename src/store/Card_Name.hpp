/*============================================================================*/
/* Card_Name.hpp -- Name                                                      */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card_SUB.hpp"

namespace STORE
{
  class Card_Name : public Card_SUB
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
