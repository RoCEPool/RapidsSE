/*============================================================================*/
/* Card_ProjElist.hpp -- List of Project Expressions                          */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card.hpp"

namespace STORE
{
  class Card_ProjElist : public Card
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_ProjElist(void);
   ~Card_ProjElist(void) noexcept(false);
  };

  Card_ProjElist* _pCard_ProjElist(void* aAddr);
  bool _bIsCard_ProjElist(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
