/*============================================================================*/
/* Card_GbyList.hpp -- List of Group By                                       */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card.hpp"

namespace STORE
{
  class Card_GbyList : public Card
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_GbyList(void);
   ~Card_GbyList(void) noexcept(false);
  };

  Card_GbyList* _pCard_GbyList(void* aAddr);
  bool _bIsCard_GbyList(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
