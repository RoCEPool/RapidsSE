/*============================================================================*/
/* Card_StreamPtn.hpp -- Stream Partition                                     */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_StreamPtn : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_StreamPtn(void);
   ~Card_StreamPtn(void) noexcept(false);

  };

  Card_StreamPtn* _pCard_StreamPtn(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
