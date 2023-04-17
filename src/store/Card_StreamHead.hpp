/*============================================================================*/
/* Card_StreamHead.hpp -- Stream Head (head of stream sequence)               */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
   class Card_StreamHead : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_StreamHead(void);
   ~Card_StreamHead(void) noexcept(false);
   };

  Card_StreamHead* _pCard_StreamHead(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
