/*============================================================================*/
/* Card_On.hpp -- On                                                          */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card.hpp"

namespace STORE
{
  class Card_On : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_On(Column* pColumn1, Table* pTable1,
            Column* pColumn2, Table* pTable2);
   ~Card_On(void) noexcept(false);
  };

  Card_On* _pCard_On(void* aAddr);
  bool _bIsCard_On(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
