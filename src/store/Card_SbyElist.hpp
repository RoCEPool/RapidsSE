/*============================================================================*/
/* Card_ObyElist.hpp -- List of Order By Expressions                          */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card.hpp"

namespace STORE
{
  class Card_SbyElist : public Card
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_SbyElist(void);
   ~Card_SbyElist(void) noexcept(false);
  };

  Card_SbyElist* _pCard_SbyElist(void* aAddr);
  bool _bIsCard_SbyElist(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
