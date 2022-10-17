/*============================================================================*/
/* Card_ParamList.hpp -- Parameter List                                       */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_ParamList : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_ParamList(void);
   ~Card_ParamList(void) noexcept(false);
  };

  Card_ParamList* _pCard_ParamList(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
