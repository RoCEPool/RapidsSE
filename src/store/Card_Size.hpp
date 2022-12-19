/*============================================================================*/
/* Card_Size.hpp -- Size (of table)                                           */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_Size : public Card
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Size(void);
   ~Card_Size(void) noexcept(false);

   static Card_Size* pDerive(Table* pTable);
  };

  Card_Size* _pCard_Size(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
