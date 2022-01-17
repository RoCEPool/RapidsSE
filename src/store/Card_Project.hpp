/*============================================================================*/
/* Card_Project.hpp -- List of Project Cards                              */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card_UNIT.hpp"

namespace STORE
{
  class Card_Project : public Card_UNIT
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Project(eCK eKind);
   ~Card_Project(void) noexcept(false);
  };

  Card_Project* _pCard_Project(void* aAddr);
  bool _bIsCard_Project(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
