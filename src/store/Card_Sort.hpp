/*============================================================================*/
/* Card_Sort.hpp -- Sort                                                      */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card.hpp"

namespace STORE
{
  class Card_Sort : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Sort(void);
   ~Card_Sort(void) noexcept(false);
  };

  Card_Sort* _pCard_Sort(void* aAddr);
  bool _bIsCard_Sort(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
