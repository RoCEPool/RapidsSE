/*============================================================================*/
/* Card_CreateTable.hpp -- Create Table                                       */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card.hpp"

namespace STORE
{
  class Card_CreateTable : public Card
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_CreateTable(eCK eKind);
   ~Card_CreateTable(void) noexcept(false);
  };

  Card_CreateTable* _pCard_CreateTable(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
