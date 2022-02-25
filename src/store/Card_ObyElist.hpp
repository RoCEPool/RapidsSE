/*============================================================================*/
/* Card_ObyElist.hpp -- List of Order By Expressions                          */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card.hpp"

namespace STORE
{
  class Card_ObyElist : public Card
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_ObyElist(void);
   ~Card_ObyElist(void) noexcept(false);
  };

  Card_ObyElist* _pCard_ObyElist(void* aAddr);
  bool _bIsCard_ObyElist(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
