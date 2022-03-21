/*============================================================================*/
/* Card_Table.hpp -- Table                                                    */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card_OBJECT.hpp"

namespace STORE
{
  class Card_Table : public Card_OBJECT
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Table(Table* pTable);
   ~Card_Table(void) noexcept(false);
  };

  Card_Table* _pCard_Table(void* aAddr);
  bool _bIsCard_Table(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
