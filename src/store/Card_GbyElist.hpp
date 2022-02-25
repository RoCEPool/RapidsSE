/*============================================================================*/
/* Card_GbyElist.hpp -- List of Group By Expressions                          */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card_ELIST.hpp"

namespace STORE
{
  class Card_GbyElist : public Card_ELIST
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_GbyElist(void);
   ~Card_GbyElist(void) noexcept(false);
  };

  Card_GbyElist* _pCard_GbyElist(void* aAddr);
  bool _bIsCard_GbyElist(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
