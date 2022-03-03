/*============================================================================*/
/* Card_Stream.hpp -- Stream                                                  */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card_SUB.hpp"

namespace STORE
{
  class Card_Stream : public Card_SUB
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Stream(void);
   ~Card_Stream(void) noexcept(false);
  };

  Card_Stream* _pCard_Stream(void* aAddr);
  bool _bIsCard_Stream(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
