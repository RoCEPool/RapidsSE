/*============================================================================*/
/* Card_FiltElist.hpp -- List of Filter Expressions                           */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card.hpp"

namespace STORE
{
  class Card_FiltElist : public Card
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_FiltElist(void);
   ~Card_FiltElist(void) noexcept(false);
  };

  Card_FiltElist* _pCard_FiltElist(void* aAddr);
  bool _bIsCard_FiltElist(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
