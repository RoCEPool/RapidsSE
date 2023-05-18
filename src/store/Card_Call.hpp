/*============================================================================*/
/* Card_Call.hpp -- Exists                                                  */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_Call : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Call(void);
   ~Card_Call(void) noexcept(false);

    void vValidate(i8 iDepth);
  };

  Card_Call* _pCard_Call(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
