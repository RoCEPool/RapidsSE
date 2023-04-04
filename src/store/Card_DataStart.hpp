/*============================================================================*/
/* Card_DataStart.hpp -- Join Head (head of join sequence)                     */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
   class Card_DataStart : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_DataStart(void);
   ~Card_DataStart(void) noexcept(false);
   };

  Card_DataStart* _pCard_DataStart(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
