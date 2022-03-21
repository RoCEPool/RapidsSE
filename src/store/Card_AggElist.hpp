/*============================================================================*/
/* Card_AggElist.hpp -- List of Group By Expressions                          */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card_ELIST.hpp"

namespace STORE
{
  class Card_AggElist : public Card_ELIST
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_AggElist(void);
   ~Card_AggElist(void) noexcept(false);
  };

  Card_AggElist* _pCard_AggElist(void* aAddr);
  bool _bIsCard_AggElist(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
